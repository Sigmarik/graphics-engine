#include "model.h"

#include "graphics/gl_debug.h"

void Model::render(const RenderInput& input, const RenderFrame& bundle) const {
    if (input.pass != RP_INITIAL) return;

    poll_gl_errors();

    material_->use();

    poll_gl_errors();

    bundle.bind_textures(material_->get_shader());

    mesh_->render(input.camera_matrix, object_matrix_, material_->get_shader());

    poll_gl_errors();
}
