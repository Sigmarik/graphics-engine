/**
 * @file tree_builder.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Execution tree builder
 * @version 0.1
 * @date 2024-07-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <set>
#include <vector>

#include "logics/blueprints/scripts/script.h"

struct ParsedTree {
    // Root nodes of a script
    std::vector<std::shared_ptr<Script::Node>> roots{};

    // All nodes of a script
    std::vector<std::weak_ptr<Script::Node>> nodes{};

    // Initial update queue
    std::vector<std::weak_ptr<Script::Node>> queue{};

    // Script variables
    std::unordered_map<std::string, std::shared_ptr<Script::Node>> variables{};
};

/**
 * @brief Build an execution tree by a sequence of lexemes
 *
 * @param[in] lexemes lexeme sequence
 * @return ParsedTree
 */
ParsedTree build_exec_ast(const std::vector<Lexeme::LexemePtr>& lexemes);
