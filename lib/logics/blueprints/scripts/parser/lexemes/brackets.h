/**
 * @file brackets.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/blueprints/scripts/parser/lexeme.h"

namespace lexemes {

using BracketRoundOp = StrictLexeme<"(">;
using BracketRoundCl = StrictLexeme<")">;

using BracketSquareOp = StrictLexeme<"[">;
using BracketSquareCl = StrictLexeme<"]">;

using BracketCurlyOp = StrictLexeme<"{">;
using BracketCurlyCl = StrictLexeme<"}">;

};  // namespace lexemes