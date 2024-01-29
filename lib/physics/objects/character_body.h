/**
 * @file character_body.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Character body class
 * @version 0.1
 * @date 2024-01-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LIB_PHYSICS_OBJECTS_CHARACTER_BODY_H
#define __LIB_PHYSICS_OBJECTS_CHARACTER_BODY_H

#include "physics/collider.h"
#include "physics/phys_object.h"

static const unsigned BODY_INTERP_LEVEL = 3;

struct CharacterBody : public PhysObject {
    CharacterBody(const glm::vec3& feet_pos, double width, double height);

    void tick(const LevelGeometry& level, double delta_time) override;

    glm::vec3 get_position() const override {
        return colliders_[0].get_position() +
               glm::vec3(0.0, -width_ / 2.0, 0.0);
    }
    void set_position(const glm::vec3& position);

    glm::vec3 get_rotation() const override { return glm::vec3(0.0); }

    glm::vec3 get_velocity() const { return velocity_; }

    glm::vec3 get_interp_pos(double time) const override {
        return get_position() + velocity_ * (float)time;
    }

    void move(const glm::vec3& input) { input_ = input; }

    void jump(double strength, bool air_jump = false);

    bool is_airborne() { return airborne_; }

    double get_width() const { return width_; }
    double get_height() const { return height_; }

    void set_height(double height) { height_ = height; }

    double get_stepup_slope() const { return stepup_slope_; }
    void set_stepup_slope(double slope) { stepup_slope_ = slope; }

    double get_air_control_multiplier() const {
        return air_control_multiplier_;
    }
    void set_air_control_multiplier(double multiplier) {
        air_control_multiplier_ = multiplier;
    }

   private:
    SphereCollider colliders_[BODY_INTERP_LEVEL] = {};

    double width_;
    double height_;

    double stepup_slope_ = 1.2;

    glm::vec3 velocity_ = glm::vec3(0.0, 0.0, 0.0);

    glm::vec3 input_ = glm::vec3(0.0);
    double acceleration_ = 100.0;

    bool airborne_ = true;
    double air_control_multiplier_ = 0.2;
    double movement_speed_ = 10.0;
};

#endif /* __LIB_PHYSICS_OBJECTS_CHARACTER_BODY_H */