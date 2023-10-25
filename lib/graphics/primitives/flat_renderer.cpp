#include "flat_renderer.h"

#include "graphics/libs.h"
#include "lib/graphics/gl_debug.h"

void FlatRenderer::render() {
    FlatRenderer& inst = instance();

    poll_gl_errors();

    glBindVertexArray(inst.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, inst.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inst.EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    poll_gl_errors();
}

FlatRenderer::FlatRenderer() {
    static const float coords[] = {-1.0f, 1.0f,  1.0f,  1.0f,
                                   1.0f,  -1.0f, -1.0f, -1.0f};
    static const unsigned indices[] = {0, 1, 2, 2, 3, 0};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    poll_gl_errors();
}
