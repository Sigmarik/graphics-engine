/**
 * @file strings.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Sting lexeme
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "hash/guid.h"
#include "logics/blueprints/scripts/parser/lexeme.h"

namespace lexemes {

struct String : public Lexeme {
    String(std::string_view& view) : value_(view.begin(), view.end()) {}

    static std::optional<LexemePtr> try_construct(std::string_view& view);

    std::string get_value() const { return value_; }

   private:
    std::string value_;
};

struct NamedComponent : public Lexeme {
    NamedComponent(std::string_view& view) : name_(view.begin(), view.end()) {}

    static std::optional<LexemePtr> try_construct(std::string_view& view);

    std::string get_name() const { return name_; }
    std::string get_guid() const { return guid_.to_string(); }

    void set_guid(const GUID& guid) { guid_ = guid; }

   private:
    std::string name_;

    GUID guid_{};
};

};  // namespace lexemes