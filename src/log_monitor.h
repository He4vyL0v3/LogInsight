/**
 * @file log_monitor.h
 * @brief Interface for the core log monitoring module.
 */

#ifndef LOG_MONITOR_H
#define LOG_MONITOR_H

#include <pthread.h>

/**
 * @brief Starts monitoring and processing a log file.
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
                       int print_lines, const char *start_date, const char *end_date);

/**
 * @brief Processes a single log line.
 *
 * @param line The log line to process.
 * @param filter_levels Array of level names to filter by.
 * @param filter_count Number of filter levels specified.
 * @param print_lines Whether to print matching lines.
 * @param start_date Start of date range filter (or NULL).
 * @param end_date End of date range filter (or NULL).
 */
void process_line(const char *line, char *filter_levels[], int filter_count, int print_lines, const char *start_date,
                  const char *end_date);

/**
 * @brief Counts the log line by its detected level.
 *
 * @param line The log line to analyze.
 */
void count_log_levels(const char *line);

/**
 * @brief Mutex for thread-safe count operations.
 */
extern pthread_mutex_t count_mutex;

/**
 * @brief Count variables for each log level.
 */
extern long int critical_count;
extern long int warning_count;
extern long int info_count;
extern long int error_count;
extern long int debug_count;
extern long int trace_count;
extern long int unknown_count;
extern long int fatal_count;

#endif // LOG_MONITOR_H
