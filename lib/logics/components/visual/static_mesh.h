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

#pragma once

#include "graphics/objects/complex_model.h"
#include "logics/scene_component.h"

struct StaticMesh : public SceneComponent {
    explicit StaticMesh(const ComplexModel& model);

    void set_transform(const glm::mat4& transform);
    const glm::mat4& get_transform() const {
        return model_.get_object_matrix();
    }

    ComplexModel& get_model() { return model_; }
    const ComplexModel& get_model() const { return model_; }
    void set_model(const Model& model) { model_ = model; }

   protected:
    void begin_play(Scene& scene) override;

   private:
    ComplexModel model_;
};
