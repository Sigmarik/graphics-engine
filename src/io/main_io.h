/**
 * @file main_io.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief IO system for the main module
 * @version 0.1
 * @date 2023-07-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MAIN_IO_H
#define MAIN_IO_H

#include <argp.h>

#include "src/config.h"

static const char ARGS_DOC[] = "INPUT_FILE_NAME";

enum OptCodeKey {
    _OPT_CUSTOM_KEYS_SHIFT = 500,
    OPT_OWL,
};

static const argp_option PARSER_OPTIONS[] = {
    {"owl", OPT_OWL, NULL, 0, "Lets the owls out"},
    {"output", 'o', "OUTPUT_FILE", 0, "Output file path"},
    {}  // <-- NULL-terminator
};

struct Options {
    Options();
    Options(const Options& options);
    ~Options();

    Options& operator=(const Options& options);

    const char* get_output_name();
    const char* get_input_name();

    void set_output_name(const char* new_name);
    void set_input_name(const char* new_name);

   private:
    const char* output_name_;
    const char* input_name_;
};

/**
 * @brief Parse command line argument
 *
 * @param key argument key
 * @param arg argument
 * @param state parser state
 * @return 0 if parsing was successful, error code otherwise
 */
error_t parse(int key, char* arg, struct argp_state* state);

static const argp ARG_P = {.options = PARSER_OPTIONS,
                           .parser = parse,
                           .args_doc = ARGS_DOC,
                           .doc = PROGRAM_DESCRIPTION};

#endif
