#include "lexemes.h"

const std::vector<Lexeme::Info> LEXEME_INFO_TABLE = {
    lexemes::AssignmentLeft::get_info(),
    lexemes::Access::get_info(),
    lexemes::AssignmentRight::get_info(),

    lexemes::And::get_info(),
    lexemes::ConditionalLeft::get_info(),
    lexemes::ConditionalRight::get_info(),
    lexemes::Equal::get_info(),
    lexemes::Not::get_info(),
    lexemes::NotEqual::get_info(),
    lexemes::Or::get_info(),
    lexemes::AlphaGreater::get_info(),
    lexemes::AlphaLess::get_info(),
    lexemes::Greater::get_info(),
    lexemes::GreaterEq::get_info(),
    lexemes::Less::get_info(),
    lexemes::LessEq::get_info(),
    lexemes::Divide::get_info(),
    lexemes::Minus::get_info(),
    lexemes::Multiply::get_info(),
    lexemes::Plus::get_info(),
    lexemes::Power::get_info(),

    lexemes::Absolute::get_info(),
    lexemes::Cosine::get_info(),
    lexemes::Length::get_info(),
    lexemes::Ln::get_info(),
    lexemes::Log10::get_info(),
    lexemes::Log2::get_info(),
    lexemes::Sign::get_info(),
    lexemes::Sine::get_info(),
    lexemes::SkipInvalid::get_info(),

    lexemes::BracketRoundOp::get_info(),
    lexemes::BracketRoundCl::get_info(),
    lexemes::BracketSquareOp::get_info(),
    lexemes::BracketSquareCl::get_info(),
    lexemes::BracketCurlyOp::get_info(),
    lexemes::BracketCurlyCl::get_info(),

    lexemes::NamedComponent::get_info(),
    lexemes::String::get_info(),

    lexemes::MacroAssignment::get_info(),
};
