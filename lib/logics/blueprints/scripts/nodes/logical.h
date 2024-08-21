/**
 * @file logical.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Logical nodes
 * @version 0.1
 * @date 2024-07-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "node_types.h"

namespace nodes {

struct Conditional : public Script::Node {
    Conditional(ChildReference condition, ChildReference true_child,
                ChildReference false_child);

    virtual std::string debug() const override {
        return "{" + condition_->debug() + " ? " + true_child_->debug() +
               " : " + false_child_->debug() + "}";
    }

    virtual bool update(Node&) override;

   private:
    ChildReference condition_;
    ChildReference true_child_;
    ChildReference false_child_;
};

struct LogicalNot : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "!"; }
};

struct LogicalOr : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "or"; }
};

struct LogicalAnd : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "and"; }
};

struct LogicalXor : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "xor"; }
};

}  // namespace nodes
