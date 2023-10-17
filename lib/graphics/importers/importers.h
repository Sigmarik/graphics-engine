/**
 * @file importers.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Visual asset importer
 * @version 0.1
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GRAPHICS_IMPORTERS_H
#define GRAPHICS_IMPORTERS_H

#include "graphics/primitives/shader.h"
#include "graphics/primitives/texture.h"
#include "managers/asset_manager.h"

struct TextureAsset : AbstractAsset {
    TextureAsset(const char* path, unsigned slot) : texture(path, slot) {}

    Texture texture;
};

struct ShaderAsset : AbstractAsset {
    ShaderAsset(const char* vsh, const char* fsh) : shader(vsh, fsh) {}

    Shader shader;
};

IMPORTER(TextureImporter)
IMPORTER(ShaderImporter)
// IMPORTER(MaterialImporter)
// IMPORTER(MeshImporter)
// IMPORTER(ModelImporter)

#endif
