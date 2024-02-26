#include "point_light.h"

#include <glm/gtc/matrix_transform.hpp>

#include "graphics/primitives/flat_renderer.h"
#include "managers/asset_manager.h"
#include "material.h"

int PointLight::render(const RenderInput& input,
                       const RenderBundle& bundle) const {
    static const Material& light_material = *AssetManager::request<Material>(
        "assets/materials/lights/point_light.material.xml");

    if (input.pass != RP_LIGHT) return 0;

    // return;

    glm::mat4 inverse_camera = glm::inverse(input.camera->get_matrix());
    glm::mat4 inverse_object = glm::inverse(object_matrix_);

    glm::vec4 object_pos = object_matrix_ * glm::vec4(0.0, 0.0, 0.0, 1.0);

    light_material.use();

    const Shader& shader = light_material.get_shader();

    bundle.bind_textures(shader);

    shader.set_uniform_mat4("camera_inv", inverse_camera);
    shader.set_uniform_mat4("object_inv", inverse_object);
    shader.set_uniform_vec3("light_color", color_);
    shader.set_uniform_float("spread", spread_);
    shader.set_uniform_float("radius", radius_);
    shader.set_uniform_vec3(
        "object_pos", glm::vec3(object_pos.x, object_pos.y, object_pos.z));
    shader.set_uniform_vec3("camera_pos", input.camera->get_position());

    FlatRenderer::render();

    return 1;
}
