/**
 * @file scene_component.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Scene component module
 * @version 0.1
 * @date 2023-12-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __LOGICS_SCENE_COMPONENT_H
#define __LOGICS_SCENE_COMPONENT_H

#include "graphics/objects/scene.h"
#include "hash/guid.h"
#include "physics/level_geometry.h"

struct Scene;

struct SceneComponent {
    SceneComponent();

    SceneComponent(const SceneComponent& instance) = default;
    virtual ~SceneComponent() = default;

    SceneComponent& operator=(const SceneComponent& instance) = default;

    virtual void spawn_self(Scene& scene);

    virtual void phys_tick(double delta_time) {}
    virtual void draw_tick(double delta_time, double subtick_time = 0.0) {}

    GUID get_guid() const { return guid_; }

    Scene* get_scene() const { return scene_; }

   private:
    GUID guid_;

    Scene* scene_ = nullptr;
};

#endif
