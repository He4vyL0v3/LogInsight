/**
 * @file log_filter.h
 * @brief Interface for log filtering by level and date range.
 */

#ifndef LOG_FILTER_H
#define LOG_FILTER_H

#include <stddef.h>

/**
 * @brief Checks if a log line passes the filters.
 *
 * @param line The log line to check.
 * @param filter_levels Array of level names to filter by.
 * @param filter_count Number of filter levels.
 * @param start_date_str Start of date range (or NULL).
 * @param end_date_str End of date range (or NULL).
 * @return Non-zero if line should be printed.
 */
int should_print_log(const char *line, char *filter_levels[], int filter_count, const char *start_date_str,
                     const char *end_date_str);
#endif // LOG_FILTER_H
