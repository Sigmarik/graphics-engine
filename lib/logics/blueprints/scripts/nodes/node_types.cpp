#include "node_types.h"

bool nodes::BinaryNode::update(Node &) {
    if (!left_->get_value() || !right_->get_value()) {
        set_value({});
        return true;
    }

    set_value(binary_update(*left_->get_value(), *right_->get_value()));

    return true;
}

bool nodes::UnaryNode::update(Node &) {
    if (!value_->get_value()) {
        set_value({});
        return true;
    }

    set_value(unary_update(*value_->get_value()));

    return true;
}
