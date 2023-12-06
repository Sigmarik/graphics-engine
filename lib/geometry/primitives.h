/**
 * @file box.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Geometrical primitives
 * @version 0.1
 * @date 2023-11-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/vec3.hpp>

struct Box {
    Box() = default;
    Box(const glm::vec3& center, const glm::vec3& size)
        : center_(center), size_(size) {}

    glm::vec3 get_center() const { return center_; }
    void set_center(const glm::vec3& center) { center_ = center; }

    glm::vec3 get_size() const { return size_; }
    void set_size(const glm::vec3& size) { size_ = size; }

    glm::vec3 local(const glm::vec3 pos) const {
        return center_ + size_ * pos / 0.5f;
    }

    bool contains(const Box& box) const;

   private:
    glm::vec3 center_ = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 size_ = glm::vec3(1.0, 1.0, 1.0);
};

bool intersect(const Box& alpha, const Box& beta);

struct Sphere {
    Sphere() = default;
    Sphere(glm::vec3 center, double radius)
        : center_(center), radius_(radius){};

    glm::vec3 get_center() const { return center_; };
    void set_center(const glm::vec3& center) { center_ = center; }

    double get_radius() const { return radius_; }
    void set_radius(double radius) { radius_ = radius; }

   private:
    glm::vec3 center_ = glm::vec3(0.0, 0.0, 0.0);
    double radius_ = 1.0;
};

bool intersect(const Sphere& alpha, const Sphere& beta);

#endif
