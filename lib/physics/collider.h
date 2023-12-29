/**
 * @file collider.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Physical shape of the object.
 * @version 0.1
 * @date 2023-11-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "geometry/primitives.h"

struct Collider;

struct Intersection {
    ~Intersection() = default;

    bool overlap = false;
    glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 delta = glm::vec3(0.0, 0.0, 0.0);
};

struct Collider {
    virtual Box get_bounding_box() const { return Box(); };

    virtual ~Collider() = default;

   private:
    glm::vec3 origin_ = glm::vec3(0.0, 0.0, 0.0);
};

struct BoxCollider : public Collider {
    BoxCollider(const Box& box, const glm::mat4& transform)
        : box_(box), transform_(transform) {}

    Box get_bounding_box() const override;

    Box get_box() const { return box_; }
    void set_box(const Box& box) { box_ = box; }

    glm::mat4 get_transform() const { return transform_; }
    void set_transform(const glm::mat4& transform) { transform_ = transform; }

   private:
    Box box_;
    glm::mat4 transform_;
};

struct DynamicCollider : public Collider {
    virtual Intersection intersect_box(const BoxCollider& box) const = 0;
};

struct SphereCollider : public DynamicCollider {
    SphereCollider() = default;
    SphereCollider(double radius, const glm::vec3& origin)
        : radius_(radius), origin_(origin) {}

    Box get_bounding_box() const override;

    Intersection intersect_box(const BoxCollider& box) const override;

   private:
    double radius_ = 1.0;
    glm::vec3 origin_ = glm::vec3(0.0, 0.0, 0.0);
};

#endif
