/**
 * @file node.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Level script node
 * @version 0.1
 * @date 2024-06-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <optional>

#include "logics/scene.h"
#include "pipelining/event.hpp"
#include "script.h"

struct Script::Node {
    using ChildReference = std::shared_ptr<Node>;

    Node() {}

    Node(const Node&) = delete;
    Node(Node&&) = delete;

    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;

    virtual ~Node() = default;

    /**
     * @brief Update the value of the node.
     *
     * @param[in] initiator the node which initiated the update
     * @return true
     * @return false
     */
    virtual bool update(Node& initiator) { return true; }

    /**
     * @brief Get the value of the node
     *
     * @return const std::optional<std::string>&
     */
    const std::optional<std::string>& get_value() const { return value_; }

    /**
     * @brief Check if the node has a valid value
     *
     * @return true if the node has a value
     * @return false otherwise
     */
    bool has_value() const { return value_.has_value(); }

    /**
     * @brief Bind the node to the scene
     *
     * @note Nodes should be bound before being used
     *
     * @param[in] scene
     */
    void assign_scene(Scene& scene) { scene_ = &scene; }

   protected:
    using Update = Event<Node&>;

    /**
     * @brief Get update event of the node
     *
     * @return Update&
     */
    Update& get_update_event() { return update_event_; }

    /**
     * @brief Subscribe the node to another node's updates
     *
     * @param[in] other_node
     */
    void subscribe_to(ChildReference other_node);

    /**
     * @brief Get the scene the node is bound to
     *
     * @return Scene*
     */
    Scene* get_scene() const { return scene_; }

    virtual void on_assignment(Scene& scene);

    /**
     * @brief Trigger the update event of the node
     *
     */
    void trigger();

    void set_value(const std::optional<std::string>& value) { value_ = value; }

   private:
    std::optional<std::string> value_{};

    Update update_event_{};

    Scene* scene_ = nullptr;

    std::vector<Update::Listener> listeners_{};
};
