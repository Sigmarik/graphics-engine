/**
 * @file vbo.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief VertexArrayObject
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VAO_HPP
#define VAO_HPP

#include "glad/gl.h"
#include "vertex.hpp"

struct VAO {
    VAO() : id(0) { glGenVertexArrays(1, &id); }
    ~VAO() { glDeleteVertexArrays(1, &id); }

    void bind() const { glBindVertexArray(id); }
    void unbind() const { glBindVertexArray(0); }

   private:
    GLuint id;
};

#endif
