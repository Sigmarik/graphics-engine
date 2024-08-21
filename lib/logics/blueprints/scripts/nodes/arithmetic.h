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

   protected:
    virtual const std::string symbol() const override { return "valid?"; }
};

struct Length : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "length"; }
};

struct Equal : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "=="; }
};

struct NotEqual : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "!="; }
};

struct Greater : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return ">"; }
};

struct Less : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "<"; }
};

struct GreaterOrEqual : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return ">="; }
};

struct LessOrEqual : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "<="; }
};

struct Add : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "+"; }
};

struct Subtract : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "-"; }
};

struct Multiply : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "*"; }
};

struct Divide : public BinaryNode {
    using BinaryNode::BinaryNode;

    virtual std::optional<std::string> binary_update(
        const std::string& left, const std::string& right) override;

   protected:
    virtual const std::string symbol() const override { return "/"; }
};

struct Negative : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "-"; }
};

struct Absolute : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "abs"; }
};

struct Sign : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "sign"; }
};

struct Sin : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "sin"; }
};

struct Cos : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "cos"; }
};

struct LogE : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "logE"; }
};

struct Log2 : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "log2"; }
};

struct Log10 : public UnaryNode {
    using UnaryNode::UnaryNode;

    virtual std::optional<std::string> unary_update(const std::
                                                        string& input) override;

   protected:
    virtual const std::string symbol() const override { return "log10"; }
};

}  // namespace nodes
