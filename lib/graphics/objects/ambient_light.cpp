#include "ambient_light.h"

#include "graphics/primitives/flat_renderer.h"
#include "managers/asset_manager.h"
#include "material.h"

int AmbientLight::render(const RenderInput& input,
                         const RenderBundle& bundle) const {
    static Material& ambient_material = *AssetManager::request<Material>(
        "assets/materials/lights/ambient_light.material.xml");

    if (input.pass != RP_LIGHT) return 0;

    ambient_material.use();
    bundle.bind_textures(ambient_material.get_shader());
    ambient_material.get_shader().set_uniform_vec3("light_color", color_);

    FlatRenderer::render();

    return 1;
}