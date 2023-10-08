/**
 * @file asset_shelf.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Asset management functionality
 * @version 0.1
 * @date 2023-10-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ASSET_SHELF_H
#define ASSET_SHELF_H

#include "graphics/primitives/mesh.h"
#include "graphics/primitives/shader.h"
#include "graphics/primitives/texture.h"

// TODO: Complete!

struct Material {
    Material();
    explicit Material(const char* folder);
    Material(const char* shader_vsh_path, const char* shader_fsh_path,
             const char* atlas_path);

    Shader shader;
    Texture atlas;
};

struct Model {
    Model() = default;
    Model(const char* model_path, Material& material);

    Mesh shape;
    const Material& material;
};

struct AssetShelf final {
    static AssetShelf& get();

   private:
    AssetShelf() = default;
};

#endif
