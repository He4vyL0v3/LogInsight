/**
 * @file performance_monitor.c
 * @brief Performance monitoring and reporting.
 *
 * Tracks CPU time, elapsed time, and memory usage of the application,
 * providing performance statistics at the end of execution.
 */

#include "performance_monitor.h"
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

static struct timeval start_time;
static struct rusage start_usage;

/**
 * @brief Starts performance monitoring.
 *
 * Captures the initial time and resource usage to calculate
 * performance metrics later. Should be called once at startup.
 */
void start_monitoring(void)
{
    gettimeofday(&start_time, NULL);
    getrusage(RUSAGE_SELF, &start_usage);
}

/**
 * @brief Stops monitoring and prints performance statistics.
 *
 * Calculates elapsed time, CPU time, and peak memory usage
 * since start_monitoring() was called, then prints formatted output.
 */
void stop_monitoring(void)
{
    struct timeval end_time;
    struct rusage end_usage;

    gettimeofday(&end_time, NULL);
    getrusage(RUSAGE_SELF, &end_usage);

    double elapsed = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) * 1e-6;

    long memory_used = end_usage.ru_maxrss;

    double user_cpu = end_usage.ru_utime.tv_sec + end_usage.ru_utime.tv_usec * 1e-6;
    double sys_cpu = end_usage.ru_stime.tv_sec + end_usage.ru_stime.tv_usec * 1e-6;
    double cpu_time = user_cpu + sys_cpu;

    printf("\033[1;32m┌─────────────────────────────⬤ \n");

    if (elapsed < 1.0)
        printf("│ ⬤  Elapsed time: %.3f milliseconds\n", elapsed * 1000);
    else if (elapsed < 60.0)
        printf("│ ⬤  Elapsed time: %.3f seconds\n", elapsed);
    else if (elapsed < 3600.0)
        printf("│ ⬤  Elapsed time: %.3f minutes\n", elapsed / 60.0);
    else if (elapsed < 86400.0)
        printf("│ ⬤  Elapsed time: %.3f hours\n", elapsed / 3600.0);
    else
        printf("│ ⬤  Elapsed time: %.3f days\n", elapsed / 86400.0);

    if (memory_used < 1024)
        printf("│ ⬤  Memory used: %ld bytes\n", memory_used);
    else if (memory_used < 1024 * 1024)
        printf("│ ⬤  Memory used: %.2f KB\n", memory_used / 1024.0);
    else
        printf("│ ⬤  Memory used: %.2f MB\n", memory_used / (1024.0 * 1024.0));

    if (cpu_time < 1.0)
        printf("│ ⬤  CPU time: %.3f milliseconds\n", cpu_time * 1000);
    else
        printf("│ ⬤  CPU time: %.3f seconds\n", cpu_time);

    printf("└─────────────────────────────⬤ \n");
}
