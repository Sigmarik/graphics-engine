/**
 * @file material.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Material class
 * @version 0.1
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "graphics/primitives/shader.h"
#include "graphics/primitives/texture.h"
#include "scene.h"
#include "uniform_set.h"

static const unsigned TEXTURE_SLOT_COUNT = 4;

struct Material {
    explicit Material(const Shader& shader) : shader_(&shader) {}

    Material(const Material&) = default;
    Material& operator=(const Material&) = default;

    ~Material() = default;

    void use() const;

    template <class T>
    void set_uniform(const char* name, const T& value) {
        uniforms_.set(name, value);
    }

    void set_shader(const Shader& shader) { shader_ = &shader; }
    const Shader& get_shader() const { return *shader_; }

   private:
    const Shader* shader_;
    UniformSet uniforms_{};
};
