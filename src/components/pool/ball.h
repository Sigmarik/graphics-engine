/**
 * @file ball.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Billiard ball class.
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "graphics/objects/model.h"
#include "logics/components/visual/point_light_cmp.h"
#include "logics/components/visual/static_mesh.h"
#include "logics/scene.h"
#include "logics/scene_component.h"
#include "physics/objects/bouncy_object.h"

static const double POOL_BALL_RADIUS = 0.025;

struct PoolBall : public SceneComponent {
    PoolBall(const glm::vec3& position, const Model& model);

    void phys_tick(double delta_time) override;
    void draw_tick(double delta_time, double subtick_time = 0.0) override;

    void collide(PoolBall& ball);

    glm::vec3 get_position() const { return bouncer_.get_position(); }
    glm::vec3 get_velocity() const { return bouncer_.get_velocity(); }

    void set_position(const glm::vec3& pos) { bouncer_.set_position(pos); }
    void set_velocity(const glm::vec3& vel) { bouncer_.set_velocity(vel); }

    bool is_moving() const;
    bool is_on_board() const;

    void capture() override;
    void reset() override;

    virtual Box get_box() const override;

   protected:
    void begin_play(Scene& scene) override;

    static const Scene::ComponentLayerId BallLayer;

   private:
    void resolve_positions(PoolBall& ball);

    glm::vec3 captured_pos_{0.0};

    BouncyObject bouncer_;

    Subcomponent<PointLightComponent> shadow_{SubcomponentNone};
    Subcomponent<StaticMesh> model_{SubcomponentNone};

    Channel knocked_down_{};

    bool is_overboard_ = false;
};
