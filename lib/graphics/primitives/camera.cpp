#include "camera.h"

#include <glm/ext/matrix_clip_space.hpp>

static const float NEAR_PLANE = 0.01f;
static const float FAR_PLANE = 200.0f;

glm::mat4 Camera::get_matrix() {
    glm::mat4 projection =
        glm::perspective(fov_, ratio_, NEAR_PLANE, FAR_PLANE);
    glm::mat4 space(glm::vec4(right_, 0.0), glm::vec4(up_, 0.0),
                    glm::vec4(forward_, 0.0), glm::vec4(position_, 1.0));
    space = glm::inverse(space);
    return projection * space;
}
