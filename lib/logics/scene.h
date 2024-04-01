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

#pragma once

#include <deque>
#include <map>
#include <memory>

#include "events.h"
#include "graphics/objects/scene.h"
#include "physics/level_geometry.h"
#include "scene_component.h"

struct Scene {
    friend struct SceneComponent;

    Scene(double width, double height, double cell_size);
    virtual ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    std::shared_ptr<SceneComponent> add_component(
        std::shared_ptr<SceneComponent> component);

    void add_component(SceneComponent& component);

    virtual void phys_tick(double delta_time);
    virtual void draw_tick(double delta_time, double subtick_time);

    LevelGeometry& get_collision() { return collision_; }
    const LevelGeometry& get_collision() const { return collision_; }

    RenderManager& get_renderer() { return renderer_; }
    const RenderManager& get_renderer() const { return renderer_; }

    TickEvent& get_phys_tick_event() { return phys_tick_; }
    const TickEvent& get_phys_tick_event() const { return phys_tick_; }

    SubtickEvent& get_draw_tick_event() { return draw_tick_; }
    const SubtickEvent& get_draw_tick_event() const { return draw_tick_; }

    void for_each_component(
        std::function<void(SceneComponent&)> function) const;

   private:
    /**
     * @brief Delete component from the scene in the next tick
     *
     * @warning Should only be requested by the component itself. Use
     * `SceneComponent::destroy()` for component deletion.
     *
     * @param[in] component
     */
    void delete_component(SceneComponent& component);
    void process_deletions();

   private:
    // TODO: Replace with grid-based component storage
    std::map<GUID, std::shared_ptr<SceneComponent>> shared_components_{};
    std::map<GUID, SceneComponent*> static_components_{};

    std::deque<GUID> deletion_queue_{};

    LevelGeometry collision_;
    RenderManager renderer_ = RenderManager();

    TickEvent phys_tick_{};
    SubtickEvent draw_tick_{};
};
