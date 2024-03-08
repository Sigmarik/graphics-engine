#include "uniform_set.h"

void UniformSet::upload(const Shader& shader) const {
    for (auto const& [key, val] : data_) {
        val.upload(shader, key.data());
    }
}

void UniformSet::UniformValue::upload(const Shader& shader,
                                      const char* name) const {
    switch (type_) {
        case UniformSet::UniformType::Int: {
            shader.set_uniform_int(name, value_.Int);
        } break;
        case UniformSet::UniformType::Float: {
            shader.set_uniform_float(name, value_.Float);
        } break;
        case UniformSet::UniformType::Vec2: {
            shader.set_uniform_vec2(name, value_.vec2);
        } break;
        case UniformSet::UniformType::Vec3: {
            shader.set_uniform_vec3(name, value_.vec3);
        } break;
        case UniformSet::UniformType::Vec4: {
            shader.set_uniform_vec4(name, value_.vec4);
        } break;
        case UniformSet::UniformType::Mat2: {
            shader.set_uniform_mat2(name, value_.mat2);
        } break;
        case UniformSet::UniformType::Mat3: {
            shader.set_uniform_mat3(name, value_.mat3);
        } break;
        case UniformSet::UniformType::Mat4: {
            shader.set_uniform_mat4(name, value_.mat4);
        } break;
        case UniformSet::UniformType::Texture: {
            assert(value_.texture);
            value_.texture->bind();
            shader.set_uniform_tex(name, *value_.texture);
        } break;
        case UniformSet::UniformType::Texture3D: {
            assert(value_.texture_3d);
            value_.texture_3d->bind();
            shader.set_uniform_tex3d(name, *value_.texture_3d);
        } break;
        default:
            break;
    }
}

UniformSet::UniformValue::UniformValue(int value)
    : type_(UniformSet::UniformType::Int), value_({.Int = value}) {}

UniformSet::UniformValue::UniformValue(float value)
    : type_(UniformSet::UniformType::Float), value_({.Float = value}) {}

UniformSet::UniformValue::UniformValue(const glm::vec2& value)
    : type_(UniformSet::UniformType::Vec2), value_({.vec2 = value}) {}

UniformSet::UniformValue::UniformValue(const glm::vec3& value)
    : type_(UniformSet::UniformType::Vec3), value_({.vec3 = value}) {}

UniformSet::UniformValue::UniformValue(const glm::vec4& value)
    : type_(UniformSet::UniformType::Vec4), value_({.vec4 = value}) {}

UniformSet::UniformValue::UniformValue(const glm::mat2& value)
    : type_(UniformSet::UniformType::Mat2), value_({.mat2 = value}) {}

UniformSet::UniformValue::UniformValue(const glm::mat3& value)
    : type_(UniformSet::UniformType::Mat3), value_({.mat3 = value}) {}

UniformSet::UniformValue::UniformValue(const glm::mat4& value)
    : type_(UniformSet::UniformType::Mat4), value_({.mat4 = value}) {}

UniformSet::UniformValue::UniformValue(const Texture& value)
    : type_(UniformSet::UniformType::Texture), value_({.texture = &value}) {}

UniformSet::UniformValue::UniformValue(const Texture3D& value)
    : type_(UniformSet::UniformType::Texture3D),
      value_({.texture_3d = &value}) {}
