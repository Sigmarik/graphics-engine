/**
 * @file decal.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Decal class
 * @version 0.1
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DECAL_H
#define DECAL_H

#include <time.h>

#include "material.h"
#include "scene.h"

struct Decal : Renderable {
    Decal(const Material& material, double time_to_live);
    Decal(const Decal& decal) = default;
    ~Decal() = default;
    Decal& operator=(const Decal& decal) = default;

    int render(const RenderInput& input,
               const RenderBundle& bundle) const override;

    void reset();

   private:
    const Material* material_;
    const double time_to_live_;
    clock_t spawn_time_;
};

#endif
