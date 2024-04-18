/**
 * @file pawn.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Basic pawn class
 * @version 0.1
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <physics/objects/character_body.h>

#include "logics/scene_component.h"
#include "managers/asset_manager.h"

struct Pawn : public SceneComponent {
    Pawn(const glm::vec3& position, double width, double height);

    void phys_tick(double delta_time) override;

   protected:
    void begin_play(Scene& scene) override;

    void move(const glm::vec3& input) { body_.move(input); }

    CharacterBody& get_body() { return body_; }
    const CharacterBody& get_body() const { return body_; }

   private:
    CharacterBody body_;
};
