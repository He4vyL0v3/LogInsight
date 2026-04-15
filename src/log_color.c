/**
 * @file log_color.c
 * @brief Colorized log output for terminal display.
 *
 * Provides functions to colorize log lines based on their log level,
 * improving readability in terminal output.
 */

#include "log_color.h"
#include "log_levels.h"
#include <stdio.h>

/**
 * @brief Colorizes and prints a log line based on its detected level.
 *
 * Searches the log line for known level markers and prints the line
 * with the corresponding color. Uses pre-compiled regex patterns
 * for efficient matching.
 *
 * @param line The log line to colorize and print. A newline is added
 *             after the line.
 */
void colorize_log(const char *line)
{
    for (int i = 0; i < LOG_LEVEL_COUNT; i++)
    {
        if (log_level_matches(line, (LogLevelType)i))
        {
            printf("%s%s%s\n", LOG_LEVELS[i].color, line, LOG_COLOR_RESET);
            return;
        }
    }
    printf("%s\n", line);
}
