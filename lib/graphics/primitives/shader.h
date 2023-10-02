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

#include "glad/gl.h"

struct Shader {
    Shader();
    Shader(const char* vsh_name, const char* fsh_name);
    ~Shader();

    void read(const char* vsh_name, const char* fsh_name);

    void use() const { glUseProgram(id_); }

    void setUniformFloat(const char* uniform, float value);
    void setUniformVec2(const char* uniform, const glm::vec2& vector);
    void setUniformVec3(const char* uniform, const glm::vec3& vector);
    void setUniformMat4(const char* uniform, const glm::mat4& matrix);

   private:
    GLuint id_ = 0;
};

#endif
