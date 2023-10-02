#include "matrix_stack.h"

MatrixStack::MatrixStack() : stack_() {
    stack_.push_back(glm::mat4());
    for (size_t id = 0; id < TS__END; ++id) {
        stages_[id] = (size_t)-1;
    }
}

void MatrixStack::push(const glm::mat4& matrix) {
    stack_.push_back(top() * matrix);
}

void MatrixStack::pop() {
    if (stack_.size() > 1) stack_.pop_back();
}

const glm::mat4& MatrixStack::at(TransformStage stage) const {
    if (stages_[stage] >= stack_.size()) {
        return top();
    }
    return stack_[stages_[stage]];
}

void MatrixStack::set_transform_stage(TransformStage stage) {
    stages_[stage] = stack_.size() - 1;
}
