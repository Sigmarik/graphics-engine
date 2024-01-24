/**
 * @file input_axis.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Input axis
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LIB_INPUT_INPUT_AXIS_H
#define __LIB_INPUT_INPUT_AXIS_H

#include <lib/graphics/libs.h>

#include <vector>

struct InputController;

struct InputAxis {
    double get_value() const { return value_; }
    void set_value(double value) { value_ = value; }

    void suggest_value(double value);
    void drop_value(double value);

    void handle(GLFW);

   private:
    double value_ = 0.0;

    // Weight of the current axis value, f.e. the number of effective keys that
    // are pressed at the moment
    unsigned weight_ = 0;
};

#endif /* __LIB_INPUT_INPUT_AXIS_H */
