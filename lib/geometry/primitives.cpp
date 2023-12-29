#include "primitives.h"

#include <math.h>

bool intersect(const Box& alpha, const Box& beta) {
    glm::vec3 delta = alpha.get_center() - beta.get_center();
    glm::vec3 sum_size = alpha.get_size() + beta.get_size();
    return abs(delta.x) < sum_size.x && abs(delta.y) < sum_size.y &&
           abs(delta.z) < sum_size.z;
}

bool Box::contains(const Box& box) const {
    glm::vec3 delta = center_ - box.center_;
    glm::vec3 size_diff = size_ - box.size_;
    return abs(delta.x) < size_diff.x && abs(delta.y) < size_diff.y &&
           abs(delta.z) < size_diff.z;
}

glm::vec3 Box::get_corner(unsigned id) const {
    assert(id < 8);

    glm::vec3 multiplier = glm::vec3(-1.0, -1.0, -1.0);

    if (id & 0x1) multiplier.x = 1.0;
    if (id & 0x2) multiplier.y = 1.0;
    if (id & 0x4) multiplier.z = 1.0;

    return size_ * multiplier / 2.0f;
}

static Line BOX_EDGES[12] = {
    {.origin = glm::vec3(0.0, 0.0, 0.0), .direction = glm::vec3(1.0, 0.0, 0.0)},
    {.origin = glm::vec3(0.0, 1.0, 0.0), .direction = glm::vec3(1.0, 0.0, 0.0)},
    {.origin = glm::vec3(0.0, 1.0, 1.0), .direction = glm::vec3(1.0, 0.0, 0.0)},
    {.origin = glm::vec3(0.0, 0.0, 1.0), .direction = glm::vec3(1.0, 0.0, 0.0)},

    {.origin = glm::vec3(0.0, 0.0, 0.0), .direction = glm::vec3(0.0, 1.0, 0.0)},
    {.origin = glm::vec3(1.0, 0.0, 0.0), .direction = glm::vec3(0.0, 1.0, 0.0)},
    {.origin = glm::vec3(1.0, 0.0, 1.0), .direction = glm::vec3(0.0, 1.0, 0.0)},
    {.origin = glm::vec3(0.0, 0.0, 1.0), .direction = glm::vec3(0.0, 1.0, 0.0)},

    {.origin = glm::vec3(0.0, 0.0, 0.0), .direction = glm::vec3(0.0, 0.0, 1.0)},
    {.origin = glm::vec3(1.0, 0.0, 0.0), .direction = glm::vec3(0.0, 0.0, 1.0)},
    {.origin = glm::vec3(1.0, 1.0, 0.0), .direction = glm::vec3(0.0, 0.0, 1.0)},
    {.origin = glm::vec3(0.0, 1.0, 0.0), .direction = glm::vec3(0.0, 0.0, 1.0)},
};

Line Box::get_edge(unsigned id) const {
    assert(id < 12);

    Line edge = BOX_EDGES[id];
    edge.origin -= glm::vec3(0.5, 0.5, 0.5);
    edge.origin *= size_;

    return edge;
}

static glm::vec3 AXISES[3] = {
    glm::vec3(1.0, 0.0, 0.0),
    glm::vec3(0.0, 1.0, 0.0),
    glm::vec3(0.0, 0.0, 1.0),
};

Plane Box::get_face(unsigned id) const {
    assert(id < 6);

    glm::vec3 normal = AXISES[id / 2];
    if (id & 1) normal *= -1.0;

    return (Plane){
        .origin = normal * size_ / 2.0f,
        .normal = normal,
    };
}

bool intersect(const Sphere& alpha, const Sphere& beta) {
    return (alpha.get_center() - beta.get_center()).length() >
           alpha.get_radius() + beta.get_radius();
}
