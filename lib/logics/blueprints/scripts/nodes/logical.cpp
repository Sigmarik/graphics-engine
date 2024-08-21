#include "logical.h"

#include "booleans.h"

nodes::Conditional::Conditional(ChildReference condition,
                                ChildReference true_child,
                                ChildReference false_child)
    : condition_(condition),
      true_child_(true_child),
      false_child_(false_child) {
    subscribe_to(condition_);
    subscribe_to(true_child_);
    subscribe_to(false_child_);
}

bool nodes::Conditional::update(Node &) {
    if (!condition_->has_value()) {
        set_value({});

        return true;
    }

    ChildReference source =
        get_bool(*condition_->get_value()) ? true_child_ : false_child_;

    set_value(source->get_value());

    return true;
}

std::optional<std::string> nodes::LogicalNot::unary_update(
    const std::string &input) {
    return to_bool(!get_bool(input));
}

std::optional<std::string> nodes::LogicalOr::binary_update(
    const std::string &left, const std::string &right) {
    return to_bool(get_bool(left) || get_bool(right));
}

std::optional<std::string> nodes::LogicalAnd::binary_update(
    const std::string &left, const std::string &right) {
    return to_bool(get_bool(left) && get_bool(right));
}

std::optional<std::string> nodes::LogicalXor::binary_update(
    const std::string &left, const std::string &right) {
    return to_bool(get_bool(left) != get_bool(right));
}
