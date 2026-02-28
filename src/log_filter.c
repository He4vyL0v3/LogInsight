#define _XOPEN_SOURCE 700
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "log_format.h"

#define MAX_PATTERN_LENGTH 256
#define MAX_FILTER_LENGTH 256
#define DATE_BUFFER_SIZE 64

static int extract_date_from_line(const char *line, const char *date_regex,
                                  char *date_buf, size_t buf_size)
{
    regex_t regex;
    if (regcomp(&regex, date_regex, REG_EXTENDED) != 0)
        return 0;

    regmatch_t match[1];
    if (regexec(&regex, line, 1, match, 0) == 0)
    {
        size_t len = match[0].rm_eo - match[0].rm_so;
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

static time_t parse_date_to_time_t(const char *date_str, const char *fmt)
{
    struct tm tm = {0};
    if (strptime(date_str, fmt, &tm) == NULL)
        return (time_t)-1;
    return mktime(&tm);
}

int should_print_log(const char *line, const char *const filter_levels[],
                     int filter_count, const char *start_date_str,
                     const char *end_date_str)
{
    if (filter_count > 0)
    {
        for (int j = 0; j < filter_count; j++)
        {
            char filter_level_lower[MAX_FILTER_LENGTH];
            size_t len = strlen(filter_levels[j]);

            for (size_t i = 0; i < len; i++)
                filter_level_lower[i] = tolower((unsigned char)filter_levels[j][i]);
            filter_level_lower[len] = '\0';

            char pattern[MAX_PATTERN_LENGTH];
            int written = snprintf(pattern, sizeof(pattern),
                                  "\\|[[:space:]]*%s[[:space:]]*\\|",
                                  filter_level_lower);

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
        if (!extract_date_from_line(line, current_format->date_regex,
                                    extracted, sizeof(extracted)))
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
            time_t st = parse_date_to_time_t(start_date_str,
                                            current_format->date_strptime);
            if (st == (time_t)-1 || line_ts < st)
                return 0;
        }
        if (end_date_str)
        {
            time_t et = parse_date_to_time_t(end_date_str,
                                            current_format->date_strptime);
            if (et == (time_t)-1 || line_ts > et)
                return 0;
        }
    }

    return 1;
}
