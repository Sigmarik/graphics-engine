#include "scene.h"

#include "graphics/gl_debug.h"
#include "graphics/libs.h"
#include "graphics/primitives/flat_renderer.h"
#include "managers/asset_manager.h"

void RenderManager::render(RenderBundle& bundle) const {
    bundle.reset();
    bundle.use();

    poll_gl_errors();

    glEnable(GL_DEPTH_TEST);
    RenderPass pass = RP_INITIAL;

    RenderInput stage_input =
        (RenderInput){.camera = &viewpoint_, .pass = pass};
    for (const Renderable* object : objects_) {
        if (object->is_hidden()) continue;

        object->render(stage_input, bundle);
    }

    glDisable(GL_DEPTH_TEST);

    bundle.swap_frames();
    bundle.use();

    pass = RP_LIGHT;

    stage_input = (RenderInput){.camera = &viewpoint_, .pass = pass};
    for (const Renderable* object : objects_) {
        if (object->is_hidden()) continue;

        int rendered = object->render(stage_input, bundle);

        if (rendered) {
            bundle.swap_frames();
            bundle.use();
        }
    }

    pass = RP_POSTPROCESSING;

    stage_input = (RenderInput){.camera = &viewpoint_, .pass = pass};
    for (const Renderable* object : objects_) {
        if (object->is_hidden()) continue;

        int rendered = object->render(stage_input, bundle);

        if (rendered) {
            bundle.swap_frames();
            bundle.use();
        }
    }

    //* Copy rendered image to the screen

    static Shader& identity_shader =
        *AssetManager::request<Shader>("assets/shaders/RB2SCR.shader.xml");

    RenderBundle::reset_to_screen();

    identity_shader.use();
    bundle.bind_textures(identity_shader);

    FlatRenderer::render();
}
