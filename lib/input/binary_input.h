/**
 * @file binary_input.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Abstract binary input class, such as "use wand" input event.
 * @version 0.1
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LIB_INPUT_BINARY_INPUT_H
#define __LIB_INPUT_BINARY_INPUT_H

#include <vector>

#include "action.h"

struct BinaryInput {
    void add_activator(const InputAction& action);

    bool poll_pushed() const;
    bool poll_released() const;

    bool get_pushed() const;

   private:
    std::vector<const InputAction*> activators_ = {};
};

#endif /* __LIB_INPUT_BINARY_INPUT_H */
