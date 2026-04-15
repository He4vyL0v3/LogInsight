/**
 * @file file_size.c
 * @brief File size reporting functionality.
 *
 * Provides functions to retrieve and display file size information
 * in human-readable format.
 */

#include <stdio.h>
#include <sys/stat.h>

/**
 * @brief Prints the size of a file in human-readable format.
 *
 * Displays the file size with appropriate units (B, KB, MB, GB, TB)
 * using a formatted box style.
 *
 * @param file_name Path to the file to measure.
 */
void print_file_size(const char *file_name)
{
    struct stat st;
    if (stat(file_name, &st) == 0)
    {
        long long size = st.st_size;
        const char *units[] = {"B", "KB", "MB", "GB", "TB"};
        int unit_index = 0;

        while (size >= 1024 && unit_index < (int)(sizeof(units) / sizeof(units[0])) - 1)
        {
            size /= 1024;
            unit_index++;
        }

        printf("\033[1;31m\n┌─────────────────────────────⬤ \n");
        printf("│ ⬤  File size: %.2f %s\n", (double)size, units[unit_index]);
        printf("└─────────────────────────────⬤ \n\033[0m");
    }
    else
    {
        perror("\033[1;31m\n\nCould not get file size\033[0m");
    }
}
