#include "lexemizer.h"

#include "logger/logger.h"

static void remove_whitespace(std::string_view& view) {
    bool has_changes = false;

    do {
        has_changes = false;

        while (!view.empty() && std::isspace(view[0])) {
            has_changes = true;
            view.remove_prefix(1);
        }

        if (view.starts_with("//")) {
            has_changes = true;
            while (!view.empty() && view[0] != '\n') view.remove_prefix(1);
        }
    } while (has_changes);
}

static std::optional<Lexeme::LexemePtr> lexeme_lookup(std::string_view& view) {
    for (auto probe : LEXEME_CONSTRUCTORS) {
        auto lexeme = probe(view);

        if (lexeme) return lexeme;
    }

    return {};
}

static void update_position(const char* start, const std::string_view& view,
                            size_t& line, size_t& column, size_t& index) {
    size_t new_index = uintptr_t(view.data()) - uintptr_t(start);

    for (; index < new_index; ++index) {
        if (start[index] == '\n') {
            column = 0;
            ++line;
        } else {
            ++column;
        }
    }
}

std::vector<Lexeme::LexemePtr> lexify(const std::string& text) {
    std::vector<Lexeme::LexemePtr> result;

    std::string_view view(text);

    size_t current_line = 0, current_column = 0, current_index = 0;

    remove_whitespace(view);

    while (!view.empty()) {
        update_position(text.c_str(), view, current_line, current_column,
                        current_index);
        auto lexeme = lexeme_lookup(view);

        if (!lexeme) {
            log_printf(
                ERROR_REPORTS, "error",
                "Could not distinguish lexeme at line %lu, column %lu.\n",
                current_line, current_column);
            break;
        }

        lexeme.value()->assign_coords(current_line, current_column);

        result.push_back(lexeme.value());
    }

    return result;
}
