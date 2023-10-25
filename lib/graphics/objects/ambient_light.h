/**
 * @file ambient_light.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Ambient light source
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "glm/vec3.hpp"
#include "scene.h"

struct AmbientLight : Renderable {
    AmbientLight(const glm::vec3& color) : color_(color) {}

    void render(const RenderInput& input,
                const RenderBundle& bundle) const override;

   private:
    glm::vec3 color_;
};

#endif
