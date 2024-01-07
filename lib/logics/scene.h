/**
 * @file scene.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Complete scene, be that editor view, game level or menu
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __LIB_LOGICS_SCENE_H
#define __LIB_LOGICS_SCENE_H

#include <set>

#include "graphics/objects/scene.h"
#include "physics/level_geometry.h"
#include "scene_component.h"

struct Scene {
    Scene(double width, double height, double cell_size);

    void add_component(SceneComponent& component);
    void delete_component(SceneComponent& component);

    void phys_tick(double delta_time) const;
    void draw_tick(double delta_time, double subtick_time) const;

    LevelGeometry& get_collision() { return collision_; }
    const LevelGeometry& get_collision() const { return collision_; }

    RenderManager& get_renderer() { return renderer_; }
    const RenderManager& get_renderer() const { return renderer_; }

    std::set<SceneComponent*>& get_components() { return components_; }
    const std::set<SceneComponent*>& get_components() const {
        return components_;
    }

   private:
    std::set<SceneComponent*> components_ = {};

    LevelGeometry collision_;
    RenderManager renderer_ = RenderManager();
};

#endif /* __LIB_LOGICS_SCENE_H */
