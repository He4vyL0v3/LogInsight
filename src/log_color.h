/**
 * @file log_color.h
 * @brief Interface for colorized log output.
 */

#ifndef LOG_COLOR_H
#define LOG_COLOR_H

/**
 * @brief Colorizes and prints a log line based on its level.
 *
 * Searches for level markers in the line and applies the corresponding
 * ANSI color before printing.
 *
 * @param line The log line to colorize and print.
 */
void colorize_log(const char *line);

#endif // LOG_COLOR_H
