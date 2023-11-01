#include "postprocessor.h"

#include "graphics/primitives/flat_renderer.h"
#include "managers/asset_manager.h"
#include "material.h"

int Postprocessor::render(const RenderInput& input,
                          const RenderBundle& bundle) const {
    if (input.pass != RP_POSTPROCESSING) return 0;

    material_->use();
    bundle.bind_textures(material_->get_shader());

    FlatRenderer::render();

    return 1;
}