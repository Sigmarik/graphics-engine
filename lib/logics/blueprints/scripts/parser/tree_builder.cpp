#include "tree_builder.h"

#include "lexemes.h"
#include "logics/blueprints/scripts/nodes/nodes.h"
#include "memory/match_to.hpp"

using LexemeVector = std::vector<Lexeme::LexemePtr>;
using LexemeIterator = LexemeVector::const_iterator;

using NodePtr = std::shared_ptr<Script::Node>;

/*
clang-format off

Code: VariableDeclaration or Pipe sequence

VariableDeclaration: String = Statement
Pipe: Statement::Statement <- Statement

Statement: Conditional or Logical
Conditional: Statement '?' Statement ':' Statement
Logical: Comparator '&&' / '||' ...
Comparator: (Addition '<' / '>' / '<=' / '>=' / '==' / ... Addition) or Addition
Addition: Multiplication '+' / '-' ...
Multiplication: Request '*' / '/' ...
Request: (Value '.' Value (parse right-to-left)) or Value
Value: ('(' Statement ')') or ('[' Statement ']') or ('{' Statement '}') or Function or Constant
Function: 'Sin' / 'Cos' / ... Value
Constant: String or NamedComponent

clang-format on
*/

struct ParserData {
    std::vector<std::weak_ptr<Script::Node>> queue{};
    std::unordered_map<std::string, NodePtr> variables{};
};

#define PARSER(name)                                                        \
    std::optional<NodePtr> name(ParserData& data, LexemeIterator& iterator, \
                                LexemeIterator end)

using Parser = PARSER();

static Parser parse_pipe;
static Parser parse_variable_decl;
static Parser parse_statement;
static Parser parse_logical;
static Parser parse_comparator;
static Parser parse_addition;
static Parser parse_multiplication;
static Parser parse_request;
static Parser parse_value;
static Parser parse_function;
static Parser parse_constant;

std::vector<Abstract<Script::Node>> build_exec_ast(
    const std::vector<Lexeme::LexemePtr>& lexemes,
    std::vector<Script::Node*>& update_queue) {
    // TODO: Implement

    return std::vector<Abstract<Script::Node>>();
}

#define EXPECT(type, message)                                           \
    if (iterator == end) {                                              \
        log_printf(ERROR_REPORTS, "error",                              \
                   "Expected " message ", got EOF instead.\n");         \
        return {};                                                      \
    } else if (!iterator->as<type>()) {                                 \
        log_printf(ERROR_REPORTS, "error",                              \
                   "Expected " message " at line %lu, column %lu.\n",   \
                   (*iterator)->get_line(), (*iterator)->get_column()); \
        return {};                                                      \
    }                                                                   \
    ++iterator;

static PARSER(parse_pipe) {
    auto object = parse_statement(data, iterator, end);
    if (!object) return {};

    EXPECT(lexemes::AssignmentLeft, "an output channel specification");

    auto channel = parse_statement(data, iterator, end);
    if (!channel) return {};

    EXPECT(lexemes::AssignmentRight, "a chanel value");

    auto value = parse_statement(data, iterator, end);
    if (!value) return {};

    return std::make_shared<nodes::InputMethod>(*object, *channel, *value);
}

static PARSER(parse_variable_decl) {
    EXPECT(lexemes::String, "a variable name");
    --iterator;

    std::string name = iterator->as<lexemes::String>()->get_value();

    if (data.variables.find(name) == data.variables.end()) {
        log_printf(ERROR_REPORTS, "error",
                   "Variable with the name \"%s\" has already been declared.\n",
                   name.c_str());
        return {};
    }

    auto value = parse_statement(data, iterator, end);
    if (!value) return {};

    data.variables.insert({name, *value});

    return value;
}

static PARSER(parse_statement) {
    auto first = parse_logical(data, iterator, end);
    if (!first) return {};

    if (iterator->as<lexemes::ConditionalLeft>()) {
        ++iterator;

        auto second = parse_statement(data, iterator, end);
        if (!second) return {};

        EXPECT(lexemes::ConditionalRight, "conditional false clause");

        auto third = parse_statement(data, iterator, end);
        if (!third) return {};

        return std::make_shared<nodes::Conditional>(*first, *second, *third);
    }

    return first;
}

static PARSER(parse_logical) {
    auto initial = parse_comparator(data, iterator, end);
    if (!initial) return {};

    for (;;) {
        if (iterator == end) {
            return initial;
        }

        auto constructor = Match<lexemes::Or, lexemes::And>::
            To<Script::Node, nodes::LogicalOr, nodes::LogicalAnd>::
                From<NodePtr, NodePtr>::constructor(&**iterator);

        if (!constructor) {
            return initial;
        }

        ++iterator;

        auto secondary = parse_comparator(data, iterator, end);
        if (!secondary) return {};

        initial = NodePtr(constructor->operator()(*initial, *secondary));
    }

    return initial;
}

