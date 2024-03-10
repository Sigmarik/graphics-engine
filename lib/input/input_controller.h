/**
 * @file input_controller.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Input controller singleton class
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LIB_INPUT_INPUT_CONTROLLER_H
#define __LIB_INPUT_INPUT_CONTROLLER_H

#include <stddef.h>

#include <map>
#include <string>
#include <vector>

#include "action.h"
#include "graphics/libs.h"
#include "key_names.hpp"
#include "text_listener.h"

enum class CursorMode {
    Desktop,
    Camera,
};

struct InputController {
    static void init(GLFWwindow* window);

    static void key_callback(GLFWwindow* window, int key, int scancode,
                             int action, int mods);

    static void character_callback(GLFWwindow* window, unsigned int codepoint);

    static void cursor_position_callback(GLFWwindow* window, double xpos,
                                         double ypos);

    static void mouse_button_callback(GLFWwindow* window, int button,
                                      int action, int mods);

    static void set_active_window(GLFWwindow* window);
    static GLFWwindow* get_active_window();

    /********************************************************
    @brief Set the cursor mode to 'mode' in active window
    @param mode new cursor mode
    ********************************************************/
    static void set_cursor_mode(CursorMode mode);

    static void set_cursor_mode(GLFWwindow* window, CursorMode mode);

    static const InputAction* get_action(const char* name);

    static double get_mouse_x() { return mouse_x; }
    static double get_mouse_y() { return mouse_y; }

    static double get_mouse_delta_x() { return mouse_delta_x; }
    static double get_mouse_delta_y() { return mouse_delta_y; }

    static void poll_events();

    static void register_text_listener(TextListener& listener);

   private:
    static std::map<std::string, const InputAction&> basic_inputs_;

    static std::vector<TextListener> text_listeners_;

    static InputAction keyboard_actions_[KEYBOARD_CODE_COUNT];
    static InputAction mouse_actions_[MOUSE_CODE_COUNT];

    static InputController instance_;

    static double mouse_x, mouse_y;
    static double mouse_delta_x, mouse_delta_y;

    static GLFWwindow *active_window_;
};

#endif /* __LIB_INPUT_INPUT_CONTROLLER_H */
