#ifndef LOG_FORMAT_H
#define LOG_FORMAT_H

// Структура для определения формата лога
typedef struct {
    const char *name;        // Название формата лога
    const char *date_regex;  // Регулярное выражение для даты
    const char *level_regex; // Базовое регулярное выражение для уровня (будет дополнено уровнем)
    const char *message_regex; // Регулярное выражение для сообщения
    int level_position;      // Позиция, где искать уровень (группа в regex)
    int date_position;       // Позиция даты
    int message_position;    // Позиция сообщения
    int strict_format;       // Флаг строгой проверки формата (0 - выводить все строки, 1 - только соответствующие формату)
} LogFormat;

// Глобальный указатель на выбранный формат
extern LogFormat *current_format;

// Инициализация форматов логов
void init_log_formats();

// Выбор формата логов по имени
int select_log_format(const char *format_name);

// Получение регулярного выражения для конкретного уровня логирования
char *get_level_pattern(const char *level);

// Освобождение ресурсов форматов логов
void cleanup_log_formats();

#endif // LOG_FORMAT_H 
