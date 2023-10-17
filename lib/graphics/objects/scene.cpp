#include "scene.h"

#include "graphics/primitives/framebuffer.h"

void RenderManager::render() const {
    glm::mat4 camera_matrix = viewpoint_.get_matrix();
    for (Renderable* renderable : objects_) {
        renderable->render((RenderInput){.camera_matrix = camera_matrix,
                                         .pass = RP_GB_ALBEDO});
    }
}
