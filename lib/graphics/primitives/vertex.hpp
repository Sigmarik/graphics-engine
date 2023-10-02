/**
 * @file vertex.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Graphical vertex data struct
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glad/gl.h>

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

/**
 * @brief Graphical vertex
 *
 */
struct Vertex final {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    Vertex apply(const glm::mat4& matrix) const {
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(matrix, scale, rotation, translation, skew, perspective);

        glm::vec3 pos = matrix * glm::vec4(position, 1.0);
        glm::vec3 norm =
            rotation *
            (glm::vec3(1.0 / scale.x, 1.0 / scale.y, 1.0 / scale.z) * normal);
        return (Vertex){.position = pos, .normal = norm, .uv = uv};
    }

    static void configure() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)(sizeof(float) * 6));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
};

#endif
