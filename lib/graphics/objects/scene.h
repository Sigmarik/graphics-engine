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

#ifndef __LIB_GRAPHICS_OBJECTS_SCENE_H
#define __LIB_GRAPHICS_OBJECTS_SCENE_H

#include <glm/mat4x4.hpp>
#include <set>

#include "graphics/primitives/camera.h"
#include "graphics/primitives/render_frame.h"

enum RenderPass {
    RP_INITIAL,
    RP_DECAL,
    RP_LIGHT,
    RP_POSTPROCESSING,
    _RP_END,
};

/**
 * @brief Render parameters passed to each individual object on the scene.
 *
 */
struct RenderInput {
    Camera* camera;
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
     * @return 1 if successful
     */
    virtual int render(const RenderInput& input,
                       const RenderBundle& bundle) const = 0;

    const glm::mat4& get_object_matrix() const { return object_matrix_; }
    void set_object_matrix(const glm::mat4& matrix) { object_matrix_ = matrix; }

    bool is_hidden() const { return hidden_; }
    void set_hidden(bool hidden) { hidden_ = hidden; }

    virtual ~Renderable() = default;

   protected:
    glm::mat4 object_matrix_ = glm::mat4(1.0);
    bool hidden_ = false;
};

struct RenderManager {
    RenderManager() = default;
    RenderManager(const RenderManager& instance) = default;
    ~RenderManager() = default;

    RenderManager& operator=(const RenderManager& instance) = default;

    void render(RenderBundle& bundle) const;

    void track_object(const Renderable& object) { objects_.insert(&object); }
    void untrack_object(const Renderable& object) { objects_.erase(&object); }

    void set_viewpoint(Camera* camera) { viewpoint_ = camera; }
    Camera* get_viewpoint() const { return viewpoint_; }

   private:
    void render_everything(RenderBundle& bundle, const RenderInput& input,
                           bool swap_buffers = true) const;

    Camera* viewpoint_ = nullptr;

    std::set<const Renderable*> objects_{};
};

#endif /* __LIB_GRAPHICS_OBJECTS_SCENE_H */