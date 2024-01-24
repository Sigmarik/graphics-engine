/**
 * @file action.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Abstract user input event
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LIB_INPUT_ACTION_H
#define __LIB_INPUT_ACTION_H

#include <stdlib.h>

enum class InputDevice {
    Mouse,
    Keyboard,
};

struct InputAction {
    InputAction() = default;
    InputAction(InputDevice device, int code) : device_(device), code_(code) {}

    bool poll_pushed() const { return just_pushed_; }
    bool poll_released() const { return just_released_; }

    void reset_states();

    bool get_pushed() const { return active_; }

    void update(int basic_action);

    InputDevice get_device() const { return device_; }
    int get_code() const { return code_; }

   private:
    bool active_ = false;

    bool just_pushed_ = false;
    bool just_released_ = false;

    InputDevice device_ = InputDevice::Keyboard;
    int code_ = 0;
};

#endif /* __LIB_INPUT_ACTION_H */
