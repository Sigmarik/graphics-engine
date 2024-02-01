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

struct AmbientLight : public Renderable {
    explicit AmbientLight(const glm::vec3& color) : color_(color) {}

    int render(const RenderInput& input,
               const RenderBundle& bundle) const override;

    glm::vec3 get_color() const { return color_; }
    void set_color(const glm::vec3& color) { color_ = color; }

   private:
    glm::vec3 color_;
};

#endif
