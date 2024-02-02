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

#include <vector>

#include "logics/components/visual/ambient_light_cmp.h"
#include "logics/components/visual/point_light_cmp.h"
#include "logics/components/visual/static_mesh.h"
#include "logics/scene.h"
#include "physics/collider.h"
#include "src/components/pool/ball.h"
#include "src/components/pool/player_ball.h"

struct PoolGame : public Scene {
    PoolGame();

    void phys_tick(double delta_time) override;

   private:
    StaticMesh field_;
    StaticMesh frame_;

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

    // std::vector<PoolBall> balls_ = {};
    PlayerBall player_;
};

#endif /* __SRC_SCENES_POOL_GAME_H */
