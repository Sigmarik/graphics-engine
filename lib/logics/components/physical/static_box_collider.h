/**
 * @file static_box_collider.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Static box collider
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/scene.h"
#include "logics/scene_component.h"
#include "physics/collider.h"

struct StaticBoxCollider : public SceneComponent {
    StaticBoxCollider(const BoxCollider& collider) : collider_(collider) {}

    void begin_play(Scene& scene) override;

   private:
    BoxCollider collider_;
};
