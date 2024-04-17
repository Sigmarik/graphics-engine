#include "scene.h"

#include "graphics/gl_debug.h"
#include "graphics/libs.h"
#include "graphics/primitives/flat_renderer.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"

void RenderManager::render(RenderBundle& bundle) const {
    if (viewpoint_ == nullptr) {
        log_printf(WARNINGS, "warning", "Render to undefined viewpoint\n");
        return;
    }

    bundle.reset();
    bundle.use();

    poll_gl_errors();

    glEnable(GL_DEPTH_TEST);
    RenderPass pass = RP_INITIAL;

    RenderInput stage_input = (RenderInput){.camera = viewpoint_, .pass = pass};
    render_everything(bundle, stage_input, false);

    glDisable(GL_DEPTH_TEST);

    bundle.swap_frames();
    bundle.use();

    pass = RP_DECAL;

    stage_input = (RenderInput){.camera = viewpoint_, .pass = pass};
    render_everything(bundle, stage_input);

    pass = RP_LIGHT;

    stage_input = (RenderInput){.camera = viewpoint_, .pass = pass};
    render_everything(bundle, stage_input);

    pass = RP_POSTPROCESSING;

    stage_input = (RenderInput){.camera = viewpoint_, .pass = pass};
    render_everything(bundle, stage_input);

    //* Copy rendered image to the screen

    static const Shader& identity_shader =
        *AssetManager::request<Shader>("assets/shaders/RB2SCR.shader.xml");

    RenderBundle::reset_to_screen();

    identity_shader.use();
    bundle.bind_textures(identity_shader);

    FlatRenderer::render();
}

void RenderManager::track_object(const Renderable& object) {
    assert(objects_.count(&object) == 0);

    objects_.insert(&object);
}

void RenderManager::untrack_object(const Renderable& object) {
    assert(objects_.count(&object) > 0);

    objects_.erase(&object);
}

void RenderManager::render_everything(RenderBundle& bundle,
                                      const RenderInput& input,
                                      bool swap_buffers) const {
    for (const Renderable* object : objects_) {
        assert(object);

        if (object->is_hidden()) continue;

        int rendered = object->render(input, bundle);

        if (rendered && swap_buffers) {
            bundle.swap_frames();
            bundle.use();
        }
    }
}
