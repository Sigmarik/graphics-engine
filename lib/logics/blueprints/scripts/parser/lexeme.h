/**
 * @file lexeme.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Lexeme class
 * @version 0.1
 * @date 2024-07-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <string.h>

#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "templates/strings.h"

struct Lexeme {
    using LexemePtr = std::shared_ptr<Lexeme>;

    using Constructor =
        std::function<std::optional<LexemePtr>(std::string_view&)>;

    static std::optional<LexemePtr> try_construct(std::string_view&) {
        return {};
    }
};

template <StringLiteral... Values>
struct StrictLexeme : public Lexeme {
    static std::optional<LexemePtr> try_construct(std::string_view& view) {
        return (compare_and_shift(view, Values.value) || ...)
                   ? std::make_shared<StrictLexeme<Values...>>()
                   : std::optional<LexemePtr>();
    }

   private:
    static bool compare_and_shift(std::string_view& view,
                                  const char* sequence) {
        if (!view.starts_with(sequence)) return false;

        view.remove_prefix(strlen(sequence));

        return true;
    }
};
