/**
 * @file point_light.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Point light scene component
 * @version 0.1
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "graphics/objects/point_light.h"
#include "logics/scene_component.h"

struct PointLightComponent : public SceneComponent {
    PointLightComponent(const glm::vec3& position, const glm::vec3& color);

    glm::vec3 get_position() const { return position_; }
    void set_position(const glm::vec3& position);

    glm::vec3 get_color() const { return light_.get_color(); }
    void set_color(const glm::vec3& color) { light_.set_color(color); }

    float get_spread() const { return light_.get_spread(); }
    void set_spread(float spread) { light_.set_spread(spread); }

    float get_radius() const { return light_.get_radius(); }
    void set_radius(float radius) { light_.set_radius(radius); }

   protected:
    void begin_play(Scene& scene) override;
    void end_play(EndPlayReason reason) override;

   private:
    glm::vec3 position_;

    PointLight light_;
};
