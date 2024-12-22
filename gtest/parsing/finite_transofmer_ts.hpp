/**
 * @file finite_transofmer_ts.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief FiniteTransformer tests
 * @version 0.1
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "logics/blueprints/scripts/parser/finite_transformer.h"

using ParsingResult = FiniteTransformer::ProcessedWord;

TEST(FiniteTransformer, SanityTest) {
    FTNode root, path_a, path_b, path_ab;

    root >> path_a.by('a');
    root >> path_b.by('b');
    path_a >> path_ab.by('b');

    path_b.mark_terminal();
    path_ab.mark_terminal();

    FiniteTransformer automaton = root.bake();

    std::string string = "ab hello b :)";

    std::string_view view(string);

    ParsingResult success = automaton(view);

    EXPECT_TRUE(success);
    EXPECT_EQ(success.word, "ab");
    EXPECT_EQ(success.end_guid, path_ab.get_guid());
}

TEST(FiniteTransformer, Branching) {
    FTNode root, path_a, path_b, path_ab;

    root >> path_a.by('a');
    root >> path_b.by('a');
    path_a >> path_ab.by('b');

    path_b.mark_terminal();
    path_ab.mark_terminal();

    FiniteTransformer automaton = root.bake();

    std::string string = "ab hello b :)";

    std::string_view view(string);

    ParsingResult success = automaton(view);

    EXPECT_TRUE(success);
    EXPECT_EQ(success.word, "ab");
    EXPECT_EQ(success.end_guid, path_ab.get_guid());
}

TEST(FiniteTransformer, FaultyString) {
    FTNode root, path_a, path_b, path_ab;

    root >> path_a.by('a');
    root >> path_b.by('b');
    path_a >> path_ab.by('b');

    path_b.mark_terminal();
    path_ab.mark_terminal();

    FiniteTransformer automaton = root.bake();

    std::string string = "hello b :)";

    std::string_view view(string);

    ParsingResult failure = automaton(view);

    EXPECT_FALSE(failure);
}

TEST(FiniteTransformer, SequentialParsing) {
    FTNode root, path_a, path_b, path_ab, space;

    root >> path_a.by('a');
    root >> path_b.by('b');
    path_a >> path_ab.by('b');
    root >> space.by(" \t\n");

    path_b.mark_terminal();
    path_ab.mark_terminal();
    space.mark_terminal();

    FiniteTransformer automaton = root.bake();

    std::string string = "ab b ab";

    std::string_view view(string);

    ParsingResult success = automaton(view);
    EXPECT_EQ(success.word, "ab");

    success = automaton(view);
    EXPECT_EQ(success.word, " ");

    success = automaton(view);
    EXPECT_EQ(success.word, "b");

    success = automaton(view);
    EXPECT_EQ(success.word, " ");

    success = automaton(view);
    EXPECT_EQ(success.word, "ab");
}

TEST(FiniteTransformer, Looping) {
    FTNode root, path_a, path_b, space;

    root >> path_a.by('a');
    path_a >> path_a.by('a');
    path_a >> path_b.by('b');

    path_b.mark_terminal();
    space.mark_terminal();

    FiniteTransformer automaton = root.bake();

    std::string string = "aaaab hehe";

    std::string_view view(string);

    ParsingResult success = automaton(view);
    EXPECT_EQ(success.word, "aaaab");
}

TEST(FiniteTransformer, Bor) {
    FTNode root;

    root.append_bor("word");
    root.append_bor("wo");
    root.append_bor("world");
    root.append_bor("worldwide");

    FiniteTransformer automaton = root.bake();

    std::string string = "world hello";

    std::string_view view(string);

    ParsingResult success = automaton(view);
    EXPECT_EQ(success.word, "world");
}

TEST(FiniteTransformer, Merging) {
    FTNode root;

    root.append_bor("word");
    root.append_bor("wo");
    root.append_bor("world");
    root.append_bor("worldwide");

    FTNode space_skipper;

    space_skipper.mark_terminal();
    space_skipper >> space_skipper.by(" \t\n");

    FTNode new_root = FTNode::merge(root, space_skipper);

    FiniteTransformer automaton = new_root.bake();

    std::string word_string = "world";
    std::string space_string = "     world";

    std::string_view word_view = word_string;
    std::string_view space_view = space_string;

    ParsingResult success = automaton(word_view);
    EXPECT_EQ(success.word, "world");

    success = automaton(space_view);
    EXPECT_EQ(success.word, "     ");
}

static FiniteTransformer build_transformer() {
    FTNode root, path_a, path_b, path_ab;

    root >> path_a.by('a');
    root >> path_b.by('b');
    path_a >> path_ab.by('b');

    path_b.mark_terminal();
    path_ab.mark_terminal();

    return root.bake();
}

TEST(FiniteTransformer, MemoryManagement) {
    FiniteTransformer automaton = build_transformer();

    std::string string = "ab hello b :)";

    std::string_view view(string);

    ParsingResult success = automaton(view);

    EXPECT_TRUE(success);
    EXPECT_EQ(success.word, "ab");
}

TEST(FiniteTransformer, Replacements) {
    FTNode root, path_a, path_b, path_ab;

    root >> path_a.by('a', "A");
    root >> path_b.by('b');
    path_a >> path_ab.by('b');

    path_b.mark_terminal();
    path_ab.mark_terminal();

    FiniteTransformer automaton = root.bake();

    std::string string = "ab hello b :)";

    std::string_view view(string);

    ParsingResult success = automaton(view);

    EXPECT_TRUE(success);
    EXPECT_EQ(success.word, "Ab");
    EXPECT_EQ(success.end_guid, path_ab.get_guid());
}

TEST(FiniteTransformer, Deletions) {
    FTNode root, path_a, path_b, path_ab;

    root >> path_a.by('a', "");
    root >> path_b.by('b');
    path_a >> path_ab.by('b');

    path_b.mark_terminal();
    path_ab.mark_terminal();

    FiniteTransformer automaton = root.bake();

    std::string string = "ab hello b :)";

    std::string_view view(string);

    ParsingResult success = automaton(view);

    EXPECT_TRUE(success);
    EXPECT_EQ(success.word, "b");
    EXPECT_EQ(success.end_guid, path_ab.get_guid());
}

TEST(FiniteTransformer, QuotedString) {
    FTNode qs_loop, qs_special, qs_end, quoted_string_recognizer;
    quoted_string_recognizer >> qs_loop.by('"', "");
    qs_loop >> qs_loop.except("\"\\");
    qs_loop >> qs_special.by('\\', "");
    qs_special >> qs_loop.by('\"');
    qs_special >> qs_loop.by('\\');
    qs_loop >> qs_end.by('"', "");
    qs_end.mark_terminal();

    FiniteTransformer automaton = quoted_string_recognizer.bake();

    std::string string = "\"hyper_uvut\"";

    std::string_view view(string);

    ParsingResult success = automaton(view);

    EXPECT_TRUE(success);
    EXPECT_EQ(success.word, "hyper_uvut");
    EXPECT_EQ(success.end_guid, qs_end.get_guid());
}
