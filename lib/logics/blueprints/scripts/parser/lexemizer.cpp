#include "lexemizer.h"

#include "finite_transformer.h"
#include "lexemes/strings.h"
#include "logger/logger.h"

struct LexemeRecognizer {
    LexemeRecognizer();

    std::optional<Lexeme::LexemePtr> operator()(std::string_view& view);

   private:
    void assemble_lex_bor(FTNode& root);

    std::map<GUID, Lexeme::Info> endpoints_{};
    FiniteTransformer transformer_{};
};

std::vector<Lexeme::LexemePtr> lexify(const std::string& text) {
    static LexemeRecognizer lexemizer{};

    std::vector<Lexeme::LexemePtr> lexemes;

    std::string_view view = text;

    while (view.length() > 0) {
        std::string_view prev_view = view;

        std::optional<Lexeme::LexemePtr> lexeme = lexemizer(view);

        if (view.length() >= prev_view.length()) break;

        if (!lexeme) continue;

        lexemes.push_back(*lexeme);
    }

    return lexemes;
}

LexemeRecognizer::LexemeRecognizer() {
    FTNode lex_bor;
    FTNode whitespace_skipper;
    FTNode comment_skipper;
    FTNode simple_string_recognizer;
    FTNode quoted_string_recognizer;
    FTNode number_recognizer;
    FTNode nickname_recognizer;

    assemble_lex_bor(lex_bor);

    whitespace_skipper >> whitespace_skipper.by(" \t\n");
    whitespace_skipper.mark_terminal();

    FTNode comm_skip_loop, comm_skip_terminal;
    comment_skipper >> comm_skip_loop.by('#');
    comm_skip_loop >> comm_skip_loop.except('\n');
    comm_skip_loop >> comm_skip_terminal.by('\n');
    comm_skip_loop.mark_terminal();
    comm_skip_terminal.mark_terminal();

    FTNode nn_loop;
    nickname_recognizer >> nn_loop.by('@', "");
    nn_loop >> nn_loop.by_alnum("_");
    endpoints_[nn_loop.get_guid()] = lexemes::NamedComponent::get_info();
    nn_loop.mark_terminal();

    simple_string_recognizer >> simple_string_recognizer.by_alnum("_");
    endpoints_[simple_string_recognizer.get_guid()] = lexemes::String::
        get_info();
    simple_string_recognizer.mark_terminal();

    FTNode qs_loop, qs_special, qs_end;
    quoted_string_recognizer >> qs_loop.by('"', "");
    qs_loop >> qs_loop.except("\"\\");
    qs_loop >> qs_special.by('\\', "");
    qs_special >> qs_loop.by('\"');
    qs_special >> qs_loop.by('\\');
    qs_loop >>
        qs_end.by('"', "\xFF");  // Hint the string constructor that the
                                 // current string is not a variable name.
    endpoints_[qs_end.get_guid()] = lexemes::String::get_info();
    qs_end.mark_terminal();

    FTNode num_mid, num_afterdot, num_end;
    number_recognizer >> num_mid.by_numeric();
    num_mid >> num_mid.by_numeric();
    num_mid >> num_afterdot.by('.');
    num_afterdot >> num_end.by_numeric();
    num_end >> num_end.by_numeric();
    endpoints_[num_end.get_guid()] = lexemes::String::get_info();
    num_end.mark_terminal();

    FTNode root = FTNode::merge(
        lex_bor, whitespace_skipper, comment_skipper, simple_string_recognizer,
        quoted_string_recognizer, number_recognizer, nickname_recognizer);

    transformer_ = root.bake();
}

std::optional<Lexeme::LexemePtr> LexemeRecognizer::
operator()(std::string_view& view) {
    std::string_view view_copy = view;
    FiniteTransformer::ProcessedWord parsing_result = transformer_(view_copy);

    if (!parsing_result) {
        log_dup(ERROR_REPORTS, "error", "Unknown token.\n\t%s\n", view.data());
        return {};
    }

    view = view_copy;

    if (!endpoints_.contains(parsing_result.end_guid)) return {};

    return endpoints_[parsing_result.end_guid].constructor(parsing_result.word);
}

void LexemeRecognizer::assemble_lex_bor(FTNode& root) {
    for (Lexeme::Info info : LEXEME_INFO_TABLE) {
        for (const std::string& name : info.names) {
            FTNode* node = root.append_bor(name);
            endpoints_[node->get_guid()] = info;
        }
    }
}
