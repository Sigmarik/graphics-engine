/**
 * @file external_level.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Importable level class
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <glm/mat4x4.hpp>

#include "component_factory.hpp"

struct ExternalLevel final {
    using Factory = ComponentFactory<const glm::mat4&>;

    ExternalLevel(const Factory& factory) : factory_(factory) {}

    void build(Scene& scene, const glm::mat4& transform);

   private:
    Factory factory_;

    // TODO: Add pipeliner script set here
};
