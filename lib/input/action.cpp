#include "action.h"

#include "graphics/libs.h"

void InputAction::reset_states() {
    just_pushed_ = false;
    just_released_ = false;
}

void InputAction::update(int basic_action) {
    if (basic_action == GLFW_PRESS) {
        just_pushed_ = true;
        active_ = true;
    } else if (basic_action == GLFW_RELEASE) {
        just_released_ = true;
        active_ = false;
    }
}
