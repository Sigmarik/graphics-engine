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

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "events.h"
#include "graphics/objects/scene.h"
#include "hash/guid.h"
#include "memory/relative_ptr.hpp"
#include "physics/level_geometry.h"

struct Scene;

struct SceneComponent {
    friend struct Scene;

    using Channel = Event<std::string>;

    enum class EndPlayReason {
        Destroyed,
        Crashed,
        Quit,
    };

    SceneComponent();

    SceneComponent(const SceneComponent&) = delete;
    SceneComponent& operator=(const SceneComponent&) = delete;
    SceneComponent(SceneComponent&&) = delete;
    SceneComponent& operator=(SceneComponent&&) = delete;

    virtual ~SceneComponent() = default;

    virtual void phys_tick(double delta_time) {}
    virtual void draw_tick(double delta_time, double subtick_time = 0.0) {}

    Event<EndPlayReason>& get_destroyed_event() { return destroyed_event_; }

    GUID get_guid() const { return guid_; }

    bool is_valid() const;

    Scene& get_scene() const;

    void destroy(EndPlayReason reason = EndPlayReason::Destroyed);

   protected:
    Event<Scene&>& get_spawned_event() { return spawned_event_; }

    virtual void begin_play(Scene& scene);
    virtual void end_play(EndPlayReason reason) {}

    /**
     * @brief Subscribe the component to physics tick events
     *
     * @see `begin_play` method
     *
     * @warning Should be called after parent initialization in `begin_play`
     */
    void receive_phys_ticks();

    /**
     * @brief Subscribe the component to graphics tick events
     *
     * @see `begin_play` method
     *
     * @warning Should be called after parent initialization in `begin_play`
     */
    void receive_draw_ticks();

    /**
     * @brief Register an abstract output channel of the component
     *
     * @warning `output` must refer to a class member
     *
     * @param[in] name name of the channel
     * @param[in] output output channel member
     */
    void register_output(const std::string& name, Channel& output);

    /**
     * @brief Register abstract input channel listener of the component
     *
     * @warning `input` must refer to a class member
     *
     * @param[in] name name of the channel
     * @param[in] input channel listener member
     */
    void register_input(const std::string& name, Channel::Listener& input);

    /**
     * @brief Construct a child object
     *
     * @tparam T child type
     * @tparam Ts child constructions arguments
     * @param[in] args
     * @return std::shared_ptr<T> child
     */
    template <class T, class... Ts>
    std::shared_ptr<T> construct_child(Ts... args);

    void attach(SceneComponent& parent);

   private:
    GUID guid_;

    Scene* scene_ = nullptr;

    bool alive_ = true;
    TickEvent::Listener phys_ticker_{};
    SubtickEvent::Listener draw_ticker_{};

    Event<Scene&> spawned_event_{};
    Event<Scene&>::Listener parent_spawned_listener_{};

    Event<EndPlayReason> destroyed_event_{};
    Event<EndPlayReason>::Listener parent_destroyed_listener_{};

    std::unordered_map<std::string, RelativePtr<Channel>> outputs_{};
    std::unordered_map<std::string, RelativePtr<Channel::Listener>> inputs_{};
};

template <class T, class... Ts>
inline std::shared_ptr<T> SceneComponent::construct_child(Ts... args) {
    std::shared_ptr<T> child(new T(args...));

    child->attach(*this);

    return child;
}
