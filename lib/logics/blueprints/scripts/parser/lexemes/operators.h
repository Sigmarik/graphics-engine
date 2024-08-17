/**
 * @file operators.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Operator lexemes
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/blueprints/scripts/parser/lexeme.h"

namespace lexemes {

using AssignmentLeft  = StrictLexeme<"::", "->">;
using AssignmentRight = StrictLexeme<"<-">;
using Access          = StrictLexeme<".">;

using Equal            = StrictLexeme<"==">;
using NotEqual         = StrictLexeme<"!=">;
using Or               = StrictLexeme<"||", "or">;
using And              = StrictLexeme<"&&", "and">;
using Not              = StrictLexeme<"!", "not">;
using ConditionalLeft  = StrictLexeme<"?">;
using ConditionalRight = StrictLexeme<":">;

using MacroAssignment = StrictLexeme<"=">;

using Less         = StrictLexeme<"<">;
using Greater      = StrictLexeme<">">;
using LessEq       = StrictLexeme<"<=">;
using GreaterEq    = StrictLexeme<">=">;
using AlphaLess    = StrictLexeme<"#<">;
using AlphaGreater = StrictLexeme<"#>">;

using Plus     = StrictLexeme<"+">;
using Minus    = StrictLexeme<"-">;
using Divide   = StrictLexeme<"/">;
using Multiply = StrictLexeme<"*">;
using Power    = StrictLexeme<"^">;

using Sine        = StrictLexeme<"sin">;
using Cosine      = StrictLexeme<"cos">;
using Ln          = StrictLexeme<"ln", "loge">;
using Log2        = StrictLexeme<"log2">;
using Log10       = StrictLexeme<"log10">;
using Absolute    = StrictLexeme<"abs">;
using Sign        = StrictLexeme<"sign">;
using Length      = StrictLexeme<"len">;
using SkipInvalid = StrictLexeme<"skip_invalid">;

using Valid = StrictLexeme<"valid">;

};  // namespace lexemes
