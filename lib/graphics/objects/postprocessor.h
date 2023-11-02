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

#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include "glm/vec3.hpp"
#include "material.h"
#include "scene.h"

struct Postprocessor : Renderable {
    Postprocessor(const Material& material) : material_(&material) {}
    Postprocessor(const Postprocessor& postprocessor) = default;
    Postprocessor& operator=(const Postprocessor& postprocessor) = default;
    ~Postprocessor() = default;

    int render(const RenderInput& input,
               const RenderBundle& bundle) const override;

    void set_material(const Material* material) { material_ = material; }
    const Material* get_material() const { return material_; }

   private:
    const Material* material_;
};

#endif
