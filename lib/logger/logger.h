/**
 * @file logger.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Module for creating program logs.
 * @version 0.1
 * @date 2022-08-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

enum IMPORTANCES {
    DATA_UPDATES = 0,
    STATUS_REPORTS = 1,
    AUTOMATIC_CORRECTIONS = 2,
    WARNINGS = 3,
    ERROR_REPORTS = 5,
    TERMINATE_REPORTS = 6,
    ABSOLUTE_IMPORTANCE = 1000,
};

#include <stdarg.h>

#ifndef NDEBUG

#ifndef NLOG_PRINT_LINE
/**
 * @brief Print message to logs followed by call information.
 *
 * @param importance message importance (more important = higher value)
 * @param tag prefix of the message
 * @param __VA_ARGS__ arguments as if they were in printf()
 */
#define log_printf(importance, tag, ...)                                  \
    do {                                                                  \
        _log_printf(importance, tag, " ----- Called from %s:%d. -----\n", \
                    __FILE__, __LINE__);                                  \
        _log_printf(importance, tag, __VA_ARGS__);                        \
    } while (0)
#else
/**
 * @brief Print message to logs.
 *
 * @param importance message importance (more important = higher value)
 * @param tag prefix of the message
 * @param __VA_ARGS__ arguments as if they were in printf()
 */
#define log_printf(importance, tag, ...)           \
    do {                                           \
        _log_printf(importance, tag, __VA_ARGS__); \
    } while (0)
#endif

#else
/**
 * @brief (DISABLED) Print message to logs.
 *
 * @param importance message importance (more important = higher value)
 * @param tag prefix of the message
 * @param __VA_ARGS__ arguments as if they were in printf()
 */
#define log_printf(importance, tag, ...) \
    do {                                 \
    } while (0)

#endif

#define log_dup(importance, tag, ...)             \
    do {                                          \
        printf(__VA_ARGS__);                      \
        log_printf(importance, tag, __VA_ARGS__); \
    } while (0)

/**
 * @brief Print line to logs with automatic prefix.
 *
 * @param importance importance of the message
 * @param tag message tag
 * @param format format string for printf()
 * @param ... arguments for printf()
 */
void _log_printf(const unsigned int importance, const char* tag,
                 const char* format, ...) __attribute__((format(printf, 3, 4)));

/**
 * @brief Close opened log file.
 *
 * @param error_code (optional) variable to put function execution code in
 */
void log_close(int* error_code = NULL);

/**
 * @brief Set logging threshold
 *
 * @param[in] threshold
 */
void set_logging_threshold(unsigned int threshold);

#endif