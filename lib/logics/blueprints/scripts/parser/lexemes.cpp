#include "lexemes.h"

const std::vector<Lexeme::Constructor> LEXEME_CONSTRUCTORS = {
    lexemes::AssignmentLeft::try_construct,
    lexemes::AssignmentRight::try_construct,
    // TODO: Complete
};
