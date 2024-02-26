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

#include <string>
#include <unordered_map>

#include "graphics/primitives/shader.h"
#include "graphics/primitives/texture.h"
#include "scene.h"

static const unsigned TEXTURE_SLOT_COUNT = 4;

struct Material {
    explicit Material(const Shader& shader) : shader_(&shader) {}

    Material(const Material& instance) = default;
    ~Material() = default;

    Material& operator=(const Material& instance) = default;

    void use() const;

    void add_texture(const char* uniform, const Texture* texture);
    const Texture* get_texture(const char* uniform) const;

    void set_shader(const Shader& shader) { shader_ = &shader; }
    const Shader& get_shader() const { return *shader_; }

   private:
    const Shader* shader_;
    std::unordered_map<std::string, const Texture*> textures_ = {};
};

#endif
