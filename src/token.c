#include "token.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char* tokenTypeNames[] = {
    "TOKEN_ADD",
    "TOKEN_SUB",
    "TOKEN_DIV",
    "TOKEN_MUL",
    "TOKEN_MOD",
    "TOKEN_POW",
    "TOKEN_AND",
    "TOKEN_OR",
    "TOKEN_HAT",
    "TOKEN_SHL",
    "TOKEN_SHR",
    "TOKEN_NOT",
    "TOKEN_BOOL_NOT",
    "TOKEN_BOOL_EQ",
    "TOKEN_BOOL_NOTE",
    "TOKEN_BOOL_LT",
    "TOKEN_BOOL_GT",
    "TOKEN_BOOL_LTE",
    "TOKEN_BOOL_GTE",
    "TOKEN_BOOL_AND",
    "TOKEN_BOOL_OR",
    "TOKEN_ID",
    "TOKEN_KEYWORD",
    "TOKEN_SIZEOF",
    "TOKEN_AS",
    "TOKEN_CHAR",
    "TOKEN_STRING",
    "TOKEN_INT",
    "TOKEN_FLOAT",
    "TOKEN_LPAREN",
    "TOKEN_RPAREN",
    "TOKEN_LBRACKET",
    "TOKEN_RBRACKET",
    "TOKEN_LBRACE",
    "TOKEN_RBRACE",
    "TOKEN_EQUALS",
    "TOKEN_COMMA",
    "TOKEN_DOT",
    "TOKEN_COLON",
    "TOKEN_NAMESPACE",
    "TOKEN_SEMI",
    "TOKEN_QUESTION",
    "TOKEN_AT",
    "TOKEN_HASH",
    "TOKEN_LINE_COMMENT",
    "TOKEN_MULTILINE_COMMENT_START",
    "TOKEN_MULTILINE_COMMENT_END",
    "TOKEN_POINTER",
    "TOKEN_ARROW",
    "TOKEN_BACKSLASH",
    "TOKEN_NL",
    "TOKEN_EOF",
    "TOKEN_UNKNOWN",
};

static void Print(token_t* self) {
    char* buf = self->Str(self);
    printf("%s\n", buf);
    free(buf);
}

static char* Str(token_t* self) {
    static const char template[] = "(%s): `%s`"; 

    size_t size = strlen(self->value) + strlen(tokenTypeToStr(self->type)) + ARRAYSIZE(template);
    char* buf = (char*) malloc(size);
    sprintf_s(buf, size, template, tokenTypeToStr(self->type), self->value[0] == '\n' ? "\\n" : self->value);
    return buf;
}

/**
 * @brief Initialized a token
 * 
 * @param self the token pointer
 * @param value the value of the token
 * @param type the type of the token
 */
void Token(token_t* self, char* value, tokenType_t type) {
    self->type = type;
    self->value = value;

    self->Print = Print;
    self->Str = Str;
}

token_t* newToken(char* value, tokenType_t type) {
    token_t* self = calloc(1, sizeof(token_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't callocate in function: %s\n", __func__);
        return NULL;
    }
    
    Token(self, value, type);
    return self;
}

const char* tokenTypeToStr(tokenType_t type) {
    return tokenTypeNames[type];
}