/**
 * @file log_levels.c
 * @brief Implementation of centralized log level management.
 *
 * Provides pre-compiled regex patterns for efficient log level detection
 * across all modules. This eliminates the previous issue where regex patterns
 * were recompiled on every log line processed.
 */

#include "log_levels.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

regex_t LOG_LEVEL_REGEXES[LOG_LEVEL_COUNT];

void log_level_compile_all(void)
{
    for (int i = 0; i < LOG_LEVEL_COUNT; i++)
    {
        int result = regcomp(&LOG_LEVEL_REGEXES[i], LOG_LEVELS[i].pattern, REG_EXTENDED | REG_ICASE);
        if (result != 0)
        {
            char error_msg[256];
            regerror(result, &LOG_LEVEL_REGEXES[i], error_msg, sizeof(error_msg));
            fprintf(stderr, "Failed to compile regex for %s: %s\n", LOG_LEVELS[i].label, error_msg);
            exit(EXIT_FAILURE);
        }
    }
}

void log_level_free_all(void)
{
    for (int i = 0; i < LOG_LEVEL_COUNT; i++)
    {
        regfree(&LOG_LEVEL_REGEXES[i]);
    }
}

LogLevelType log_level_from_string(const char *level_name)
{
    if (level_name == NULL)
    {
        return LOG_LEVEL_UNKNOWN;
    }

    size_t len = strlen(level_name);
    if (len == 0)
    {
        return LOG_LEVEL_UNKNOWN;
    }

    char upper[32];
    size_t copy_len = len < sizeof(upper) - 1 ? len : sizeof(upper) - 1;
    for (size_t i = 0; i < copy_len; i++)
    {
        upper[i] = (char)toupper((unsigned char)level_name[i]);
    }
    upper[copy_len] = '\0';

    for (int i = 0; i < LOG_LEVEL_COUNT; i++)
    {
        if (strcmp(upper, LOG_LEVELS[i].label) == 0)
        {
            return (LogLevelType)i;
        }
    }

    return LOG_LEVEL_UNKNOWN;
}

bool log_level_matches(const char *line, LogLevelType level)
{
    if (line == NULL || level < 0 || level >= LOG_LEVEL_COUNT)
    {
        return false;
    }
    return regexec(&LOG_LEVEL_REGEXES[level], line, 0, NULL, 0) == 0;
}
