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

struct Line {
    glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 direction = glm::vec3(0.0, 1.0, 0.0);
};

struct Plane {
    glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
};

static const unsigned char X_MASK = 0x01;
static const unsigned char Y_MASK = 0x02;
static const unsigned char Z_MASK = 0x04;

enum class IntersectionType {
    OVERLAP,
    OVERSET,
    UNDERSET,
};

struct Box {
    Box() = default;
    Box(const glm::vec3& center, const glm::vec3& size)
        : center_(center), size_(size) {}

    glm::vec3 get_center() const { return center_; }
    void set_center(const glm::vec3& center) { center_ = center; }

    glm::vec3 get_size() const { return size_; }
    void set_size(const glm::vec3& size) { size_ = size; }

    glm::vec3 local(const glm::vec3 pos) const {
        return center_ + size_ * pos / 2.0f;
    }

    bool contains(const Box& box) const;

    glm::vec3 get_corner(unsigned id) const;
    Line get_edge(unsigned id, unsigned char slice_mask) const;
    Plane get_face(unsigned id, unsigned char slice_mask) const;
    Plane get_slice(unsigned id) const;

    bool operator==(const Box& other) const;

   private:
    glm::vec3 center_ = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 size_ = glm::vec3(1.0, 1.0, 1.0);
};

bool intersect(const Box& alpha, const Box& beta);

bool match_intersection(const Box& alpha, const Box& beta,
                        IntersectionType intersection);

struct Sphere {
    Sphere() = default;
    Sphere(glm::vec3 center, double radius)
        : center_(center), radius_(radius) {};

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
