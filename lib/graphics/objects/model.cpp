#include "model.h"

#include "graphics/gl_debug.h"

const RenderPass Model::PASS = RP_INITIAL;

int Model::render(const RenderInput& input, const RenderBundle& bundle) const {
    return render_pure(input, bundle, object_matrix_);
}

int Model::render_pure(const RenderInput& input, const RenderBundle& bundle,
                       const glm::mat4& transform) const {
    if (input.pass != PASS) return 0;

    poll_gl_errors();

    material_.use();

    poll_gl_errors();

    bundle.bind_textures(material_.get_shader());

    mesh_->render(input.camera->get_matrix(), transform,
                  material_.get_shader());

    poll_gl_errors();

    return 1;
}
