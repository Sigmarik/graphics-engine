#include "static_mesh.h"

#include "logics/scene.h"

StaticMesh::StaticMesh(const Mesh& mesh, const Material& material)
    : model_(mesh, material) {}

StaticMesh::StaticMesh(const Model& model) : model_(model) {}

void StaticMesh::spawn_self(Scene& scene) {
    scene.get_renderer().track_object(model_);

    SceneComponent::spawn_self(scene);
}

void StaticMesh::set_transform(const glm::mat4& transform) {
    model_.set_object_matrix(transform);
}
