/**
 * @file component_io.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Nodes for component interaction.
 * @version 0.1
 * @date 2024-06-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <memory>

#include "logics/blueprints/scripts/node.h"

namespace nodes {

struct OutputMethod : public Script::Node {
    OutputMethod(ChildReference object, ChildReference method);

    virtual bool update(Node&) override;

    virtual std::string debug() const override {
        return "{" + object_->debug() + " . " + method_->debug() + "}";
    }

   private:
    ChildReference object_;
    ChildReference method_;

    SceneComponent::Channel::Listener update_listener_{};
};

struct InputMethod : public Script::Node {
    InputMethod(ChildReference object, ChildReference method,
                ChildReference value);

    virtual bool update(Node& initiator) override;

    virtual std::string debug() const override {
        return "{" + object_->debug() + " :: " + method_->debug() + " <- " +
               value_->debug() + "}";
    }

   private:
    void connect();

    ChildReference object_;
    ChildReference method_;
    ChildReference value_;

    bool connected_ = false;

    SceneComponent::Channel output_{};
};

struct StringConstant : public Script::Node {
    StringConstant(const std::string& value);

    virtual std::string debug() const override {
        return "{\"" + get_value().value_or("NOTHING") + "\"}";
    }

    virtual bool update(Node& initiator) override;
};

};  // namespace nodes
