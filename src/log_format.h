#ifndef LOG_FORMAT_H
#define LOG_FORMAT_H

typedef struct
{
    const char *name;
    const char *date_regex;
    const char *level_regex;
    const char *message_regex;
    int level_position;
    int date_position;
    int message_position;
    int strict_format;
} LogFormat;

extern LogFormat *current_format;

void init_log_formats();
int select_log_format(const char *format_name);
char *get_level_pattern(const char *level);
void cleanup_log_formats();

#endif // LOG_FORMAT_H
