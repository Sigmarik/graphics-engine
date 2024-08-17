/**
 * @file shouter.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief A component that shouts whatever comes into its input channels
 * @version 0.1
 * @date 2024-08-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/scene_component.h"

struct ShouterComponent : public SceneComponent {
    ShouterComponent(const std::string& name);

   private:
    std::string name_;
    Channel::Listener input_;
};