static PARSER(parse_comparator) {
    auto initial = parse_addition(data, iterator, end);
    if (!initial) return {};

    if (iterator == end) {
        return initial;
    }

    auto constructor =
        Match<lexemes::Less, lexemes::Greater, lexemes::LessEq,
              lexemes::GreaterEq, lexemes::Equal, lexemes::NotEqual>::
            To<Script::Node, nodes::Less, nodes::Greater, nodes::LessOrEqual,
               nodes::GreaterOrEqual, nodes::Equal, nodes::NotEqual>::
                From<NodePtr, NodePtr>::constructor(&**iterator);

    if (!constructor) return initial;

    auto secondary = parse_addition(data, iterator, end);
    if (!secondary) return {};

    return NodePtr(constructor->operator()(*initial, *secondary));
}

static PARSER(parse_addition) {
    auto initial = parse_multiplication(data, iterator, end);
    if (!initial) return {};

    for (;;) {
        if (iterator == end) {
            return initial;
        }

        auto constructor = Match<lexemes::Plus, lexemes::Minus>::
            To<Script::Node, nodes::Add, nodes::Subtract>::
                From<NodePtr, NodePtr>::constructor(&**iterator);

        if (!constructor) {
            return initial;
        }

        ++iterator;

        auto secondary = parse_multiplication(data, iterator, end);
        if (!secondary) return {};

        initial = NodePtr(constructor->operator()(*initial, *secondary));
    }

    return initial;
}

static PARSER(parse_multiplication) {
    auto initial = parse_request(data, iterator, end);
    if (!initial) return {};

    for (;;) {
        if (iterator == end) {
            return initial;
        }

        auto constructor = Match<lexemes::Multiply, lexemes::Divide>::
            To<Script::Node, nodes::Multiply, nodes::Divide>::
                From<NodePtr, NodePtr>::constructor(&**iterator);

        if (!constructor) {
            return initial;
        }

        ++iterator;

        auto secondary = parse_request(data, iterator, end);
        if (!secondary) return {};

        initial = NodePtr(constructor->operator()(*initial, *secondary));
    }

    return initial;
}

static PARSER(parse_request) {
    bool unary_minus =
        iterator != end && iterator->as<lexemes::Minus>() != nullptr;
    if (unary_minus) ++iterator;

    auto initial = parse_value(data, iterator, end);
    if (!initial) return {};

    for (;;) {
        if (iterator == end || iterator->as<lexemes::Access>() == nullptr) {
            break;
        }

        ++iterator;

        auto secondary = parse_value(data, iterator, end);
        if (!secondary) return {};

        initial = NodePtr(new nodes::OutputMethod(*initial, *secondary));
    }

    if (unary_minus) {
        return NodePtr(new nodes::Negative(*initial));
    }

    return initial;
}

static PARSER(parse_value) {
    if (iterator == end) return {};

    auto wrapper_constructor =
        Match<lexemes::BracketCurlyOp, lexemes::BracketSquareOp>::
            To<Script::Node, nodes::DetectChange, nodes::RequireValidity>::
                From<NodePtr>::constructor(&**iterator);

    bool just_separation = iterator->as<lexemes::BracketRoundOp>();

    if (wrapper_constructor || just_separation) {
        ++iterator;

        auto statement = parse_statement(data, iterator, end);
        if (!statement) return {};

        if (wrapper_constructor) {
            return NodePtr(wrapper_constructor->operator()(*statement));
        }

        return statement;
    }

    auto function_call = parse_function(data, iterator, end);
    if (function_call) return function_call;

    if (iterator == end) {
        log_printf(
            ERROR_REPORTS, "error",
            "Expected a value in the end of the script, got EOF instead.\n");
        return {};
    }

    auto constant = parse_constant(data, iterator, end);
    if (!constant) {
        log_printf(ERROR_REPORTS, "error",
                   "Expected a value at line %lu, column %lu.\n",
                   (*iterator)->get_line(), (*iterator)->get_column());
    }

    return *constant;
}

static PARSER(parse_function) {
    if (iterator == end) return {};

    auto constructor = Match<lexemes::Sine, lexemes::Cosine, lexemes::Ln,
                             lexemes::Log2, lexemes::Log10, lexemes::Absolute,
                             lexemes::Sign, lexemes::Length>::
        To<Script::Node, nodes::Sin, nodes::Cos, nodes::LogE, nodes::Log2,
           nodes::Log10, nodes::Absolute, nodes::Sign, nodes::Length>::
            From<NodePtr>::constructor(&**iterator);

    if (!constructor) return {};

    ++iterator;

    auto value = parse_value(data, iterator, end);
    if (!value) return {};

    return NodePtr(constructor->operator()(*value));
}

static PARSER(parse_constant) {
    if (iterator == end) return {};

    NodePtr result;

    if (auto lexeme = iterator->as<lexemes::String>()) {
        result = NodePtr(new nodes::StringConstant(lexeme->get_value()));

        // In case it is a name of a variable
        auto variable_find = data.variables.find(lexeme->get_value());
        if (variable_find != data.variables.end()) {
            return variable_find->second;
        }
    }

    if (auto lexeme = iterator->as<lexemes::NamedComponent>()) {
        std::string value = lexeme->get_guid().to_string();
        result = NodePtr(new nodes::StringConstant(value));
    }

    if (!result) return {};

    data.queue.push_back(result);
}
