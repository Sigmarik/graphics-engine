/**
 * @file script.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Level script object
 * @version 0.1
 * @date 2024-06-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/blueprints/component_factory.hpp"
#include "logics/scene_component.h"
#include "parser/lexeme.h"

struct Script {
    struct Node;

    Script(const std::string& string);

    Script(const Script& other) : lexemes_(other.lexemes_) {}
    Script& operator=(const Script& other) {
        lexemes_ = other.lexemes_;
        nodes_.clear();
        return *this;
    }

    Script(Script&&) = default;
    Script& operator=(Script&&) = default;

    void assemble(Scene& scene, const SubcomponentNameMap& name_map);

   private:
    std::vector<Lexeme::LexemePtr> lexemes_{};

    std::vector<Node> nodes_{};
};

#include "node.h"
