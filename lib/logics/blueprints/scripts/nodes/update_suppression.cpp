#include "update_suppression.h"

nodes::DetectChange::DetectChange(ChildReference value) : value_(value) {
    subscribe_to(value_);
}

bool nodes::DetectChange::refresh(Node *) {
    auto current = value_.lock()->get_value();

    if (previous_value_ == current) return false;

    previous_value_ = current;

    set_value(current);

    return true;
}

nodes::RequireValidity::RequireValidity(ChildReference value) : value_(value) {}

bool nodes::RequireValidity::refresh(Node *) {
    auto current = value_.lock()->get_value();

    if (!current) return false;

    set_value(current);

    return true;
}
