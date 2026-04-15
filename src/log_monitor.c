/**
 * @file log_monitor.c
 * @brief Core log monitoring and processing functionality.
 *
 * Handles file monitoring, real-time updates via inotify, line processing,
 * filtering, and statistics collection. This is the main orchestrator
 * module that coordinates all other components.
 */

#include "log_monitor.h"
#include "file_size.h"
#include "log_color.h"
#include "log_filter.h"
#include "log_levels.h"
#include "log_statistics.h"
#include "performance_monitor.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define INITIAL_BUFFER_SIZE 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * EVENT_SIZE)

long int critical_count = 0;
long int warning_count = 0;
long int info_count = 0;
long int debug_count = 0;
long int error_count = 0;
long int trace_count = 0;
long int unknown_count = 0;
long int fatal_count = 0;

static int running = 1;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Signal handler for graceful shutdown.
 *
 * Catches SIGINT (Ctrl+C) and sets the running flag to 0,
 * allowing the main loop to exit cleanly.
 *
 * @param signal The signal number received.
 */
void handle_signal(int signal)
{
    (void)signal;
    if (running)
    {
        running = 0;
        printf("\033[1;31m\n\nBye...\n\033[0m");
    }
}

/**
 * @brief Increments the count for the matched log level.
 *
 * Thread-safe function that uses a mutex to protect the
 * global count variables. Uses pre-compiled regex patterns
 * from log_levels.h for efficient matching.
 *
 * @param line The log line to analyze.
 */
void count_log_levels(const char *line)
{
    pthread_mutex_lock(&count_mutex);

    bool matched = false;

    for (int i = 0; i < LOG_LEVEL_COUNT; i++)
    {
        if (log_level_matches(line, (LogLevelType)i))
        {
            matched = true;
            switch (i)
            {
            case LOG_LEVEL_CRITICAL:
                critical_count++;
                break;
            case LOG_LEVEL_WARNING:
                warning_count++;
                break;
            case LOG_LEVEL_INFO:
                info_count++;
                break;
            case LOG_LEVEL_DEBUG:
                debug_count++;
                break;
            case LOG_LEVEL_ERROR:
                error_count++;
                break;
            case LOG_LEVEL_UNKNOWN:
                unknown_count++;
                break;
            case LOG_LEVEL_TRACE:
                trace_count++;
                break;
            case LOG_LEVEL_FATAL:
                fatal_count++;
                break;
            default:
                break;
            }
            break;
        }
    }

    if (!matched)
    {
        unknown_count++;
    }

    pthread_mutex_unlock(&count_mutex);
}

/**
 * @brief Processes a single log line through filters and output.
 *
 * Checks if the line passes all filters, then either prints it
 * with colorization or just counts it. Statistics are always updated.
 *
 * @param line The log line to process.
 * @param filter_levels Array of level names to filter by.
 * @param filter_count Number of filter levels specified.
 * @param print_lines Whether to actually print matching lines.
 * @param start_date Start of date range filter (or NULL).
 * @param end_date End of date range filter (or NULL).
 */
void process_line(const char *line, char *filter_levels[], int filter_count, int print_lines,
                  const char *const start_date, const char *const end_date)
{
    if (should_print_log(line, filter_levels, filter_count, start_date, end_date))
    {
        if (print_lines)
        {
            colorize_log(line);
            count_log_levels(line);
        }
        else
        {
            count_log_levels(line);
        }
    }
    else
    {
        count_log_levels(line);
    }
}

/**
 * @brief Starts the log monitoring process.
 *
 * Opens the specified file (or reads from stdin), optionally enables
 * real-time monitoring with inotify, processes all lines, and displays
 * statistics at the end.
 *
 * @param file_name Path to log file, or NULL for stdin.
 * @param filter_levels Array of level names to filter by.
 * @param filter_count Number of filter levels specified.
 * @param real_time Enable real-time file monitoring.
 * @param show_stats Display performance statistics.
 * @param print_lines Whether to print matching lines.
 * @param start_date Start of date range filter (or NULL).
 * @param end_date End of date range filter (or NULL).
 */
void start_log_monitor(const char *file_name, char *filter_levels[], int filter_count, int real_time, int show_stats,
                       int print_lines, const char *start_date, const char *end_date)
{
    signal(SIGINT, handle_signal);

    log_level_compile_all();
    if (show_stats)
    {
        start_monitoring();
    }

    bool use_stdin = (file_name == NULL);
    int fd;
    if (use_stdin)
    {
        fd = STDIN_FILENO;
    }
    else
    {
        fd = open(file_name, O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            log_level_free_all();
            return;
        }
    }

    char *buffer = malloc(INITIAL_BUFFER_SIZE);
    if (buffer == NULL)
    {
        perror("malloc");
        if (!use_stdin)
            close(fd);
        log_level_free_all();
        return;
    }
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t current_length = 0;

    if (real_time && !use_stdin)
    {
        int inotify_fd = inotify_init();
        if (inotify_fd < 0)
        {
            perror("inotify_init");
            close(fd);
            free(buffer);
            log_level_free_all();
            return;
        }

        int wd = inotify_add_watch(inotify_fd, file_name, IN_MODIFY);
        if (wd == -1)
        {
            perror("inotify_add_watch");
            close(fd);
            close(inotify_fd);
            free(buffer);
            log_level_free_all();
            return;
        }

        off_t offset = lseek(fd, 0, SEEK_END);

        while (running)
        {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(inotify_fd, &readfds);

            struct timeval tv = {1, 0};

            int retval = select(inotify_fd + 1, &readfds, NULL, NULL, &tv);

            if (retval == -1)
            {
                perror("select");
                break;
            }
            else if (retval == 0)
            {
                continue;
            }

            char event_buf[EVENT_BUF_LEN];

            ssize_t bytes = read(inotify_fd, event_buf, EVENT_BUF_LEN);

            if (bytes < 0)
            {
                perror("read");
                break;
            }

            lseek(fd, offset, SEEK_SET);
            ssize_t bytes_read = read(fd, buffer + current_length, buffer_size - current_length - 1);

            if (bytes_read > 0)
            {
                current_length += bytes_read;
                buffer[current_length] = '\0';

                char *line_start = buffer;
                char *line_end;

                while ((line_end = strchr(line_start, '\n')) != NULL)
                {
                    *line_end = '\0';
                    process_line(line_start, filter_levels, filter_count, print_lines, start_date, end_date);
                    line_start = line_end + 1;
                }

                current_length -= (size_t)(line_start - buffer);
                memmove(buffer, line_start, current_length);
                offset += bytes_read;
            }
        }

        inotify_rm_watch(inotify_fd, wd);
        close(inotify_fd);

        print_file_size(file_name);
        print_statistics();
    }
    else
    {
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer + current_length, buffer_size - current_length - 1)) > 0)
        {
            current_length += bytes_read;
            buffer[current_length] = '\0';

            char *line_start = buffer;
            char *line_end;

            while ((line_end = strchr(line_start, '\n')) != NULL)
            {
                *line_end = '\0';
                process_line(line_start, filter_levels, filter_count, print_lines, start_date, end_date);
                line_start = line_end + 1;
            }

            current_length -= (size_t)(line_start - buffer);
            memmove(buffer, line_start, current_length);
        }

        if (!use_stdin)
        {
            close(fd);
            print_file_size(file_name);
        }
        print_statistics();
    }

    log_level_free_all();
    free(buffer);
    if (show_stats)
    {
        stop_monitoring();
    }
}
