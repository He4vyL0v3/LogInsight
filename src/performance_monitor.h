/**
 * @file performance_monitor.h
 * @brief Interface for performance monitoring.
 */

#ifndef PERFORMANCE_MONITOR_H
#define PERFORMANCE_MONITOR_H

#include <sys/resource.h>
#include <sys/time.h>

/**
 * @brief Starts performance monitoring.
 *
 * Captures initial resource usage for later comparison.
 */
void start_monitoring(void);

/**
 * @brief Stops monitoring and prints statistics.
 */
void stop_monitoring(void);

#endif // PERFORMANCE_MONITOR_H
