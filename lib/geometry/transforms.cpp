#include "transforms.h"

#include <glm/geometric.hpp>

glm::vec3 reflect_plane(const glm::vec3& vector, const glm::vec3& normal) {
    return vector - normal * (float)(glm::dot(vector, normal) * 2.0 /
                                     (normal.length() * normal.length()));
}
