/**
 * @file complex_model.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Complex model class
 * @version 0.1
 * @date 2024-03-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <unordered_map>

#include "model.h"

/**
 * @brief A model composed of multiple parts (models)
 *
 */
struct ComplexModel final : public Renderable {
    ComplexModel() = default;

    /**
     * @brief Construct a new ComplexModel object from a single model
     *
     * @param[in] base
     * @param[in] name model label ("base" if not specified)
     */
    ComplexModel(const Model& base,
                 const char* name = "base");  // Non-explicit

    int render(const RenderInput& input,
               const RenderBundle& bundle) const override;

    /**
     * @brief Register part of the model. Its current transform will be
     * interpreted as relative to the group model origin.
     *
     * @param[in] model part to add
     * @param[in] label name of the part (will be randomly generated if not
     * specified)
     */
    void add_part(const Model& model, const char* label = nullptr);

    Model& get_part(const char* name) { return parts_.find(name)->second; }
    const Model& get_part(const char* name) const {
        return parts_.find(name)->second;
    }

    /**
     * @brief Get some part of the model (useful for single-part complex models)
     *
     * @return Model&
     */
    Model& get_base() { return parts_.begin()->second; }
    const Model& get_base() const { return parts_.begin()->second; }

    void for_each_part(
        std::function<void(Model&, const std::string&)> function);
    void for_each_part(
        std::function<void(const Model&, const std::string&)> function) const;

   private:
    std::unordered_map<std::string, Model> parts_{};
};
