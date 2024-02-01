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

struct PointLight : public Renderable {
    explicit PointLight(const glm::vec3& color, float spread = 3.0)
        : color_(color), spread_(spread) {}

    int render(const RenderInput& input,
               const RenderBundle& bundle) const override;

    glm::vec3 get_color() const { return color_; }
    void set_color(const glm::vec3& color) { color_ = color; }

    float get_spread() const { return spread_; }
    void set_spread(float spread) { spread_ = spread; }

    float get_radius() const { return radius_; }
    void set_radius(float radius) { radius_ = radius; }

   private:
    glm::vec3 color_;
    float spread_;
    float radius_ = 0.0;
};

#endif
