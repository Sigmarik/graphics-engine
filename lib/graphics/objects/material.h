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

#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>

#include "graphics/primitives/shader.h"
#include "graphics/primitives/texture.h"

static const unsigned TEXTURE_SLOTS_COUNT = 4;

struct Material {
    Material(Shader& shader) : shader_(shader) {}

    Material(const Material& material) = default;
    Material& operator=(const Material& material) = default;

    void use() const;

    void set_shader(Shader& shader) { shader_ = shader; }
    Shader& get_shader() const { return shader_; }

    void set_texture(const Texture& texture, unsigned slot) {
        textures_[slot] = &texture;
    }
    const Texture* get_texture(unsigned slot) const { return textures_[slot]; }

   private:
    Shader& shader_;
    const Texture* textures_[TEXTURE_SLOTS_COUNT];
};

#endif
