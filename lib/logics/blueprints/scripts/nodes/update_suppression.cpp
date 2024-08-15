#include "update_suppression.h"

nodes::DetectChange::DetectChange(ChildReference value) : value_(value) {
    subscribe_to(value_);
}

bool nodes::DetectChange::update(Node &) {
    auto current = value_->get_value();

    if (previous_value_ == current) return false;

    previous_value_ = current;

    set_value(current);

    return true;
}

nodes::RequireValidity::RequireValidity(ChildReference value) : value_(value) {
    subscribe_to(value_);
}

bool nodes::RequireValidity::update(Node &) {
    auto current = value_->get_value();

    if (!current) return false;

    set_value(current);

    return true;
}

nodes::DetectSource::DetectSource(ChildReference value) : value_(value) {
    subscribe_to(value_);
}

bool nodes::DetectSource::update(Node &) {
    set_value(value_->get_value());

    trigger();

    set_value({});

    return false;
}
