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

bool intersect(const Sphere& alpha, const Sphere& beta) {
    return (alpha.get_center() - beta.get_center()).length() >
           alpha.get_radius() + beta.get_radius();
}
