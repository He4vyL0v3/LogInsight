/**
 * @file log_filter.c
 * @brief Log filtering by level and date range.
 *
 * Provides functions to filter log lines based on level names
 * and date/time ranges. Used by the main monitoring loop
 * to decide which lines to display.
 */

#define _XOPEN_SOURCE 700

#include "log_filter.h"
#include "log_format.h"
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_FILTER_LENGTH 256
#define DATE_BUFFER_SIZE 64

/**
 * @brief Extracts a date substring from a log line.
 *
 * Uses a regex to find and extract the date portion of a log line.
 *
 * @param line The log line to search.
 * @param date_regex Regex pattern for the date format.
 * @param date_buf Output buffer for the extracted date.
 * @param buf_size Size of the output buffer.
 * @return 1 on success, 0 on failure.
 */
static int extract_date_from_line(const char *line, const char *date_regex, char *date_buf, size_t buf_size)
{
    regex_t regex;
    if (regcomp(&regex, date_regex, REG_EXTENDED) != 0)
        return 0;

    regmatch_t match[1];
    if (regexec(&regex, line, 1, match, 0) == 0)
    {
        size_t len = (size_t)(match[0].rm_eo - match[0].rm_so);
        if (len >= buf_size)
            len = buf_size - 1;
        memcpy(date_buf, line + match[0].rm_so, len);
        date_buf[len] = '\0';
        regfree(&regex);
        return 1;
    }

    regfree(&regex);
    return 0;
}

/**
 * @brief Parses a date string into a time_t value.
 *
 * @param date_str The date string to parse.
 * @param fmt The strptime format string.
 * @return Time value, or (time_t)-1 on failure.
 */
static time_t parse_date_to_time_t(const char *date_str, const char *fmt)
{
    struct tm tm = {0};
    if (strptime(date_str, fmt, &tm) == NULL)
        return (time_t)-1;
    return mktime(&tm);
}

/**
 * @brief Determines if a log line should be printed.
 *
 * Checks the line against filter levels (if any) and date range.
 *
 * @param line The log line to check.
 * @param filter_levels Array of level names to filter by.
 * @param filter_count Number of filter levels.
 * @param start_date_str Start of date range (or NULL).
 * @param end_date_str End of date range (or NULL).
 * @return 1 if line should be printed, 0 otherwise.
 */
int should_print_log(const char *line, char *filter_levels[], int filter_count, const char *start_date_str,
                     const char *end_date_str)
{
    if (filter_count > 0)
    {
        for (int j = 0; j < filter_count; j++)
        {
            char filter_level_lower[MAX_FILTER_LENGTH];
            size_t len = strlen(filter_levels[j]);

            if (len >= MAX_FILTER_LENGTH)
                len = MAX_FILTER_LENGTH - 1;

            for (size_t i = 0; i < len; i++)
                filter_level_lower[i] = (char)tolower((unsigned char)filter_levels[j][i]);
            filter_level_lower[len] = '\0';

            char pattern[MAX_FILTER_LENGTH * 2];
            int written = snprintf(pattern, sizeof(pattern), "\\|[[:space:]]*%s[[:space:]]*\\|", filter_level_lower);

            if (written < 0 || (size_t)written >= sizeof(pattern))
                return 0;

            regex_t regex;
            if (regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE) != 0)
                return 0;

            if (regexec(&regex, line, 0, NULL, 0) == 0)
            {
                regfree(&regex);
                goto date_check;
            }
            regfree(&regex);
        }
        return 0;
    }

date_check:
    if (start_date_str || end_date_str)
    {
        if (!current_format || !current_format->date_regex)
            return 0;

        char extracted[DATE_BUFFER_SIZE];
        if (!extract_date_from_line(line, current_format->date_regex, extracted, sizeof(extracted)))
            return 0;

        char *s = extracted;
        while (*s && !isalnum((unsigned char)*s))
            s++;
        char *e = extracted + strlen(extracted) - 1;
        while (e > s && !isalnum((unsigned char)*e))
        {
            *e = '\0';
            e--;
        }

        time_t line_ts = parse_date_to_time_t(s, current_format->date_strptime);
        if (line_ts == (time_t)-1)
            return 0;

        if (start_date_str)
        {
            time_t st = parse_date_to_time_t(start_date_str, current_format->date_strptime);
            if (st == (time_t)-1 || line_ts < st)
                return 0;
        }
        if (end_date_str)
        {
            time_t et = parse_date_to_time_t(end_date_str, current_format->date_strptime);
            if (et == (time_t)-1 || line_ts > et)
                return 0;
        }
    }

    return 1;
}
