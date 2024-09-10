/**
 * @file scene.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Scene manager
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

#include "blueprints/scripts/script.h"
#include "events.h"
#include "geometry/box_search.hpp"
#include "graphics/objects/scene.h"
#include "hash/guid.h"
#include "physics/level_geometry.h"
#include "subcomponent.hpp"

struct SceneComponent;

struct Scene {
    friend struct SceneComponent;

    Scene(double width, double height, double cell_size);
    virtual ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    void add_component(Subcomponent<SceneComponent> component);

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

    SceneComponent* get_component(GUID guid);

    size_t get_component_count() const { return shared_components_.size(); }

    std::shared_ptr<Script> add_script(const Script& script);

    using ComponentLayerId = GUID;

    std::set<GUID> get_components_in_area(
        const Box& box, ComponentLayerId layer,
        IntersectionType intersection = IntersectionType::OVERLAP) const;

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

    void add_boxable_component(const SceneComponent& component);
    void remove_boxable_component(const SceneComponent& component);

    void update_boxable_component(const SceneComponent& component);

   private:
    double width_, height_, cell_size_;

    std::map<GUID, Subcomponent<SceneComponent>> shared_components_{};

    std::deque<GUID> deletion_queue_{};

    LevelGeometry collision_;
    RenderManager renderer_ = RenderManager();

    std::vector<std::shared_ptr<Script>> scripts_{};

    TickEvent phys_tick_{};
    SubtickEvent draw_tick_{};

    std::map<ComponentLayerId, BoxField<GUID>> box_fields_{};
};
