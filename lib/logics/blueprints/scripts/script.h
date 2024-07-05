/**
 * @file script.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Level script object
 * @version 0.1
 * @date 2024-06-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/scene_component.h"
#include "node.h"

struct Script {
    struct Node;

   private:
    std::vector<std::shared_ptr<Node>> nodes_;
};
