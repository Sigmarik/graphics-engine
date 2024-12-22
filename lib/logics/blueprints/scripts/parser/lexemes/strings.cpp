#include "strings.h"

#include <iostream>

static size_t get_length(const std::string_view& view, bool inclusive) {
    size_t end_index = 0;

    bool only_numeric = true;

    while (end_index < view.size()) {
        char current = view[end_index];

        if (inclusive) {
            if (current == '"') break;
            if (current == '\\') {
                ++end_index;
            }
        } else {
            if (!std::isalnum(current) && current != '_' &&
                !(only_numeric && current == '.'))
                break;
            if (!std::isdigit(current)) only_numeric = false;
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

std::optional<Lexeme::LexemePtr> lexemes::String::
    try_construct(std::string_view& view) {
    if (view.size() == 0) return {};

    bool inclusive = false;

    if (view[0] == '"') {
        view.remove_prefix(1);
        inclusive = true;
    }

    size_t end_index = get_length(view, inclusive);

    if (!inclusive && end_index == 0) return {};

    std::string result = view_to_string(view, end_index);

    view.remove_prefix(inclusive ? end_index + 1 : end_index);

    return LexemePtr(new String(result, inclusive));
}

Lexeme::LexemePtr lexemes::String::construct(const std::string& string) {
    bool exact = false;

    std::string copy = string;
    if (copy.back() == (char)(-1)) {
        exact = true;
        copy.pop_back();
    }

    return LexemePtr(new String(copy, exact));
}

std::string lexemes::String::dump() const {
    std::stringstream stream;
    stream << "STRING " << value_;
    return stream.str();
}

static const char COMPONENT_NAME_IDENTIFIER = '@';

std::optional<Lexeme::LexemePtr> lexemes::NamedComponent::
    try_construct(std::string_view& view) {
    if (view.length() == 0 || view[0] != COMPONENT_NAME_IDENTIFIER) return {};

    view.remove_prefix(1);

    size_t end_index = get_length(view, false);

    std::string result = view_to_string(view, end_index);

    view.remove_prefix(end_index);

    return LexemePtr(new NamedComponent(result));
}

Lexeme::LexemePtr lexemes::NamedComponent::
    construct(const std::string& string) {
    return LexemePtr(new NamedComponent(string));
}

std::string lexemes::NamedComponent::dump() const {
    std::stringstream stream;
    stream << "NAMED_COMPONENT " << name_ << ' ' << guid_.left << ' '
           << guid_.right;
    return stream.str();
}
