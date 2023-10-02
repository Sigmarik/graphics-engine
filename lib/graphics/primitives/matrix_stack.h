/**
 * @file matrix_stack.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Matrix stack data structure
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include <glm/mat4x4.hpp>
#include <vector>

/**
 * @brief Stage of object transformation
 *
 */
enum TransformStage {
    TS_PROJECT,
    TS_CAM_SPACE,
    TS_WORLD,
    TS_OBJECT,
    TS__END,
};

/**
 * @brief Stack of 4x4 matrices used to describe object transformations
 *
 */
struct MatrixStack {
    MatrixStack();

    void push(const glm::mat4& matrix);
    void pop();

    const glm::mat4& at(TransformStage stage) const;
    void set_transform_stage(TransformStage stage);

    const glm::mat4& top() const { return stack_[stack_.size() - 1]; }

   private:
    std::vector<glm::mat4> stack_;

    size_t stages_[TS__END];
};

#endif
