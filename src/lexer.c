#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static void appendChar(char** src, char c) {
    if (!(*src = realloc(*src, strlen(*src) + 2))) {
        fprintf(stderr, "ERROR: Couldn't reallocate in function: %s\n", __func__);
        exit(1);
    }
    strcat(*src, (char[]){c, 0});
}

static bool isSymbol(char c) {
    static const char symbols[] = "()[]{}=@#,.:;?\\+-/*%&|^<>~!";
    for (size_t i = 0; i < ARRAYSIZE(symbols); i++)
        if (symbols[i] == c) 
            return true;
    
    return false;
}

static tokenType_t findSymbol(char* s) {
    switch (s[0]) {
        case ':': if (s[1] == s[0]) return TOKEN_NAMESPACE;
        case '&': if (s[1] == s[0]) return TOKEN_BOOL_AND; 
        case '|': if (s[1] == s[0]) return TOKEN_BOOL_OR;  
        case '*': if (s[1] == s[0]) return TOKEN_POW;      

        case '-': 
            if (s[1] == '>') return TOKEN_POINTER; 
            break;

        case '!': 
            if (s[1] == '=') return TOKEN_BOOL_NOTE; 
            break;
        
        case '=': 
            if (s[1] == s[0]) return TOKEN_BOOL_EQ; 
            else if (s[1] == '>') return TOKEN_ARROW; 
            break;
        
        case '<': 
            if (s[1] == s[0]) return TOKEN_SHL; 
            else if (s[1] == '=') return TOKEN_BOOL_LTE; 
            break;
        
        case '>': 
            if (s[1] == s[0]) return TOKEN_SHR; 
            else if (s[1] == '=') return TOKEN_BOOL_GTE; 
            break;

        default: break;
    }

    if (s[1] != 0) return TOKEN_UNKNOWN;

    switch (s[0]) {
        case '(': return TOKEN_LPAREN;
        case ')': return TOKEN_RPAREN;
        case '[': return TOKEN_LBRACKET;
        case ']': return TOKEN_RBRACKET;
        case '{': return TOKEN_LBRACE;
        case '}': return TOKEN_RBRACE;
        case '=': return TOKEN_EQUALS;
        case ',': return TOKEN_COMMA;
        case '.': return TOKEN_DOT;
        case ':': return TOKEN_COLON;
        case ';': return TOKEN_SEMI;
        case '?': return TOKEN_QUESTION;
        case '%': return TOKEN_MOD;
        case '\\': return TOKEN_BACKSLASH;
        case '#': return TOKEN_HASH;
        case '@': return TOKEN_AT;
        case '+': return TOKEN_ADD;
        case '-': return TOKEN_SUB;
        case '/': return TOKEN_DIV;
        case '*': return TOKEN_MUL;
        case '&': return TOKEN_AND;
        case '|': return TOKEN_OR;
        case '^': return TOKEN_HAT;
        case '<': return TOKEN_BOOL_LT;
        case '>': return TOKEN_BOOL_GT;
        case '~': return TOKEN_NOT;
        case '!': return TOKEN_BOOL_NOT;
        default:
            return TOKEN_UNKNOWN;
    }
}

// static char Peek(lexer_t* self, size_t offset) {
//     if (self->i + offset < self->src_size)
//         return self->src[self->i + offset];
//     return 0;
// }

static bool CanAdvance(lexer_t* self) {
    return self->i < self->src_size && self->c != 0;
}

static void Advance(lexer_t* self) {
    if (self->CanAdvance(self)) {
        self->i++;
        self->c = self->src[self->i];
        return;
    }

    self->c = 0;
}

static token_t* NextToken(lexer_t* self) {
    if (self->c == 0) return newToken("EOF", TOKEN_EOF);

    self->SkipWhitespace(self);
    if (self->c == '"' || self->c == '\'') 
        return self->CollectString(self);
    
    if (isdigit(self->c))
        return self->CollectNumber(self);

    if (isSymbol(self->c))
        return self->CollectSymbol(self);

    if (!isspace(self->c))
        return self->CollectId(self);

    if (self->c == '\n') {
        self->Advance(self);
        return newToken("\n", TOKEN_NL);
    }
    
    return NULL;
}

