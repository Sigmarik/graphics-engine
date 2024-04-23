/**
 * @file component_pack.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Component pack definition
 * @version 0.1
 * @date 2024-04-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <vector>

#include "logics/scene_component.h"

struct ComponentPack : public SceneComponent {
    void add_component(Subcomponent<SceneComponent> component) {
        components_.push_back(component);
    }

   protected:
    void begin_play(Scene& scene) override;

   private:
    std::vector<Subcomponent<SceneComponent>> components_{};
};
