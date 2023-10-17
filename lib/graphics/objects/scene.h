/**
 * @file scene.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Visual scene
 * @version 0.1
 * @date 2023-10-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SCENE_H
#define SCENE_H

#include <glm/mat4x4.hpp>
#include <set>

enum RenderPass {
    RP_OPAQUE,
    RP_TRANSLUCENT,
    RP_LIGHT_MAP,
    RP_GB_POS,
    RP_GB_NORMAL,
    RP_GB_ALBEDO,
    _RP_END,
};

/**
 * @brief Render parameters passed to each individual object on the scene.
 *
 */
struct RenderInput {
    glm::mat4 camera_matrix;
    RenderPass pass;
};

/**
 * @brief Renderable object that stores its position and hiding state and can be
 * draw to the framebuffer
 *
 */
struct Renderable {
    /**
     * @brief Render object to the currently bound framebuffer with given render
     * parameters.
     *
     * @param input
     */
    virtual void render(RenderInput input) const = 0;

    const glm::mat4& get_object_matrix() const { return object_matrix_; }
    void set_object_matrix(const glm::mat4& matrix) { object_matrix_ = matrix; }

    bool is_hidden() const { return hidden_; }
    void set_hidden(bool hidden) { hidden_ = hidden; }

   protected:
    glm::mat4 object_matrix_ = glm::mat4(1.0);
    bool hidden_ = false;
};

struct RenderManager {
    RenderManager(const Camera& viewpoint) : viewpoint_(viewpoint) {}

    void render() const;

    void set_viewpoint(const Camera& viewpoint) { viewpoint_ = viewpoint; }
    const Camera& get_viewpoint() const { return viewpoint_; }

    void track_object(const Renderable& object) { objects_.insert(&object); }
    void untrack_object(const Renderable& object) { objects_.erase(&object); }

   private:
    const Camera& viewpoint_;

    std::set<const Renderable*> objects_;
};

#endif
