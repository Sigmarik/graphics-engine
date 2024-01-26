#include "input_controller.h"

#include <string.h>

#include "logger/logger.h"

std::map<std::string, const InputAction&> InputController::basic_inputs_ = {};

std::vector<TextListener> InputController::text_listeners_ = {};

InputAction InputController::keyboard_actions_[KEYBOARD_CODE_COUNT];
InputAction InputController::mouse_actions_[MOUSE_CODE_COUNT];

InputController InputController::instance_ = InputController();

double InputController::mouse_x = 0;
double InputController::mouse_y = 0;
double InputController::mouse_delta_x = 0;
double InputController::mouse_delta_y = 0;

static const size_t INPUT_CODE_NAME_LENGTH = 128;

void InputController::init(GLFWwindow* window) {
    log_printf(STATUS_REPORTS, "status", "Initializing input controller\n");

    log_printf(STATUS_REPORTS, "status", "Keyboard bindings:\n");

    for (size_t id = 0; id < KEYBOARD_CODE_COUNT; ++id) {
        keyboard_actions_[id] = InputAction(InputDevice::Keyboard, (int)id);

        const char* key_name = KEY_NAMES[id];

        if (key_name == nullptr) continue;

        _log_printf(STATUS_REPORTS, "status", "\t%03lu: %s\n", id,
                    (key_name == nullptr) ? "---" : key_name);

        basic_inputs_.insert({key_name, keyboard_actions_[id]});
    }

    log_printf(STATUS_REPORTS, "status", "Mouse button bindings:\n");

    for (size_t id = 0; id < MOUSE_CODE_COUNT; ++id) {
        mouse_actions_[id] = InputAction(InputDevice::Mouse, (int)id);

        const char* btn_name = BUTTON_NAMES[id];
        _log_printf(STATUS_REPORTS, "status", "\t%03lu: %s\n", id,
                    (btn_name == nullptr) ? "---" : btn_name);

        if (btn_name == nullptr) continue;

        basic_inputs_.insert({btn_name, mouse_actions_[id]});
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void InputController::key_callback(GLFWwindow* window, int key, int scancode,
                                   int action, int mods) {
    if (key >= 0) {
        keyboard_actions_[key].update(action);
    } else {
        log_printf(WARNINGS, "warning",
                   "Unrecognized negative keyboard key %d\n", key);
    }
}

void InputController::character_callback(GLFWwindow* window,
                                         unsigned int codepoint) {
    for (TextListener& listener : text_listeners_) {
        listener.character_callback(codepoint);
    }
}

void InputController::cursor_position_callback(GLFWwindow* window, double xpos,
                                               double ypos) {
    mouse_delta_x = xpos - mouse_x;
    mouse_delta_y = ypos - mouse_y;

    mouse_x = xpos;
    mouse_y = ypos;
}

void InputController::mouse_button_callback(GLFWwindow* window, int button,
                                            int action, int mods) {
    if (button >= 0) {
        mouse_actions_[button].update(action);
    } else {
        log_printf(WARNINGS, "warning",
                   "Unrecognized negative mouse button %d\n", button);
    }
}

void InputController::set_cursor_mode(GLFWwindow* window, CursorMode mode) {
    switch (mode) {
        case CursorMode::Camera: {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        } break;
        case CursorMode::Desktop: {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        } break;
        default:
            break;
    }
}

const InputAction* InputController::get_action(const char* name) {
    auto search_result = basic_inputs_.find(name);
    if (search_result == basic_inputs_.end()) return nullptr;

    return &search_result->second;
}

void InputController::poll_events() {
    for (size_t id = 0; id < KEYBOARD_CODE_COUNT; ++id) {
        keyboard_actions_[id].reset_states();
    }

    for (size_t id = 0; id < MOUSE_CODE_COUNT; ++id) {
        mouse_actions_[id].reset_states();
    }

    glfwPollEvents();
}

void InputController::register_text_listener(TextListener& listener) {
    text_listeners_.push_back(listener);
}
