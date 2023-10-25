#include "ambient_light.h"

#include "graphics/primitives/flat_renderer.h"
#include "graphics/primitives/shader.h"
#include "managers/asset_manager.h"

void AmbientLight::render(const RenderInput& input,
                          const RenderBundle& bundle) const {
    static Shader& ambient_shader = *AssetManager::request<Shader>(
        "assets/shaders/ambient_light.shader.xml");

    if (input.pass != RP_LIGHT) return;

    ambient_shader.use();
    bundle.bind_textures(ambient_shader);

    ambient_shader.set_uniform_vec3("light_color", color_);

    FlatRenderer::render();
}