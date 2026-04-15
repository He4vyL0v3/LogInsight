/**
 * @file log_levels.h
 * @brief Centralized log level definitions and color management.
 *
 * This file provides a single source of truth for log level types,
 * their names, colors, and regex patterns used throughout the application.
 * Previously, this data was duplicated across multiple files.
 */

#ifndef LOG_LEVELS_H
#define LOG_LEVELS_H

#include <regex.h>
#include <stdbool.h>

/**
 * @brief Enumeration of all supported log levels.
 *
 * These values are used to index into the level-related arrays,
 * ensuring type safety and compile-time checking.
 */
typedef enum
{
    LOG_LEVEL_CRITICAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_FATAL = 2,
    LOG_LEVEL_WARNING = 3,
    LOG_LEVEL_INFO = 4,
    LOG_LEVEL_DEBUG = 5,
    LOG_LEVEL_TRACE = 6,
    LOG_LEVEL_UNKNOWN = 7,
    LOG_LEVEL_COUNT = 8
} LogLevelType;

/**
 * @brief Structure holding a complete description of a log level.
 *
 * Contains all information needed for a log level: its display name,
 * the ANSI color code for terminal output, and the regex pattern
 * for detecting this level in log lines.
 */
typedef struct
{
    const char *label;   /**< Human-readable name (e.g., "ERROR", "WARNING") */
    const char *color;   /**< ANSI escape code for terminal color */
    const char *pattern; /**< Regex pattern for detecting this level */
} LogLevelInfo;

/**
 * @brief Array containing all log level definitions.
 *
 * This is the single source of truth for log level data.
 * Order must match LogLevelType enum values.
 */
static const LogLevelInfo LOG_LEVELS[LOG_LEVEL_COUNT] = {
    [LOG_LEVEL_CRITICAL] = {"CRITICAL", "\033[1;31m", "\\|\\s*CRITICAL\\s*\\|"},
    [LOG_LEVEL_ERROR] = {"ERROR", "\033[1;31m", "\\|\\s*ERROR\\s*\\|"},
    [LOG_LEVEL_FATAL] = {"FATAL", "\033[38;5;214m", "\\|\\s*FATAL\\s*\\|"},
    [LOG_LEVEL_WARNING] = {"WARNING", "\033[1;33m", "\\|\\s*WARNING\\s*\\|"},
    [LOG_LEVEL_INFO] = {"INFO", "\033[1;32m", "\\|\\s*INFO\\s*\\|"},
    [LOG_LEVEL_DEBUG] = {"DEBUG", "\033[1;32m", "\\|\\s*DEBUG\\s*\\|"},
    [LOG_LEVEL_TRACE] = {"TRACE", "\033[1;34m", "\\|\\s*TRACE\\s*\\|"},
    [LOG_LEVEL_UNKNOWN] = {"UNKNOWN", "\033[1;37m", "\\|\\s*UNKNOWN\\s*\\|"}};

/**
 * @brief Global array of pre-compiled regex patterns for log level detection.
 *
 * These patterns are compiled once at startup and reused for matching.
 * Access via log_level_get_pattern() and log_level_compile_all().
 */
extern regex_t LOG_LEVEL_REGEXES[LOG_LEVEL_COUNT];

/**
 * @brief ANSI escape code to reset terminal color.
 */
#define LOG_COLOR_RESET "\033[0m"

/**
 * @brief Compiles all regex patterns for log level detection.
 *
 * Should be called once at application startup before any log processing.
 * If compilation fails, prints error message and terminates the program.
 */
void log_level_compile_all(void);

/**
 * @brief Frees all compiled regex patterns.
 *
 * Should be called once at application shutdown to prevent resource leaks.
 */
void log_level_free_all(void);

/**
 * @brief Returns the log level index matching the given level name.
 *
 * @param level_name Case-insensitive level name to search for.
 * @return LogLevelType enum value, or LOG_LEVEL_UNKNOWN if not found.
 */
LogLevelType log_level_from_string(const char *level_name);

/**
 * @brief Checks if a log line contains the specified level marker.
 *
 * Uses pre-compiled regex patterns for efficient matching.
 *
 * @param line The log line to check.
 * @param level The log level to search for.
 * @return true if the line contains the level marker, false otherwise.
 */
bool log_level_matches(const char *line, LogLevelType level);

/**
 * @brief Returns the LogLevelInfo structure for a given level type.
 *
 * @param level The log level type.
 * @return Pointer to the corresponding LogLevelInfo structure.
 */
static inline const LogLevelInfo *log_level_get_info(LogLevelType level)
{
    return &LOG_LEVELS[level];
}

#endif // LOG_LEVELS_H
