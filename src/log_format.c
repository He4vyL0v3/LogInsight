/**
 * @file log_format.c
 * @brief Log format definitions and selection.
 *
 * Manages supported log formats (basic, Apache, syslog, JSON, custom)
 * and provides format-specific regex patterns for date and level extraction.
 */

#include "log_format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FORMATS 10

static LogFormat log_formats[MAX_FORMATS];
static int format_count = 0;
LogFormat *current_format = NULL;

/**
 * @brief Initializes all supported log formats.
 *
 * Registers basic, Apache, syslog, JSON, and custom formats.
 * Sets the current format to "basic" by default.
 */
void init_log_formats(void)
{
    LogFormat basic_format = {.name = "basic",
                              .date_regex = "\\([0-9]{4}-[0-9]{2}-[0-9]{2}[[:space:]][0-9]{2}:[0-9]{2}:[0-9]{2}\\)",
                              .level_regex = "\\|[[:space:]]*%s[[:space:]]*\\|",
                              .message_regex = ".*",
                              .level_position = 1,
                              .date_position = 0,
                              .message_position = 2,
                              .strict_format = 0,
                              .date_strptime = "%Y-%m-%d %H:%M:%S"};

    LogFormat apache_format = {
        .name = "apache",
        .date_regex = "\\(\\[[0-9]{2}/[A-Za-z]{3}/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}[[:space:]][+-][0-9]{4}\\]\\)",
        .level_regex = "\"[A-Z]+\"",
        .message_regex = ".*",
        .level_position = 1,
        .date_position = 0,
        .message_position = 2,
        .strict_format = 0,
        .date_strptime = "%d/%b/%Y:%H:%M:%S %z"};

    LogFormat syslog_format = {.name = "syslog",
                               .date_regex =
                                   "\\(\\[A-Za-z]{3}[[:space:]]+[0-9]+[[:space:]][0-9]{2}:[0-9]{2}:[0-9]{2}\\)",
                               .level_regex = "[[:space:]]%s[[:space:]]",
                               .message_regex = ".*",
                               .level_position = 1,
                               .date_position = 0,
                               .message_position = 2,
                               .strict_format = 0,
                               .date_strptime = "%b %d %H:%M:%S"};

    LogFormat json_format = {.name = "json",
                             .date_regex = "\\(\"timestamp\"[[:space:]]*:[[:space:]]*\"[^\"]+\"\\)",
                             .level_regex = "\"level\"[[:space:]]*:[[:space:]]*\"%s\"",
                             .message_regex = "\"message\"[[:space:]]*:[[:space:]]*\"[^\"]+\"",
                             .level_position = 1,
                             .date_position = 0,
                             .message_position = 2,
                             .strict_format = 0,
                             .date_strptime = "%Y-%m-%dT%H:%M:%SZ"};

    LogFormat custom_format = {.name = "custom",
                               .date_regex =
                                   "([0-9]{4}-[0-9]{2}-[0-9]{2}[[:space:]][0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})",
                               .level_regex = "\\|[[:space:]]*%s[[:space:]]*\\|",
                               .message_regex = ".*",
                               .level_position = 1,
                               .date_position = 0,
                               .message_position = 2,
                               .strict_format = 0,
                               .date_strptime = "%Y-%m-%d %H:%M:%S"};

    log_formats[format_count++] = basic_format;
    log_formats[format_count++] = apache_format;
    log_formats[format_count++] = syslog_format;
    log_formats[format_count++] = json_format;
    log_formats[format_count++] = custom_format;

    current_format = &log_formats[0];
}

/**
 * @brief Selects the active log format by name.
 *
 * @param format_name Name of the format to activate.
 * @return 1 on success, 0 if format not found.
 */
int select_log_format(const char *format_name)
{
    for (int i = 0; i < format_count; i++)
    {
        if (strcmp(log_formats[i].name, format_name) == 0)
        {
            current_format = &log_formats[i];
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Generates a regex pattern for a specific level.
 *
 * @param level The level name to substitute.
 * @return Static buffer with the formatted pattern, or NULL on error.
 */
char *get_level_pattern(const char *level)
{
    if (!current_format)
        return NULL;

    static char pattern[512];
    snprintf(pattern, sizeof(pattern), current_format->level_regex, level);
    return pattern;
}

/**
 * @brief Cleans up format resources.
 *
 * Currently a placeholder for future cleanup needs.
 */
void cleanup_log_formats(void)
{
}
