/**
 * @file shader.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Shader program
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "graphics/libs.h"
#include "graphics/primitives/texture.h"

struct Shader {
    Shader();
    Shader(const char* vsh_name, const char* fsh_name);

    void read(const char* vsh_name, const char* fsh_name);

    void use() const { glUseProgram(id_); }

    void set_uniform_float(const char* uniform, float value);
    void set_uniform_vec2(const char* uniform, const glm::vec2& vector);
    void set_uniform_vec3(const char* uniform, const glm::vec3& vector);
    void set_uniform_mat4(const char* uniform, const glm::mat4& matrix);
    void set_uniform_img(const char* uniform, const Texture& texture);

   private:
    GLuint id_ = 0;
};

#endif
