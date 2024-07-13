/**
 * @file arithmetic.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Arithmetics
 * @version 0.1
 * @date 2024-07-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "node_types.h"

namespace nodes {

struct IsValid : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual bool update(Node&) override;
};

struct Length : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

struct Equal : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct NotEqual : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct Greater : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct Less : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct GreaterOrEqual : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct LessOrEqual : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct Add : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct Subtract : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct Multiply : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct Divide : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;
};

struct Negative : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

struct Absolute : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

struct Sign : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

struct Sin : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

struct Cos : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

struct LogE : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

struct Log2 : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

struct Log10 : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(
        const std::string& input) override;
};

}  // namespace nodes
