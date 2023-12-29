#include "collider.h"

#include <math.h>

#include <glm/geometric.hpp>

static Intersection intersect_point(const Box& box, const glm::vec3& center,
                                    double radius, unsigned id) {
    glm::vec3 point = box.get_corner(id);

    double distance = glm::distance(center, point);

    return (Intersection){
        .overlap = distance < radius,
        .center = point,
        .delta = (center - point) * (float)(radius / distance - 1.0),
    };
}

static Intersection intersect_edge(const Box& box, const glm::vec3& center,
                                   double radius, unsigned id) {
    Line edge = box.get_edge(id);

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
                                   double radius, unsigned id) {
    Plane face = box.get_face(id);

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
                                   double radius, unsigned id);

Box SphereCollider::get_bounding_box() const {
    return Box(origin_, glm::vec3(radius_, radius_, radius_) * 2.0f);
}

Intersection SphereCollider::intersect_box(const BoxCollider& box) const {
    glm::vec4 local_center =
        glm::inverse(box.get_transform()) * glm::vec4(origin_, 1.0);
    glm::vec3 local = glm::vec3(local_center.x, local_center.y, local_center.z);

    unsigned intersects = 0;

    glm::vec3 size = box.get_box().get_size();

    if (abs(local.x) * 2.0 < size.x) intersects++;
    if (abs(local.y) * 2.0 < size.y) intersects++;
    if (abs(local.z) * 2.0 < size.z) intersects++;

    Intersection closest = (Intersection){
        .overlap = false,
        .center = glm::vec3(0.0, 0.0, 0.0),
        .delta = glm::vec3(0.0, 0.0, 0.0),
    };

    static const unsigned PRIMITIVE_COUNTS[4] = {8, 12, 6, 6};
    static const intersector_t* INTERSECTORS[4] = {
        intersect_point,
        intersect_edge,
        intersect_face,
        intersect_face,
    };

    intersector_t* intersect = INTERSECTORS[intersects];

    unsigned count = PRIMITIVE_COUNTS[intersects];
    for (unsigned id = 0; id < count; ++id) {
        Intersection intersection =
            intersect(box.get_box(), local, radius_, id);

        if (!intersection.overlap) continue;

        if (intersection.delta.length() <= closest.delta.length()) continue;

        closest = intersection;
    }

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

Box BoxCollider::get_bounding_box() const {
    glm::vec3 start = transform(transform_, box_.get_corner(0));

    glm::vec3 low = start;
    glm::vec3 high = start;

    for (unsigned id = 1; id < 8; ++id) {
        glm::vec3 point = transform(transform_, box_.get_corner(id));

        low.x = min(point.x, low.x);
        low.y = min(point.y, low.y);
        low.z = min(point.z, low.z);

        high.x = max(point.x, high.x);
        high.y = max(point.y, high.y);
        high.z = max(point.z, high.z);
    }

    return Box((high + low) / 2.0f, high - low);
}
