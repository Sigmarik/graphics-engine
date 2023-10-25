#include "scene.h"

#include "graphics/gl_debug.h"
#include "graphics/libs.h"
#include "graphics/primitives/flat_renderer.h"
#include "managers/asset_manager.h"

void RenderManager::render(const RenderFrame& frame) const {
    glm::mat4 camera_matrix = viewpoint_.get_matrix();

    frame.use();

    frame.clear();

    poll_gl_errors();

    glEnable(GL_DEPTH_TEST);
    RenderPass pass = RP_INITIAL;

    RenderInput stage_input =
        (RenderInput){.camera_matrix = camera_matrix, .pass = pass};
    for (const Renderable* object : objects_) {
        if (object->is_hidden()) continue;

        object->render(stage_input, frame);
    }

    glDisable(GL_DEPTH_TEST);

    //* Copy rendered image to the screen

    static Shader& identity_shader =
        *AssetManager::request<Shader>("assets/shaders/RB2SCR.shader.xml");

    RenderFrame::reset_to_screen();

    identity_shader.use();
    frame.bind_textures(identity_shader);

    FlatRenderer::render();

    // TODO: Copy frame color channel to the canvas
}
