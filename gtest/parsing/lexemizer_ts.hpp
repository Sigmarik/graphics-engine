/**
 * @file lexemizer_ts.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Lexemizer test suit
 * @version 0.1
 * @date 2024-12-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/blueprints/scripts/parser/lexemizer.h"

TEST(Lexemizer, Operators) {
    std::string code = "+-*/==->::";
    std::vector<Lexeme::LexemePtr> lexemes = lexify(code);

    ASSERT_EQ(lexemes.size(), 7);

    EXPECT_TRUE(lexemes[0].as<lexemes::Plus>());
    EXPECT_TRUE(lexemes[1].as<lexemes::Minus>());
    EXPECT_TRUE(lexemes[2].as<lexemes::Multiply>());
    EXPECT_TRUE(lexemes[3].as<lexemes::Divide>());
    EXPECT_TRUE(lexemes[4].as<lexemes::Equal>());
    EXPECT_TRUE(lexemes[5].as<lexemes::AssignmentLeft>());
    EXPECT_TRUE(lexemes[6].as<lexemes::AssignmentLeft>());
}

TEST(Lexemizer, WhitespaceAndComments) {
    std::string code =
        "# Oh no!\n\t\t\t   \t \n# This code is full of comments!\n#Terminal";
    std::vector<Lexeme::LexemePtr> lexemes = lexify(code);

    ASSERT_EQ(lexemes.size(), 0);
}

TEST(Lexemizer, SimpleString) {
    std::string code = "hyper_uvut";
    std::vector<Lexeme::LexemePtr> lexemes = lexify(code);

    ASSERT_EQ(lexemes.size(), 1);

    ASSERT_TRUE(lexemes[0].as<lexemes::String>());

    EXPECT_STREQ(lexemes[0].as<lexemes::String>()->get_value().c_str(),
                 "hyper_uvut");
    EXPECT_FALSE(lexemes[0].as<lexemes::String>()->is_exact());
}

TEST(Lexemizer, SimpleQuotedString) {
    std::string code = "\"hyper_uvut\"";
    std::vector<Lexeme::LexemePtr> lexemes = lexify(code);

    ASSERT_EQ(lexemes.size(), 1);

    ASSERT_TRUE(lexemes[0].as<lexemes::String>());

    EXPECT_STREQ(lexemes[0].as<lexemes::String>()->get_value().c_str(),
                 "hyper_uvut");
    EXPECT_TRUE(lexemes[0].as<lexemes::String>()->is_exact());
}

TEST(Lexemizer, ComplexQuotedString) {
    std::string code = "\"\\\\hyper\\\\ \\\"uvut\\\"\"";
    std::vector<Lexeme::LexemePtr> lexemes = lexify(code);

    ASSERT_EQ(lexemes.size(), 1);

    ASSERT_TRUE(lexemes[0].as<lexemes::String>());

    EXPECT_STREQ(lexemes[0].as<lexemes::String>()->get_value().c_str(),
                 "\\hyper\\ \"uvut\"");
    EXPECT_TRUE(lexemes[0].as<lexemes::String>()->is_exact());
}

TEST(Lexemizer, Numeric) {
    std::string code = "11.99";
    std::vector<Lexeme::LexemePtr> lexemes = lexify(code);

    ASSERT_EQ(lexemes.size(), 1);

    ASSERT_TRUE(lexemes[0].as<lexemes::String>());

    EXPECT_STREQ(lexemes[0].as<lexemes::String>()->get_value().c_str(),
                 "11.99");
    EXPECT_FALSE(lexemes[0].as<lexemes::String>()->is_exact());
}

TEST(Lexemizer, AllStrings) {
    std::string code =
        "\"\\\\Hello\\\\, \\\"world!\\\"\" testicular+torsion=pain 112.99.00";
    std::vector<Lexeme::LexemePtr> lexemes = lexify(code);

    ASSERT_EQ(lexemes.size(), 9);

    ASSERT_TRUE(lexemes[0].as<lexemes::String>());
    ASSERT_TRUE(lexemes[1].as<lexemes::String>());
    ASSERT_TRUE(lexemes[2].as<lexemes::Plus>());
    ASSERT_TRUE(lexemes[3].as<lexemes::String>());
    ASSERT_TRUE(lexemes[4].as<lexemes::MacroAssignment>());
    ASSERT_TRUE(lexemes[5].as<lexemes::String>());
    ASSERT_TRUE(lexemes[6].as<lexemes::String>());
    ASSERT_TRUE(lexemes[7].as<lexemes::Access>());
    ASSERT_TRUE(lexemes[8].as<lexemes::String>());

    EXPECT_STREQ(lexemes[0].as<lexemes::String>()->get_value().c_str(),
                 "\\Hello\\, \"world!\"");
    EXPECT_STREQ(lexemes[3].as<lexemes::String>()->get_value().c_str(),
                 "torsion");
    EXPECT_STREQ(lexemes[6].as<lexemes::String>()->get_value().c_str(),
                 "112.99");
}

static const char CODE_EXAMPLE[] = R"""(
# This is a sample script.

message = "Hello from the scripting world! See the level script to learn where this message came from!"

@SomeShouterComponent::shout <- message
@SomeShouterComponent::shout <- @PlayerBall.knocked_down ? "RESET!" : IMPOSSIBLE
)""";

TEST(Lexemizer, RealCode) {
    std::string code = CODE_EXAMPLE;
    std::vector<Lexeme::LexemePtr> lexemes = lexify(code);

    ASSERT_EQ(lexemes.size(), 19);

    ASSERT_TRUE(lexemes[0].as<lexemes::String>());
    ASSERT_TRUE(lexemes[1].as<lexemes::MacroAssignment>());
    ASSERT_TRUE(lexemes[2].as<lexemes::String>());

    ASSERT_TRUE(lexemes[3].as<lexemes::NamedComponent>());
    ASSERT_TRUE(lexemes[4].as<lexemes::AssignmentLeft>());
    ASSERT_TRUE(lexemes[5].as<lexemes::String>());
    ASSERT_TRUE(lexemes[6].as<lexemes::AssignmentRight>());
    ASSERT_TRUE(lexemes[7].as<lexemes::String>());

    ASSERT_TRUE(lexemes[8].as<lexemes::NamedComponent>());
    ASSERT_TRUE(lexemes[9].as<lexemes::AssignmentLeft>());
    ASSERT_TRUE(lexemes[10].as<lexemes::String>());
    ASSERT_TRUE(lexemes[11].as<lexemes::AssignmentRight>());

    ASSERT_TRUE(lexemes[12].as<lexemes::NamedComponent>());
    ASSERT_TRUE(lexemes[13].as<lexemes::Access>());
    ASSERT_TRUE(lexemes[14].as<lexemes::String>());
    ASSERT_TRUE(lexemes[15].as<lexemes::ConditionalLeft>());
    ASSERT_TRUE(lexemes[16].as<lexemes::String>());
    ASSERT_TRUE(lexemes[17].as<lexemes::ConditionalRight>());
    ASSERT_TRUE(lexemes[18].as<lexemes::String>());

    EXPECT_STREQ(lexemes[0].as<lexemes::String>()->get_value().c_str(),
                 "message");
    EXPECT_FALSE(lexemes[0].as<lexemes::String>()->is_exact());

    EXPECT_STREQ(lexemes[7].as<lexemes::String>()->get_value().c_str(),
                 "message");
    EXPECT_FALSE(lexemes[7].as<lexemes::String>()->is_exact());

    EXPECT_STREQ(lexemes[12].as<lexemes::NamedComponent>()->get_name().c_str(),
                 "PlayerBall");
}
