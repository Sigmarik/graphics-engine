/**
 * @file entity.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Entity class
 * @version 0.1
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <glm/vec3.hpp>

struct Scene;

struct Entity {
    Entity(Scene& scene) : scene_(scene) {}
    virtual ~Entity();

    virtual void synch_visuals() {}

    virtual void tick(double delta_time);

    void remove() { present_ = false; }
    bool is_present() const { return present_; }

   private:
    Scene& scene_;
    bool present_ = true;

    glm::vec3 position_ = glm::vec3(0.0, 0.0, 0.0);
};

#endif
