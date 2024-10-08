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

#define PARSER(name)                                                        \
    std::optional<NodePtr> name(ParsedTree& data, LexemeIterator& iterator, \
                                LexemeIterator end)

static NodePtr new_node(Script::Node* node,
                        std::vector<std::weak_ptr<Script::Node>>& nodes) {
    NodePtr result(node);

    nodes.push_back(result);

    return result;
}

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

template <class T>
static void remove_expired(std::vector<std::weak_ptr<T>>& vector) {
    size_t left = 0;
    for (size_t id = 0; id < vector.size(); ++id) {
        if (vector[id].expired()) continue;

        vector[left] = std::move(vector[id]);
        ++left;
    }

    vector.resize(left);
}

ParsedTree build_exec_ast(const std::vector<Lexeme::LexemePtr>& lexemes) {
    ParsedTree tree;

    auto iterator = lexemes.begin();
    auto end = lexemes.end();

    while (iterator != end) {
        auto current = iterator;

        auto variable = parse_variable_decl(tree, iterator, end);
        if (variable) continue;

        auto pipe = parse_pipe(tree, iterator, end);
        if (!pipe) {
            log_printf(ERROR_REPORTS, "error",
                       "Failed to parse statement at line %lu, column %lu.\n",
                       (*current)->get_line(), (*current)->get_column());
            return {};
        }

        tree.roots.push_back(*pipe);
    }

    remove_expired(tree.nodes);
    remove_expired(tree.queue);

    return tree;
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

    return new_node(new nodes::InputMethod(*object, *channel, *value),
                    data.nodes);
}

static PARSER(parse_variable_decl) {
    if (iterator == end || iterator + 1 == end) return {};

    if (!iterator->as<lexemes::String>() ||
        !(iterator + 1)->as<lexemes::MacroAssignment>())
        return {};

    std::string name = iterator->as<lexemes::String>()->get_value();

    if (data.variables.find(name) != data.variables.end()) {
        log_printf(ERROR_REPORTS, "error",
                   "Variable with the name \"%s\" has already been declared.\n",
                   name.c_str());
        return {};
    }

    ++iterator;
    ++iterator;

    auto value = parse_statement(data, iterator, end);
    if (!value) return {};

    data.variables.insert({name, *value});

    return value;
}

static PARSER(parse_statement) {
    auto first = parse_logical(data, iterator, end);
    if (!first) return {};

    if (iterator == end) return first;

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

        initial =
            new_node(constructor->operator()(*initial, *secondary), data.nodes);
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

    return new_node(constructor->operator()(*initial, *secondary), data.nodes);
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

        initial =
            new_node(constructor->operator()(*initial, *secondary), data.nodes);
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

        initial =
            new_node(constructor->operator()(*initial, *secondary), data.nodes);
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
        if (iterator == end || !iterator->as<lexemes::Access>()) {
            break;
        }

        ++iterator;

        auto secondary = parse_value(data, iterator, end);
        if (!secondary) return {};

        initial =
            new_node(new nodes::OutputMethod(*initial, *secondary), data.nodes);
    }

    if (unary_minus) {
        return new_node(new nodes::Negative(*initial), data.nodes);
    }

    return initial;
}

static PARSER(parse_value) {
    if (iterator == end) return {};

    auto wrapper_constructor =
        Match<lexemes::BracketCurlyOp, lexemes::BracketSquareOp>::
            To<Script::Node, nodes::DetectChange, nodes::DetectSource>::
                From<NodePtr>::constructor(&**iterator);

    bool just_separation = iterator->as<lexemes::BracketRoundOp>();

    if (wrapper_constructor || just_separation) {
        ++iterator;

        auto statement = parse_statement(data, iterator, end);
        if (!statement) return {};

        if (iterator == end) {
            log_printf(
                ERROR_REPORTS, "error",
                "Expected a closing bracket at the end of the script.\n");
            return {};
        }

        // TODO: Check for the exact type of bracket.
        if (!iterator->as<lexemes::BracketCurlyCl>() &&
            !iterator->as<lexemes::BracketSquareCl>() &&
            !iterator->as<lexemes::BracketRoundCl>()) {
            log_printf(ERROR_REPORTS, "error",
                       "Expected a closing bracket at line %lu, column %lu.\n",
                       (*iterator)->get_line(), (*iterator)->get_column());
            return {};
        }

        ++iterator;

        if (wrapper_constructor) {
            return new_node(wrapper_constructor->operator()(*statement),
                            data.nodes);
        }

        return statement;
    }

    auto function_call = parse_function(data, iterator, end);
    if (function_call) return function_call;

    auto constant = parse_constant(data, iterator, end);
    if (constant) return constant;

    if (iterator == end) {
        log_printf(
            ERROR_REPORTS, "error",
            "Expected a value in the end of the script, got EOF instead.\n");
    } else {
        log_printf(ERROR_REPORTS, "error",
                   "Expected a value at line %lu, column %lu.\n",
                   (*iterator)->get_line(), (*iterator)->get_column());
    }

    return {};
}

static PARSER(parse_function) {
    if (iterator == end) return {};

    auto constructor =
        Match<lexemes::Sine, lexemes::Cosine, lexemes::Ln, lexemes::Log2,
              lexemes::Log10, lexemes::Absolute, lexemes::Sign, lexemes::Length,
              lexemes::SkipInvalid>::
            To<Script::Node, nodes::Sin, nodes::Cos, nodes::LogE, nodes::Log2,
               nodes::Log10, nodes::Absolute, nodes::Sign, nodes::Length,
               nodes::RequireValidity>::From<NodePtr>::constructor(&**iterator);

    if (!constructor) return {};

    ++iterator;

    auto value = parse_value(data, iterator, end);
    if (!value) return {};

    return new_node(constructor->operator()(*value), data.nodes);
}

static PARSER(parse_constant) {
    if (iterator == end) return {};

    NodePtr result;

    if (auto lexeme = iterator->as<lexemes::String>()) {
        result = new_node(new nodes::StringConstant(lexeme->get_value()),
                          data.nodes);

        // In case it is a name of a variable
        auto variable_find = data.variables.find(lexeme->get_value());
        if (!lexeme->is_exact() && variable_find != data.variables.end()) {
            ++iterator;
            return variable_find->second;
        }
    }
    if (auto lexeme = iterator->as<lexemes::NamedComponent>()) {
        std::string value = lexeme->get_guid().to_string();
        result = new_node(new nodes::StringConstant(value), data.nodes);
    }

    if (!result) return {};

    data.queue.push_back(result);

    ++iterator;

    return result;
}
