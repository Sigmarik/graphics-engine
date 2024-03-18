#include "complex_model.h"

#include "logger/logger.h"
#include "managers/asset_manager.h"

ComplexModel::ComplexModel(const Model& base, const char* name) {
    parts_.insert({name, base});
}

int ComplexModel::render(const RenderInput& input,
                         const RenderBundle& bundle) const {
    if (input.pass != Model::PASS) return 0;

    for (const auto& [key, part] : parts_) {
        glm::mat4 part_transform = object_matrix_ * part.get_object_matrix();

        part.render_pure(input, bundle, part_transform);
    }

    return 1;
}

static const size_t NAME_LENGTH = 32;

void ComplexModel::add_part(const Model& model, const char* label) {
    if (label == nullptr) {
        static char generated_name[NAME_LENGTH + 1] = "";
        for (size_t id = 0; id < NAME_LENGTH; ++id) {
            generated_name[id] = 'a' + (char)(rand() % ('z' - 'a' + 1));
        }
        parts_.insert({generated_name, model});
    } else {
        parts_.insert({label, model});
    }
}

void ComplexModel::for_each_part(
    std::function<void(Model&, const std::string&)> function) {
    for (auto& [key, part] : parts_) {
        function(part, key);
    }
}

void ComplexModel::for_each_part(
    std::function<void(const Model&, const std::string&)> function) const {
    for (const auto& [key, part] : parts_) {
        function(part, key);
    }
}
