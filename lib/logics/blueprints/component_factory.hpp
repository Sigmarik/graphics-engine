/**
 * @file component_factory.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Component factory class
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <map>
#include <string>

#include "logger/logger.h"
#include "logics/scene_component.h"

using SubcomponentNameMap = std::map<std::string, Subcomponent<SceneComponent>>;

/**
 * @brief Component builder
 *
 * @tparam Ts build arguments
 */
template <class... Ts>
struct ComponentFactory final {
    ComponentFactory() = default;
    ~ComponentFactory() { instructions_.clear(); }

    /**
     * @brief Run the factory
     *
     * @param[in] args construction parameters
     * @return SubcomponentNameMap - a name map of constructed components
     */
    SubcomponentNameMap build(Ts&&... args) const;

    /**
     * @brief Component producer
     *
     */
    using Producer = std::function<Subcomponent<SceneComponent>(Ts&&... args)>;

    /**
     * @brief Add instruction to the factory
     *
     * @param[in] name future component name
     * @param[in] producer
     */
    void register_producer(const std::string& name, const Producer& producer);

   private:
    std::map<std::string, Producer> instructions_{};
};

template <class... Ts>
inline SubcomponentNameMap ComponentFactory<Ts...>::build(Ts&&... args) const {
    SubcomponentNameMap map{};

    for (const auto& [name, producer] : instructions_) {
        map[name] = producer(std::forward<Ts>(args)...);
    }

    return map;
}

template <class... Ts>
inline void ComponentFactory<Ts...>::register_producer(
    const std::string& name, const Producer& producer) {
    if (instructions_.count(name) > 0) {
        log_printf(ERROR_REPORTS, "error",
                   "Double producer assignment for \"%s\"\n", name.c_str());
    }

    instructions_.insert({name, producer});
}
