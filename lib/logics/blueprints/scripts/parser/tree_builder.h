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

#include <vector>

#include "logics/blueprints/scripts/script.h"

/**
 * @brief Build an execution tree by a sequence of lexemes
 *
 * @param[in] lexemes lexeme sequence
 * @param[out] update_queue nodes which require an update after tree
 * construction
 * @return std::vector<Abstract<Script::Node>> roots of a tree
 */
std::vector<Abstract<Script::Node>> build_exec_ast(
    const std::vector<Lexeme::LexemePtr>& lexemes,
    std::vector<Script::Node*>& update_queue);
