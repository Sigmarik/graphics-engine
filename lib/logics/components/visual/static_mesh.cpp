#include "static_mesh.h"

#include "logics/scene.h"

StaticMesh::StaticMesh(const ComplexModel& model) : model_(model) {}

void StaticMesh::set_transform(const glm::mat4& transform) {
    model_.set_object_matrix(transform);
}

void StaticMesh::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    get_scene().get_renderer().track_object(model_);
}

void StaticMesh::end_play(EndPlayReason reason) {
    if (has_scene()) {
        get_scene().get_renderer().untrack_object(model_);
    }
}
