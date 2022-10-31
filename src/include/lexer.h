#pragma once

#include "token.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct lexer_t lexer_t;

struct lexer_t {
    char* src;          // src code
    size_t src_size;    // src code size
    char c;             // current char
    size_t i;           // current index

    void     (*Advance)        (lexer_t* self);
    bool     (*CanAdvance)     (lexer_t* self);
    token_t* (*NextToken)      (lexer_t* self);
    void     (*SkipWhitespace) (lexer_t* self);
    char     (*Peek)           (lexer_t* self, size_t offset);

    token_t* (*CollectString) (lexer_t* self);
    token_t* (*CollectNumber) (lexer_t* self);
    token_t* (*CollectSymbol) (lexer_t* self);
    token_t* (*CollectId)     (lexer_t* self);

};

void Lexer(lexer_t* self, char* src);
lexer_t* newLexer(char* src);