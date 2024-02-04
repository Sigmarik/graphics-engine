/**
 * @file pool_game.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Pool game scene type
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __SRC_SCENES_POOL_GAME_H
#define __SRC_SCENES_POOL_GAME_H

#include <array>

#include "graphics/objects/postprocessor.h"
#include "logics/components/visual/ambient_light_cmp.h"
#include "logics/components/visual/point_light_cmp.h"
#include "logics/components/visual/static_mesh.h"
#include "logics/scene.h"
#include "physics/collider.h"
#include "src/components/pool/generic_ball.h"
#include "src/components/pool/player_ball.h"

struct PoolGame : public Scene {
    PoolGame();

    void phys_tick(double delta_time) override;

    void reset();

   protected:
    bool has_moving_parts() const;

   private:
    void process_int_collisions();

    StaticMesh field_;
    StaticMesh frame_;

    Postprocessor contrast_vignette_;

    PointLightComponent main_lamp_;
    PointLightComponent sun_;
    AmbientLightComponent ambient_;

    BoxCollider floor_;
    BoxCollider top_left_;
    BoxCollider top_right_;
    BoxCollider bottom_left_;
    BoxCollider bottom_right_;
    BoxCollider left_top_;
    BoxCollider left_bottom_;
    BoxCollider right_top_;
    BoxCollider right_bottom_;

    GenericBall balls_[6];
    PlayerBall player_;
};

#endif /* __SRC_SCENES_POOL_GAME_H */
