/**
 * @file point_light.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Point light renderable
 * @version 0.1
 * @date 2023-10-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "scene.h"

struct PointLight : Renderable {
    PointLight(const glm::vec3& color) : color_(color) {}

    int render(const RenderInput& input,
               const RenderBundle& bundle) const override;

   private:
    glm::vec3 color_;
};

#endif
