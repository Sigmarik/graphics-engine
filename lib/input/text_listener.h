/**
 * @file text_listener.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Abstract text input listener class
 * @version 0.1
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LIB_INPUT_TEXT_LISTENER_H
#define __LIB_INPUT_TEXT_LISTENER_H

#include <stdlib.h>

static const size_t TEXT_LINE_LENGTH = 1024;

struct TextListener {
    virtual ~TextListener() = default;

    virtual void character_callback(unsigned int codepoint);

    virtual void add_char(char character);
    virtual void erase_char();

    virtual void clear();

   private:
    char content_[TEXT_LINE_LENGTH] = "";
    size_t carriage_ = 0;
};

#endif /* __LIB_INPUT_TEXT_LISTENER_H */
