/**
 * @file bouncy_head.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Bouncy monkey head
 * @version 0.1
 * @date 2024-01-07
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __SRC_COMPONENTS_MISC_BOUNCY_HEAD_H
#define __SRC_COMPONENTS_MISC_BOUNCY_HEAD_H

#include "graphics/objects/model.h"
#include "logics/scene_component.h"
#include "managers/asset_manager.h"
#include "physics/objects/bouncy_object.h"

struct BouncyHead : public SceneComponent {
    explicit BouncyHead(const glm::vec3& position);

    void spawn_self(Scene& scene) override;

    void phys_tick(double delta_time) override;
    void draw_tick(double delta_time, double subtick_time = 0.0) override;

    glm::vec3 get_position() const { return bouncer_.get_position(); }
    glm::vec3 get_velocity() const { return bouncer_.get_velocity(); }

   private:
    Model model_;
    BouncyObject bouncer_;
};

#endif /* __SRC_COMPONENTS_MISC_BOUNCY_HEAD_H */
