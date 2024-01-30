#include "binary_input.h"

void BinaryInput::add_activator(const InputAction& action) {
    activators_.push_back(&action);
}

bool BinaryInput::poll_pushed() const {
    for (const InputAction* action : activators_) {
        if (action->poll_pushed()) return true;
    }

    return false;
}

bool BinaryInput::poll_released() const {
    for (const InputAction* action : activators_) {
        if (action->poll_released()) return true;
    }

    return false;
}

bool BinaryInput::get_pushed() const {
    for (const InputAction* action : activators_) {
        if (action->get_pushed()) return true;
    }

    return false;
}
