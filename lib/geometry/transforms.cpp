#include "transforms.h"

#include <glm/geometric.hpp>

glm::vec3 reflect_plane(const glm::vec3& vector, const glm::vec3& normal) {
    return vector -
           normal * (float)(glm::dot(vector, normal) * 2.0 /
                            (glm::length(normal) * glm::length(normal)));
}

glm::vec3 to_point(const glm::mat4& transform) {
    const glm::vec4& shift = transform[3];
    return glm::vec3(shift.x, shift.y, shift.z) / shift.w;
}
