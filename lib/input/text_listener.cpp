#include "text_listener.h"

#include <stdio.h>
#include <string.h>

void TextListener::character_callback(unsigned int codepoint) {
    printf("Detected codepoint %u\n", codepoint);

    // TODO: Implement
}

void TextListener::add_char(char character) {
    if (carriage_ + 1 >= TEXT_LINE_LENGTH) return;

    memcpy(content_ + carriage_ + 1, content_ + carriage_,
           TEXT_LINE_LENGTH - carriage_ - 1);
    content_[carriage_] = character;
    carriage_++;
}

void TextListener::erase_char() {
    if (carriage_ == 0) return;

    memcpy(content_ + carriage_ - 1, content_ + carriage_,
           TEXT_LINE_LENGTH - carriage_);

    carriage_--;
}

void TextListener::clear() {
    memset(content_, 0, TEXT_LINE_LENGTH);
    carriage_ = 0;
}
