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
#include "logger/logger.h"

/**
 * @brief A set of material uniforms, distinguished by their uniform names
 *
 */
struct UniformSet final {
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

    template <class TexType>
    void set(const char* name, const TexType* value);

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

    template <class TexType>
    struct TextureUniform final {
        TextureUniform(const TexType* source) : texture(source) {}
        TextureUniform(const TexType* source, unsigned tex_slot)
            : slot(tex_slot), texture(source) {}

        unsigned slot = 0;
        const TexType* texture;
    };

    struct UniformValue final {
        explicit UniformValue(int value);
        explicit UniformValue(float value);
        explicit UniformValue(const glm::vec2& value);
        explicit UniformValue(const glm::vec3& value);
        explicit UniformValue(const glm::vec4& value);
        explicit UniformValue(const glm::mat2& value);
        explicit UniformValue(const glm::mat3& value);
        explicit UniformValue(const glm::mat4& value);
        explicit UniformValue(TextureUniform<Texture> value);
        explicit UniformValue(TextureUniform<Texture3D> value);

        explicit UniformValue(const Texture* value)
            : UniformValue(TextureUniform<Texture>(value)) {}
        explicit UniformValue(const Texture3D* value)
            : UniformValue(TextureUniform<Texture3D>(value)) {}

        UniformValue(const UniformValue&) = default;
        UniformValue& operator=(const UniformValue&);

        void upload(const Shader& shader, const char* name) const;

       private:
        UniformSet::UniformType type_;

        union {
            int Int;
            float Float;
            glm::vec2 vec2;
            glm::vec3 vec3;
            glm::vec4 vec4;
            glm::mat2 mat2;
            glm::mat3 mat3;
            glm::mat4 mat4;
            TextureUniform<Texture> texture;
            TextureUniform<Texture3D> texture_3d;
        } value_;
    };

   private:
    std::unordered_map<std::string, UniformSet::UniformValue> data_{};

    unsigned slot_count_ = 0;
};

template <class T>
inline void UniformSet::set(const char* name, const T& value) {
    assert(name);

    auto found = data_.find(name);
    if (found != data_.end()) {
        found->second = UniformSet::UniformValue(value);
    } else {
        data_.insert({name, UniformSet::UniformValue(value)});
    }
}

static const unsigned TEXTURE_LIMIT = 7;

template <class TexType>
inline void UniformSet::set(const char* name, const TexType* value) {
    assert(name);

    auto found = data_.find(name);
    if (found != data_.end()) {
        found->second =
            UniformSet::UniformValue(TextureUniform<TexType>(value));
    } else {
        if (slot_count_ >= TEXTURE_LIMIT) {
            log_printf(WARNINGS, "warning",
                       "Exceeding texture slot count limit, %u out of %u "
                       "textures in a single uniform batch\n",
                       slot_count_ + 1, TEXTURE_LIMIT + 1);
        }

        data_.insert({name, UniformSet::UniformValue(
                                TextureUniform<TexType>(value, slot_count_))});
        ++slot_count_;
    }
}
