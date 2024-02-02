/**
 * @file ball.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Pool ball class. Can be tossed around.
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __SRC_COMPONENTS_POOL_BALL_H
#define __SRC_COMPONENTS_POOL_BALL_H

#include "graphics/objects/model.h"
#include "logics/components/visual/point_light_cmp.h"
#include "logics/scene.h"
#include "logics/scene_component.h"
#include "physics/objects/bouncy_object.h"

static const double POOL_BALL_RADIUS = 0.025;

struct PoolBall : public SceneComponent {
    explicit PoolBall(Scene& scene, const glm::vec3& position,
                      const Model& model);

    void phys_tick(double delta_time) override;
    void draw_tick(double delta_time, double subtick_time = 0.0) override;

    void collide(PoolBall& ball);

    glm::vec3 get_position() const { return bouncer_.get_position(); }
    glm::vec3 get_velocity() const { return bouncer_.get_velocity(); }

    void set_position(const glm::vec3& pos) { bouncer_.set_position(pos); }
    void set_velocity(const glm::vec3& vel) { bouncer_.set_velocity(vel); }

   protected:
    void set_model_shift(const glm::vec3& shift) { model_shift_ = shift; }

   private:
    Model model_;
    BouncyObject bouncer_;

    PointLightComponent shadow_;

    glm::vec3 model_shift_ = glm::vec3(0.0);
};

#endif /* __SRC_COMPONENTS_POOL_BALL_H */
