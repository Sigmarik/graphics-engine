/**
 * @file flat_renderer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Flat renderer structure
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef FLAT_RENDERER_H
#define FLAT_RENDERER_H

struct FlatRenderer {
    static void render();

   private:
    static FlatRenderer& instance() {
        static FlatRenderer renderer;

        return renderer;
    }

    FlatRenderer();
    ~FlatRenderer() = default;
    FlatRenderer(const FlatRenderer& renderer) = default;
    FlatRenderer& operator=(const FlatRenderer& renderer) = default;

    unsigned VBO = 0, VAO = 0, EBO = 0;
};

#endif
