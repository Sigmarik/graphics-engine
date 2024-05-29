/**
 * @file ambient_light_cmp.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Ambient light component
 * @version 0.1
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "graphics/objects/ambient_light.h"
#include "logics/scene_component.h"

struct AmbientLightComponent : public SceneComponent {
    AmbientLightComponent(const glm::vec3& color);

    glm::vec3 get_color() const { return light_.get_color(); }
    void set_color(const glm::vec3& color) { light_.set_color(color); }

   protected:
    void begin_play(Scene& scene) override;
    void end_play(EndPlayReason reason) override;

   private:
    AmbientLight light_;
};