static void SkipWhitespace(lexer_t* self) {
    while (self->c == ' ' || self->c == '\r') self->Advance(self);
}

static token_t* CollectString(lexer_t* self) {
    char* value = (char*) calloc(1, sizeof(char));

    const bool single = self->c == '\'';
    self->Advance(self);

    // Collecting the string
    while (self->CanAdvance(self) && (
        (self->c != '\'' && single) || 
        (self->c != '"' && !single)
    )) {
        appendChar(&value, self->c);
        self->Advance(self);
    }

    // Checking end of string
    if ((self->c != '\'' && single) || (self->c != '"' && !single)) {
        // TODO: add error handling
        char* buf = (char*) malloc(strlen(value) + 3);
        sprintf_s(buf, strlen(value) + 3, "%c%s%c", (single ? '\'' : '"'), value, self->c ? self->c : ' ');
        fprintf(stderr, "Lexer error: Unexpected end of string:\n\t%s", buf);
    }

    // Eating last quote
    self->Advance(self);
    return newToken(value, TOKEN_STRING);
}

static token_t* CollectNumber (lexer_t* self) {
    char* value = (char*) calloc(1, sizeof(char));
    if (!value) {
        fprintf(stderr, "ERROR: Couldn't callocate in function: %s\n", __func__);
        return NULL;
    };

    bool dot = false;
    while (self->CanAdvance(self) && (
        isdigit(self->c) || (!dot && self->c == '.')
    )) {
        if (self->c == '.') dot = true;
        appendChar(&value, self->c);
        self->Advance(self);
    }
    
    return newToken(value, dot ? TOKEN_FLOAT : TOKEN_INT);
}

static token_t* CollectSymbol (lexer_t* self) {
    char* value = (char*) calloc(1, 2 * sizeof(char));
    if (!value) {
        fprintf(stderr, "ERROR: Couldn't callocate in function: %s\n", __func__);
        return NULL;
    }

    value[0] = self->c;
    self->Advance(self);
    tokenType_t type = findSymbol(value);

    if (self->CanAdvance(self) && isSymbol(self->c)) {
        tokenType_t tType = findSymbol((char[]){value[0], self->c, 0});

        if (tType != TOKEN_UNKNOWN) {
            appendChar(&value, self->c);
            self->Advance(self);
            type = tType;
        }
    }

    return newToken(value, type);
}

static token_t* CollectId (lexer_t* self) {
    char* value = (char*) calloc(1, sizeof(char));
    if (!value) {
        fprintf(stderr, "ERROR: Couldn't callocate in function: %s\n", __func__);
        return NULL;
    }

    while (self->CanAdvance(self) && !isspace(self->c) && !isSymbol(self->c)) {
        appendChar(&value, self->c);
        self->Advance(self);
    }

    if (strcmp(value, "sizeof") == 0) return newToken(value, TOKEN_SIZEOF);
    if (strcmp(value, "as") == 0)     return newToken(value, TOKEN_AS);

    return newToken(value, TOKEN_ID);
}

void Lexer(lexer_t* self, char* src) {
    self->src = src;
    self->src_size = strlen(src);
    self->i = 0;
    self->c = src[self->i];

    self->Advance = Advance;
    self->CanAdvance = CanAdvance;
    self->NextToken = NextToken;
    self->SkipWhitespace = SkipWhitespace;

    self->CollectString = CollectString;
    self->CollectNumber = CollectNumber;
    self->CollectSymbol = CollectSymbol;
    self->CollectId     = CollectId;
}

lexer_t* newLexer(char* src) {
    lexer_t* self = (lexer_t*)malloc(sizeof(lexer_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }
    
    Lexer(self, src);
    return self;
}