/**
 * @file lexemizer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Text lexification tools
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "lexemes.h"

std::vector<Lexeme::LexemePtr> lexify(const std::string& text);
