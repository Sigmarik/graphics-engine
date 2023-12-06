/**
 * @file scene.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Scene class
 * @version 0.1
 * @date 2023-11-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SCENE_H
#define SCENE_H

#include <set>

#include "graphics/objects/scene.h"

struct Entity;

struct Scene {
    void tick(double delta_time);
    void synch_visuals();

    void add_entity(Entity& entity) { entities_.insert(&entity); }
    void kill_entity(Entity& entity);

   private:
    void clean();
    std::set<Entity*> entities_ = {};
};

#endif
