/**
 * @file debug.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Module with debugging information.
 * @version 0.1
 * @date 2022-08-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DEBUGGING_H
#define DEBUGGING_H

#define SILENCE_UNUSED(var) ((void)(var))

#include <assert.h>
#include <errno.h>

#include "logger.h"

typedef int* const err_anchor_t;
#define ERROR_MARKER err_anchor_t err_code = NULL

/**
 * @brief Print description to log file if equation fails.
 *
 * @param condition value to use as an inverse trigger for assert
 * @param tag prefix to print before failure message
 * @param importance
 * @param action sequence to run of failure
 * @param errcode variable to write errtype in
 * @param errtype error code
 */
#define _LOG_FAIL_CHECK_(condition, tag, importance, action, errcode, errtype) \
    do {                                                                       \
        if (!(condition)) {                                                    \
            int* errptr = errcode;                                             \
            if (errptr) *(errptr) = (errtype);                                 \
            log_printf(importance, tag,                                        \
                       "Equation `%s` in file %s at line %d failed.\n",        \
                       #condition, __FILE__, __LINE__);                        \
            action;                                                            \
        }                                                                      \
    } while (0)

/**
 * @brief Print errno value and its description and close log file.
 */
void log_end_program();

/**
 * @brief Check if pointer is readable.
 *
 * @param ptr pointer to check
 * @return true if pointer is valid,
 * @return false otherwise
 */
bool check_ptr(const void* ptr);

/**
 * @brief End program if errno is not zero.
 *
 */
#define _ABORT_ON_ERRNO_()                                       \
    _LOG_FAIL_CHECK_(!errno, "FATAL ERROR", ABSOLUTE_IMPORTANCE, \
                     exit(EXIT_FAILURE);                         \
                     , NULL, 0);

/**
 * @brief Calculate hash value of the buffer.
 *
 * @param start pointer to the start of the buffer
 * @param end pointer to the end of the buffer
 * @return hash_t
 */
unsigned long long get_simple_hash(const void* start, const void* end);

#endif