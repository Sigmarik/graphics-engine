/**
 * @file main.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Main module of the program
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "io/main_io.h"
#include "logger/debug.h"
#include "logger/logger.h"
#include "utils/main_utils.h"

#define MAIN

#include "config.h"

int main(const int argc, char** argv) {
    atexit(log_end_program);

    Options options;

    log_init("program_log.html", LOG_THRESHOLD, &errno);
    print_label();

    log_printf(STATUS_REPORTS, "status", "Initializing\n");

    if (argp_parse(&ARG_P, argc, argv, 0, 0, &options) != 0) {
        return EXIT_FAILURE;
    }

    // PROGRAM BODY //

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
