/**
 * @file rc_head.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Test pawn
 * @version 0.1
 * @date 2024-01-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __SRC_COMPONENTS_MISC_RC_HEAD_H
#define __SRC_COMPONENTS_MISC_RC_HEAD_H

#include "graphics/objects/model.h"
#include "logics/components/physical/pawn.h"

struct RCHead : public Pawn {
    RCHead(Scene& scene, const glm::vec3& position);

    void phys_tick(double delta_time) override;
    void draw_tick(double delta_time, double subtick_time = 0.0) override;

   private:
    Model model_;
};

#endif /* __SRC_COMPONENTS_MISC_RC_HEAD_H */
