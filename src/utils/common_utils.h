/**
 * @file common_utils.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Utility functions present in all main modules of the project.
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdlib.h>

/**
 * @brief Memory mapping result.
 *
 * @param ptr pointer to the mapped area
 * @param fd file descriptor
 */
struct MmapResult {
    void* ptr = NULL;
    int fd = 0;
    size_t size = 0;
};

/**
 * @brief Memory map file and return pointer to the mapped area.
 *
 * @param name file name
 * @param oflags open flags
 * @param prot mmap protection flags
 * @return MapResult
 */
MmapResult map_file(const char* name, int oflags, int prot, int privacy);

#endif
