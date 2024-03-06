/**
 * @file uniform_set.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Uniform set class
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <unordered_map>

#include "graphics/primitives/shader.h"
#include "graphics/primitives/texture.h"

/**
 * @brief A set of material uniforms, distinguished by their uniform names
 *
 */
struct UniformSet {
    /**
     * @brief Upload the uniform values to the GPU
     *
     * @param[in] shader
     */
    void upload(const Shader& shader) const;

    /**
     * @brief Set the value of the uniform
     *
     * @tparam T value type
     * @param[in] name name of the uniform
     * @param[in] value value of the uniform
     */
    template <class T>
    void set(const char* name, const T& value);

   private:
    enum class UniformType {
        Int,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat2,
        Mat3,
        Mat4,
        Texture,
        Texture3D,
    };

    struct UniformValue {
        explicit UniformValue(int value);
        explicit UniformValue(float value);
        explicit UniformValue(const glm::vec2& value);
        explicit UniformValue(const glm::vec3& value);
        explicit UniformValue(const glm::vec4& value);
        explicit UniformValue(const glm::mat2& value);
        explicit UniformValue(const glm::mat3& value);
        explicit UniformValue(const glm::mat4& value);
        explicit UniformValue(const Texture& value);
        explicit UniformValue(const Texture3D& value);

        void upload(const Shader& shader, const char* name) const;

       private:
        const UniformSet::UniformType type_;

        union {
            int Int;
            float Float;
            glm::vec2 vec2;
            glm::vec3 vec3;
            glm::vec4 vec4;
            glm::mat2 mat2;
            glm::mat3 mat3;
            glm::mat4 mat4;
            const Texture* texture;
            const Texture3D* texture_3d;
        } value_;
    };

    std::unordered_map<std::string, UniformSet::UniformValue> data_;
};

template <class T>
inline void UniformSet::set(const char* name, const T& value) {
    data_[name] = UniformSet::UniformValue(value);
}
