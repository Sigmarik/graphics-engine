/**
 * @file model.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Model structure
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "graphics/primitives/mesh.h"
#include "material.h"
#include "scene.h"

struct Model : public Renderable {
    Model(const Mesh& mesh, const Material& material)
        : mesh_(&mesh), material_(material) {}
    ~Model() = default;
    Model(const Model& model) = default;
    Model& operator=(const Model& model) = default;

    int render(const RenderInput& input,
               const RenderBundle& bundle) const override;

    const Mesh& get_mesh() const { return *mesh_; }
    void set_mesh(const Mesh& mesh) { mesh_ = &mesh; }

    const Material& get_material() const { return material_; }
    Material& get_material() { return material_; }
    void set_material(const Material& material) { material_ = material; }

    friend struct ComplexModel;

   private:
    int render_pure(const RenderInput& input, const RenderBundle& bundle,
                    const glm::mat4& transform) const;

    static const RenderPass PASS;
    const Mesh* mesh_;
    Material material_;
};
