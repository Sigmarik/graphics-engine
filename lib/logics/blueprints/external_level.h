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
#include "scripts/script.h"

struct ExternalLevel final {
    using Factory = ComponentFactory<const glm::mat4&>;

    struct Metadata {};

    ExternalLevel(const Factory& factory, const std::vector<Script>& scripts,
                  const Metadata* meta)
        : factory_(factory), scripts_(scripts), meta_(meta) {}

    ExternalLevel(const ExternalLevel&) = default;
    ExternalLevel(ExternalLevel&&) = default;

    ExternalLevel& operator=(const ExternalLevel&) = default;
    ExternalLevel& operator=(ExternalLevel&&) = default;

    ~ExternalLevel() = default;

    SubcomponentNameMap build(Scene& scene, const glm::mat4& transform) const;

    template <class T = Metadata>
        requires std::is_base_of_v<Metadata, T>
    const T* meta() const {
        return dynamic_cast<const T*>(meta_);
    }

    bool has_meta() const { return meta_; }

   private:
    Factory factory_;

    std::vector<Script> scripts_;

    const Metadata* meta_;
};
