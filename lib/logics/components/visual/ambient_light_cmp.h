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

#ifndef __LIB_LOGICS_COMPONENTS_VISUAL_AMBIENT_LIGHT_CMP_H
#define __LIB_LOGICS_COMPONENTS_VISUAL_AMBIENT_LIGHT_CMP_H

#include "graphics/objects/ambient_light.h"
#include "logics/scene_component.h"

struct AmbientLightComponent : public SceneComponent {
    AmbientLightComponent(const glm::vec3& color);

    void spawn_self(Scene& scene) override;

    glm::vec3 get_color() const { return light_.get_color(); }
    void set_color(const glm::vec3& color) { light_.set_color(color); }

   private:
    AmbientLight light_;
};

#endif /* __LIB_LOGICS_COMPONENTS_VISUAL_AMBIENT_LIGHT_CMP_H */
