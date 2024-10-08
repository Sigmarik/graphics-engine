#include "lexemes.h"

const std::vector<Lexeme::Constructor> LEXEME_CONSTRUCTORS = {
    lexemes::AssignmentLeft::try_construct,
    lexemes::Access::try_construct,
    lexemes::AssignmentRight::try_construct,

    lexemes::And::try_construct,
    lexemes::ConditionalLeft::try_construct,
    lexemes::ConditionalRight::try_construct,
    lexemes::Equal::try_construct,
    lexemes::Not::try_construct,
    lexemes::NotEqual::try_construct,
    lexemes::Or::try_construct,
    lexemes::AlphaGreater::try_construct,
    lexemes::AlphaLess::try_construct,
    lexemes::Greater::try_construct,
    lexemes::GreaterEq::try_construct,
    lexemes::Less::try_construct,
    lexemes::LessEq::try_construct,
    lexemes::Divide::try_construct,
    lexemes::Minus::try_construct,
    lexemes::Multiply::try_construct,
    lexemes::Plus::try_construct,
    lexemes::Power::try_construct,

    lexemes::Absolute::try_construct,
    lexemes::Cosine::try_construct,
    lexemes::Length::try_construct,
    lexemes::Ln::try_construct,
    lexemes::Log10::try_construct,
    lexemes::Log2::try_construct,
    lexemes::Sign::try_construct,
    lexemes::Sine::try_construct,
    lexemes::SkipInvalid::try_construct,

    lexemes::BracketRoundOp::try_construct,
    lexemes::BracketRoundCl::try_construct,
    lexemes::BracketSquareOp::try_construct,
    lexemes::BracketSquareCl::try_construct,
    lexemes::BracketCurlyOp::try_construct,
    lexemes::BracketCurlyCl::try_construct,

    lexemes::NamedComponent::try_construct,
    lexemes::String::try_construct,

    lexemes::MacroAssignment::try_construct,
};
