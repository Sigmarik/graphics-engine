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
    String(const std::string& value, bool exact)
        : value_(value), exact_(exact) {}

    static std::optional<LexemePtr> try_construct(std::string_view& view);

    static LexemePtr construct(const std::string& string);
    static Info      get_info() {
        return (Info){
                 .constructor     = construct,
                 .try_constructor = try_construct,
                 .names           = {},
        };
    }

    std::string get_value() const { return value_; }
    bool        is_exact() const { return exact_; }

    virtual std::string dump() const override;

   private:
    std::string value_;

    bool exact_;
};

struct NamedComponent : public Lexeme {
    NamedComponent(const std::string& name) : name_(name) {}

    static std::optional<LexemePtr> try_construct(std::string_view& view);

    static LexemePtr construct(const std::string& string);
    static Info      get_info() {
        return (Info){
                 .constructor     = construct,
                 .try_constructor = try_construct,
                 .names           = {},
        };
    }

    std::string get_name() const { return name_; }
    GUID        get_guid() const { return guid_; }

    void set_guid(const GUID& guid) { guid_ = guid; }

    virtual std::string dump() const override;

   private:
    std::string name_;

    GUID guid_{};
};

};  // namespace lexemes