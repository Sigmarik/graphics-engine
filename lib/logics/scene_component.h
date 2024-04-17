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

#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>

#include "events.h"
#include "graphics/objects/scene.h"
#include "hash/guid.h"
#include "memory/relative_ptr.hpp"
#include "physics/level_geometry.h"
#include "subcomponent.hpp"

struct Scene;

struct SceneComponent {
    friend struct Scene;

    using Channel = Event<const std::string&>;

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

    /**
     * @brief Request a destroyed event of the component that is triggered when
     * the component gets destroyed
     *
     * @return Event<EndPlayReason>&
     */
    Event<EndPlayReason>& get_destroyed_event() { return destroyed_event_; }

    GUID get_guid() const { return guid_; }

    /**
     * @brief Check if the component is bound to a scene and alive
     *
     * @return true
     * @return false
     */
    bool is_valid() const;

    /**
     * @brief Get the owning scene of the component
     *
     * @warning Should only be called on `valid` components
     *
     * @return Scene&
     */
    Scene& get_scene() const;

    bool has_scene() const { return scene_ != nullptr; }

    /**
     * @brief Destroy the component and remove it from the scene
     *
     * @param[in] reason
     */
    void destroy(EndPlayReason reason = EndPlayReason::Destroyed);

    Channel* get_output(const std::string& name);
    Channel::Listener* get_input(const std::string& name);

    /**
     * @brief Capture component state for restoration
     *
     * @warning Implementation of this method is entirely optional, component
     * may choose not to capture its current state
     */
    virtual void capture() {}

    /**
     * @brief Reset component to its previously captured state
     *
     * @warning Implementation of this method is entirely optional, component
     * may choose not to reset itself
     */
    virtual void reset() {}

   protected:
    Event<Scene&>& get_spawned_event() { return spawned_event_; }

    /**
     * @brief A function that is called when the component gets added to a scene
     *
     * @param[in] scene owning scene
     */
    virtual void begin_play(Scene& scene);

    /**
     * @brief A function that is called when the component gets removed from the
     * scene
     *
     * @param[in] reason removal reason
     */
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
     * @return Subcomponent<T> child
     */
    template <class T, class... Ts>
    requires std::derived_from<T, SceneComponent> Subcomponent<T> new_child(
        Ts&&... args);

    /**
     * @brief Attach a child to the component
     *
     * @param[in] child child component
     */
    void attach(Subcomponent<SceneComponent> child);

   private:
    GUID guid_;

    Scene* scene_ = nullptr;

    bool alive_ = false;
    TickEvent::Listener phys_ticker_{};
    SubtickEvent::Listener draw_ticker_{};

    Event<Scene&> spawned_event_{};
    Event<EndPlayReason> destroyed_event_{};

    Event<Scene&>::Listener parent_spawned_listener_{};
    Event<EndPlayReason>::Listener parent_destroyed_listener_{};

    std::unordered_map<std::string, RelativePtr<Channel>> outputs_{};
    std::unordered_map<std::string, RelativePtr<Channel::Listener>> inputs_{};
};

template <class T, class... Ts>
requires std::derived_from<T, SceneComponent>
inline Subcomponent<T> SceneComponent::new_child(Ts&&... args) {
    Subcomponent<T> child(args...);

    attach(child);

    return child;
}
