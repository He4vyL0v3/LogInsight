#include "log_color.h"
#include "log_format.h"
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define RED "\033[0;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define WHITE "\033[1;37m"
#define ORANGE "\033[38;5;214m"
#define NC "\033[0m"

void colorize_log(const char *line)
{
    regex_t regex;

    const char *level_names[] = {"CRITICAL", "WARNING", "INFO", "DEBUG", "ERROR", "UNKNOWN", "TRACE", "FATAL"};
    const char *colors[] = {RED, YELLOW, GREEN, BLUE, RED, WHITE, PURPLE, ORANGE};

    for (int i = 0; i < sizeof(level_names) / sizeof(level_names[0]); i++)
    {
        char *pattern = get_level_pattern(level_names[i]);
        int reti;
        reti = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
        if (reti)
        {
            fprintf(stderr, "Could not compile regex\n");
            return;
        }

        reti = regexec(&regex, line, 0, NULL, 0);
        if (reti == 0)
        {
            printf("%s%s%s\n", colors[i], line, NC);
            regfree(&regex);
            return;
        }
        regfree(&regex);
    }

    printf("%s\n", line);
}
