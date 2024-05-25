#include "collider.h"

#include <math.h>
#include <stdio.h>

#include <glm/geometric.hpp>

#include "logger/logger.h"

Box SphereCollider::get_bounding_box() const {
    return Box(origin_, glm::vec3(radius_, radius_, radius_) * 2.0f);
}

static Intersection intersect_point(const Box& box, const glm::vec3& center,
                                    double radius, unsigned id,
                                    unsigned char mask) {
    glm::vec3 point = box.get_corner(id);

    double distance = glm::distance(center, point);

    return (Intersection){
        .overlap = distance < radius,
        .center = point,
        .delta = (center - point) * (float)(radius / distance - 1.0),
    };
}

static Intersection intersect_edge(const Box& box, const glm::vec3& center,
                                   double radius, unsigned id,
                                   unsigned char mask) {
    Line edge = box.get_edge(id, mask);

    glm::vec3 relative = center - edge.origin;
    glm::vec3 projected = edge.direction * glm::dot(relative, edge.direction);

    double distance = glm::distance(relative, projected);

    return (Intersection){
        .overlap = distance < radius,
        .center = edge.origin + projected,
        .delta = (relative - projected) * (float)(radius / distance - 1.0),
    };
}

static Intersection intersect_face(const Box& box, const glm::vec3& center,
                                   double radius, unsigned id,
                                   unsigned char mask) {
    Plane face = box.get_face(id, mask);

    glm::vec3 relative = center - face.origin;

    float shift = glm::dot(relative, face.normal);
    glm::vec3 projected = relative - face.normal * shift;

    return (Intersection){
        .overlap = abs(shift) < radius,
        .center = face.origin + projected,
        .delta = (relative - projected) * (float)(radius / abs(shift) - 1.0),
    };
}

static Intersection intersect_volume(const Box& box, const glm::vec3& center,
                                     double radius, unsigned id,
                                     unsigned char mask) {
    Plane face = box.get_slice(id);

    glm::vec3 relative = center - face.origin;

    double distance = glm::dot(relative, face.normal);
    glm::vec3 projected = relative - face.normal * (float)distance;

    return (Intersection){
        .overlap = distance < radius,
        .center = face.origin + projected,
        .delta = (relative - projected) * (float)(radius / distance - 1.0),
    };
}

typedef Intersection intersector_t(const Box& box, const glm::vec3& center,
                                   double radius, unsigned id,
                                   unsigned char mask);

Intersection SphereCollider::intersect_box(const BoxCollider& box) const {
    glm::mat4 world_to_collider = glm::inverse(box.get_transform());

    glm::vec4 local_center = world_to_collider * glm::vec4(origin_, 1.0);
    glm::vec3 local =
        glm::vec3(local_center.x, local_center.y, local_center.z) /
        local_center.w;

    unsigned intersects = 0;

    glm::vec3 size = box.get_box().get_size();
    glm::vec3 center = box.get_box().get_center();

    unsigned char mask = 0;

    if (abs(local.x - center.x) * 2.0 < size.x) {
        intersects++;
        mask |= X_MASK;
    }
    if (abs(local.y - center.y) * 2.0 < size.y) {
        intersects++;
        mask |= Y_MASK;
    }
    if (abs(local.z - center.z) * 2.0 < size.z) {
        intersects++;
        mask |= Z_MASK;
    }

    Intersection closest = (Intersection){
        .overlap = false,
        .center = glm::vec3(0.0, 0.0, 0.0),
        .delta = glm::vec3(0.0, 0.0, 0.0),
    };

    static const unsigned PRIMITIVE_COUNTS[4] = {8, 4, 2, 6};
    static const intersector_t* INTERSECTORS[4] = {
        intersect_point,
        intersect_edge,
        intersect_face,
        intersect_volume,
    };

    intersector_t* intersect = INTERSECTORS[intersects];

    unsigned count = PRIMITIVE_COUNTS[intersects];
    for (unsigned id = 0; id < count; ++id) {
        Intersection intersection =
            intersect(box.get_box(), local, radius_, id, mask);

        if (!intersection.overlap) continue;

        bool close =
            glm::length(intersection.delta) <= glm::length(closest.delta);

        if ((close == (intersects < 2)) && closest.overlap) continue;

        closest = intersection;
    }

    glm::mat4 local_to_world = box.get_transform();
    glm::vec4 new_center = local_to_world * glm::vec4(closest.center, 1.0);
    glm::vec4 new_delta = local_to_world * glm::vec4(closest.delta, 0.0);

    new_center /= new_center.w;

    closest.center = glm::vec3(new_center.x, new_center.y, new_center.z);
    closest.delta = glm::vec3(new_delta.x, new_delta.y, new_delta.z);

    return closest;
}

static glm::vec3 transform(const glm::mat4& matrix, const glm::vec3 vector) {
    glm::vec4 transformed = matrix * glm::vec4(vector, 1.0);
    return glm::vec3(transformed.x, transformed.y, transformed.z) /
           transformed.w;
}

static float min(float alpha, float beta) {
    return alpha < beta ? alpha : beta;
}
static float max(float alpha, float beta) {
    return alpha > beta ? alpha : beta;
}

BoxCollider::BoxCollider(const Box& box, const glm::mat4& transform)
    : box_(box), transform_(transform) {
    if (abs(glm::determinant(transform_) - 1.0) > 1e-4f) {
        log_printf(WARNINGS, "warning",
                   "Trying to construct a box collider with transform "
                   "determinant %g\n",
                   glm::determinant(transform_));
    }
}

Box BoxCollider::get_bounding_box() const {
    glm::vec3 start = transform(get_transform(), box_.get_corner(0));

    glm::vec3 low = start;
    glm::vec3 high = start;

    for (unsigned id = 1; id < 8; ++id) {
        glm::vec3 point = transform(get_transform(), box_.get_corner(id));

        low.x = min(point.x, low.x);
        low.y = min(point.y, low.y);
        low.z = min(point.z, low.z);

        high.x = max(point.x, high.x);
        high.y = max(point.y, high.y);
        high.z = max(point.z, high.z);
    }

    return Box((high + low) / 2.0f, high - low);
}

void BoxCollider::set_transform(const glm::mat4& transform) {
    transform_ = transform;

    if (abs(glm::determinant(transform_)) < 1e-4f) {
        log_printf(
            WARNINGS, "warning",
            "Trying to assign singular matrix as a box collider transform\n");
    }
}
