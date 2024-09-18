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

#include <stdio.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "geometry/primitives.h"
#include "hash/guid.h"

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
};

struct BoxCollider : public Collider {
    BoxCollider(const Box& box, const glm::mat4& transform = glm::mat4(1.0));

    Box get_bounding_box() const override;

    Box get_box() const { return box_; }
    void set_box(const Box& box) { box_ = box; }

    const glm::mat4& get_transform() const { return transform_; }
    void set_transform(const glm::mat4& transform);

    bool operator<(const BoxCollider& other) const;

   private:
    Box box_;
    glm::mat4 transform_ = glm::mat4(1.0);

    GUID guid_{};
};

struct DynamicCollider : public Collider {
    virtual Intersection intersect_box(const BoxCollider& box) const = 0;
};

struct SphereCollider : public DynamicCollider {
    SphereCollider() = default;
    SphereCollider(double radius, const glm::vec3& origin)
        : radius_(radius), origin_(origin) {}

    double get_radius() const { return radius_; }
    void set_radius(double radius) { radius_ = radius; }

    const glm::vec3& get_position() const { return origin_; }
    void set_position(const glm::vec3& position) { origin_ = position; }

    Box get_bounding_box() const override;

    Intersection intersect_box(const BoxCollider& box) const override;

   private:
    double radius_ = 1.0;
    glm::vec3 origin_ = glm::vec3(0.0, 0.0, 0.0);
};

#endif
