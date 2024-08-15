/**
 * @file node_types.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Useful node types
 * @version 0.1
 * @date 2024-07-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/blueprints/scripts/node.h"

namespace nodes {

struct BinaryNode : public Script::Node {
    BinaryNode(ChildReference left, ChildReference right)
        : left_(left), right_(right) {
        subscribe_to(left_);
        subscribe_to(right_);
    }

    virtual bool update(Node&) override;

    virtual std::string debug() const override {
        return "{" + left_->debug() + " " + symbol() + " " + right_->debug() +
               "}";
    }

   protected:
    virtual std::optional<std::string> binary_update(const std::string& left,
                                                     const std::string& right) {
        return "";
    }

    /**
     * @brief Symbol used for debug string representation
     *
     * @return const std::string
     */
    virtual const std::string symbol() const = 0;

    ChildReference left_;
    ChildReference right_;
};

struct UnaryNode : public Script::Node {
    UnaryNode(ChildReference value) : value_(value) { subscribe_to(value_); }

    virtual std::string debug() const override {
        return "{" + symbol() + " " + value_->debug() + "}";
    }

    virtual bool update(Node&) override;

   protected:
    /**
     * @brief Symbol used for debug string representation
     *
     * @return const std::string
     */
    virtual const std::string symbol() const = 0;

    virtual std::optional<std::string> unary_update(const std::string& input) {
        return "";
    }

    ChildReference value_;
};

};  // namespace nodes
