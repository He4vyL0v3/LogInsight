/**
 * @file log_statistics.c
 * @brief Log statistics collection and reporting.
 *
 * Tracks and displays statistics about processed log lines,
 * organized by log level.
 */

#include "log_statistics.h"
#include "log_levels.h"
#include <stdio.h>

/**
 * @brief Counts for each log level.
 *
 * These are defined in log_monitor.c and externed here.
 */
extern long int critical_count;
extern long int warning_count;
extern long int info_count;
extern long int error_count;
extern long int debug_count;
extern long int trace_count;
extern long int unknown_count;
extern long int fatal_count;

/**
 * @brief Maps log level types to their corresponding count variables.
 *
 * Provides access to count variables by log level index.
 *
 * @param level The log level type.
 * @return Pointer to the count variable for the given level.
 */
static long int *get_count_ptr(LogLevelType level)
{
    switch (level)
    {
    case LOG_LEVEL_CRITICAL:
        return &critical_count;
    case LOG_LEVEL_ERROR:
        return &error_count;
    case LOG_LEVEL_FATAL:
        return &fatal_count;
    case LOG_LEVEL_WARNING:
        return &warning_count;
    case LOG_LEVEL_INFO:
        return &info_count;
    case LOG_LEVEL_DEBUG:
        return &debug_count;
    case LOG_LEVEL_TRACE:
        return &trace_count;
    case LOG_LEVEL_UNKNOWN:
    default:
        return &unknown_count;
    }
}

/**
 * @brief Prints formatted statistics for all log levels.
 *
 * Displays a summary table of log counts, only showing levels
 * with non-zero counts. Output is colorized by level.
 */
void print_statistics(void)
{
    printf("\033[1;34m┌─────────────────────────────⬤ \n");
    printf("│    Log Statistics:\n");

    for (int i = 0; i < LOG_LEVEL_COUNT; i++)
    {
        long int *count = get_count_ptr((LogLevelType)i);
        if (*count > 0)
        {
            printf("\033[0;34m│%s ⬤ %s: %ld\n", LOG_LEVELS[i].color, LOG_LEVELS[i].label, *count);
        }
    }

    printf("\033[1;34m└─────────────────────────────⬤ \n");
}
