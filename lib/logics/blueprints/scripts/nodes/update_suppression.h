/**
 * @file update_suppression.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Update control nodes
 * @version 0.1
 * @date 2024-07-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/blueprints/scripts/node.h"

namespace nodes {

struct DetectChange : public Script::Node {
    DetectChange(ChildReference value);

    virtual bool refresh(Node*) override;

   private:
    ChildReference value_;

    std::optional<std::string> previous_value_;
};

struct RequireValidity : public Script::Node {
    RequireValidity(ChildReference value);

    virtual bool refresh(Node*) override;

   private:
    ChildReference value_;
};

}  // namespace nodes
