#include "strings.h"

static size_t get_length(const std::string_view& view, bool inclusive) {
    size_t end_index = 0;

    while (end_index < view.size()) {
        char current = view[end_index];

        if (inclusive) {
            if (current == '"') break;
            if (current == '\\') {
                ++end_index;
            }
        } else {
            if (!std::isalnum(current)) break;
        }

        ++end_index;
    }

    return end_index;
}

static std::string view_to_string(const std::string_view& view,
                                  size_t                  end_index) {
    std::string result;

    for (size_t id = 0; id < end_index; ++id) {
        char current = view[id];

        if (current != '\\') {
            result.push_back(current);
        } else if (id + 1 < end_index) {
            result.push_back(view[++id]);
        }
    }

    return result;
}

std::optional<Lexeme::LexemePtr> lexemes::String::try_construct(
    std::string_view& view) {
    if (view.size() == 0) return {};

    bool inclusive = false;

    if (view[0] == '"') {
        view.remove_prefix(1);
        inclusive = true;
    }

    size_t end_index = get_length(view, inclusive);

    if (!inclusive && end_index == 0) return {};

    std::string result = view_to_string(view, end_index);

    view.remove_prefix(inclusive ? end_index : end_index - 1);

    return std::make_shared<String>(result);
}

static const char COMPONENT_NAME_IDENTIFIER = '@';

std::optional<Lexeme::LexemePtr> lexemes::NamedComponent::try_construct(
    std::string_view& view) {
    if (view.length() == 0 || view[0] != COMPONENT_NAME_IDENTIFIER) return {};

    view.remove_prefix(1);

    size_t end_index = get_length(view, false);

    std::string result = view_to_string(view, end_index);

    view.remove_prefix(end_index - 1);

    return std::make_shared<NamedComponent>(result);
}
