/**
 * @file static_mesh.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Static mesh scene component
 * @version 0.1
 * @date 2024-01-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LIB_LOGICS_COMPONENTS_VISUAL_STATIC_MESH_H
#define __LIB_LOGICS_COMPONENTS_VISUAL_STATIC_MESH_H

#include "graphics/objects/model.h"
#include "logics/scene_component.h"

struct StaticMesh : public SceneComponent {
    explicit StaticMesh(const Mesh& mesh, const Material& material);
    explicit StaticMesh(const Model& model);

    void spawn_self(Scene& scene) override;

    void set_transform(const glm::mat4& transform);
    const glm::mat4& get_transform() const {
        return model_.get_object_matrix();
    }

   private:
    Model model_;
};

#endif /* __LIB_LOGICS_COMPONENTS_VISUAL_STATIC_MESH_H */
