/**
 * @file main_utils.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Utility functions used in the main module
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include "common_utils.h"
#include "graphics/libs.h"

/**
 * @brief Print the label of the program
 *
 */
void print_label();

/**
 * @brief Create window, initialize GLFW & GLAD
 *
 * @return GLFWwindow* (nullptr on fail)
 */
GLFWwindow* create_window();

#endif
