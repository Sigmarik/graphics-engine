#include "static_mesh.h"

#include "logics/scene.h"

StaticMesh::StaticMesh(Scene& scene, const Mesh& mesh, const Material& material)
    : SceneComponent(scene), model_(mesh, material) {
    scene.get_renderer().track_object(model_);
}

void StaticMesh::set_transform(const glm::mat4& transform) {
    transform_ = transform;
    model_.set_object_matrix(transform_);
}
