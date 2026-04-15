/**
 * @file log_statistics.h
 * @brief Interface for log statistics tracking and reporting.
 */

#ifndef LOG_STATISTICS_H
#define LOG_STATISTICS_H

#include <stdio.h>

/**
 * @brief Prints formatted statistics for all processed log levels.
 *
 * Shows a summary of log line counts by level, with non-zero levels
 * displayed in their respective colors.
 */
void print_statistics(void);

#endif // LOG_STATISTICS_H
