#include "decal.h"

#include "graphics/primitives/flat_renderer.h"
#include "managers/world_timer.h"

Decal::Decal(const Material& material, double time_to_live)
    : material_(&material),
      time_to_live_(time_to_live),
      spawn_time_(WorldTimer::get_time()) {}

int Decal::render(const RenderInput& input, const RenderBundle& bundle) const {
    if (input.pass != RP_DECAL) return 0;

    glm::mat4 inverse_camera = glm::inverse(input.camera->get_matrix());
    glm::mat4 inverse_object = glm::inverse(object_matrix_);

    glm::vec4 object_pos = object_matrix_ * glm::vec4(0.0, 0.0, 0.0, 1.0);

    material_->use();

    Shader& shader = material_->get_shader();

    bundle.bind_textures(shader);

    shader.set_uniform_mat4("object_inv", inverse_object);
    shader.set_uniform_mat4("camera_inv", inverse_camera);
    shader.set_uniform_vec3("object_pos", object_pos);

    float decay_percentage =
        1.0f - ((float)(WorldTimer::get_time() - spawn_time_) / 100000.0) /
                   (float)time_to_live_;

    if (decay_percentage < 0.0) {
        return 0;
    }

    // printf("Decay: %g\n", decay_percentage);

    shader.set_uniform_float("decay", decay_percentage);
    shader.set_uniform_float("time_to_live", (float)time_to_live_);

    FlatRenderer::render();

    return 1;
}

void Decal::reset() { spawn_time_ = WorldTimer::get_time(); }
