#include "log_format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FORMATS 10
#define MAX_PATTERN_LENGTH 512

static LogFormat log_formats[MAX_FORMATS];
static int format_count = 0;
LogFormat *current_format = NULL;

void init_log_formats() {
    // Стандартный формат (текущий)
    LogFormat basic_format = {
        .name = "basic",
        .date_regex = "\\d{4}-\\d{2}-\\d{2}\\s\\d{2}:\\d{2}:\\d{2}",
        .level_regex = "\\|\\s*%s\\s*\\|", // %s будет заменено на уровень логирования
        .message_regex = ".*",
        .level_position = 1,
        .date_position = 0,
        .message_position = 2,
        .strict_format = 0    // По умолчанию не строгая проверка
    };
    
    // Apache Access Log
    LogFormat apache_format = {
        .name = "apache",
        .date_regex = "\\[\\d{2}/\\w{3}/\\d{4}:\\d{2}:\\d{2}:\\d{2}\\s[+-]\\d{4}\\]",
        .level_regex = "\"[A-Z]+", // HTTP методы как уровни (GET, POST и т.д.)
        .message_regex = ".*",
        .level_position = 1,
        .date_position = 0,
        .message_position = 2,
        .strict_format = 0
    };
    
    // Syslog формат
    LogFormat syslog_format = {
        .name = "syslog",
        .date_regex = "\\w{3}\\s+\\d+\\s\\d{2}:\\d{2}:\\d{2}",
        .level_regex = "\\s%s\\s", // %s будет заменено на уровень логирования
        .message_regex = ".*",
        .level_position = 1,
        .date_position = 0,
        .message_position = 2,
        .strict_format = 0
    };
    
    // JSON формат
    LogFormat json_format = {
        .name = "json",
        .date_regex = "\"timestamp\"\\s*:\\s*\"[^\"]+\"",
        .level_regex = "\"level\"\\s*:\\s*\"%s\"", // %s будет заменено на уровень
        .message_regex = "\"message\"\\s*:\\s*\"[^\"]+\"",
        .level_position = 1,
        .date_position = 0,
        .message_position = 2,
        .strict_format = 0
    };

    // Добавление форматов в массив
    log_formats[format_count++] = basic_format;
    log_formats[format_count++] = apache_format;
    log_formats[format_count++] = syslog_format;
    log_formats[format_count++] = json_format;
    
    // По умолчанию используем базовый формат
    current_format = &log_formats[0];
}

int select_log_format(const char *format_name) {
    for (int i = 0; i < format_count; i++) {
        if (strcmp(log_formats[i].name, format_name) == 0) {
            current_format = &log_formats[i];
            return 1;
        }
    }
    return 0; // Формат не найден
}

char *get_level_pattern(const char *level) {
    if (!current_format) return NULL;
    
    static char pattern[MAX_PATTERN_LENGTH];
    snprintf(pattern, sizeof(pattern), current_format->level_regex, level);
    return pattern;
}

void cleanup_log_formats() {
    // Освобождение ресурсов, если необходимо
} 
