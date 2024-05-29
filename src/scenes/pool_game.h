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

#pragma once

#include <array>

#include "graphics/objects/postprocessor.h"
#include "logics/components/visual/point_light_cmp.h"
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

    void load();

    Postprocessor contrast_vignette_;

    Subcomponent<PointLightComponent> main_lamp_;
    Subcomponent<PointLightComponent> sun_;

    std::vector<WeakSubcomponent<SceneComponent>> loaded_components_{};

    std::vector<WeakSubcomponent<GenericBall>> balls_{};
    WeakSubcomponent<PlayerBall> player_{};
};
