/**
 * @file lexemes.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief All lexeme types combined into a single file
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <vector>

#include "lexeme.h"
#include "lexemes/brackets.h"
#include "lexemes/operators.h"
#include "lexemes/strings.h"

extern const std::vector<Lexeme::Info> LEXEME_INFO_TABLE;
