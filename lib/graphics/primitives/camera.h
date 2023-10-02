/**
 * @file camera.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Camera object
 * @version 0.1
 * @date 2023-10-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

enum CameraType {
    CAM_PERSPECTIVE,
    CAM_ORTHOGRAPHIC,
};

struct Camera {
    Camera() = default;
    explicit Camera(float aspect_ratio) : ratio_(aspect_ratio) {}

    void set_position(const glm::vec3 pos) { position_ = pos; }
    const glm::vec3& get_position() const { return position_; }
    const glm::vec3& get_forward() const { return forward_; }
    const glm::vec3& get_up() const { return up_; }
    const glm::vec3& get_right() const { return right_; }

    void direct(const glm::vec3 forward, const glm::vec3 up) {
        forward_ = glm::normalize(forward);
        right_ = glm::normalize(glm::cross(up, forward_));
        up_ = glm::normalize(glm::cross(forward_, right_));
    }

    glm::mat4 get_matrix();

    void set_fov(float fov) { fov_ = fov; }
    float get_fov() const { return fov_; }

    void set_type(CameraType type) { type_ = type; }
    CameraType get_type() const { return type_; }

   private:
    glm::vec3 position_ = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 forward_ = glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 up_ = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 right_ = glm::vec3(-1.0, 0.0, 0.0);
    float ratio_ = 1.0;
    float fov_ = 90.0;
    CameraType type_ = CAM_PERSPECTIVE;
};

#endif
