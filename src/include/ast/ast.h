#pragma once

#include <stddef.h>

// make clean if u want to update this
#define INDENT_SCALAR 4
#define INDENT(x) for (size_t i = 0; i < (x) * INDENT_SCALAR; ++i) \
                    printf(" ");

typedef enum astType_t {
    AST_NOOP,
    AST_ROOT,
    AST_COMPOUND,
    AST_CALL,
    AST_OP,
    AST_UNARY_OP,
    AST_BIN_OP,
    AST_FUNCTION,
    AST_ASSIGNMENT,
    AST_VARIABLE,
    AST_STATEMENT,
    AST_CONDITIONAL_STATEMENT,
    AST_UNARY_STATEMENT,
    AST_INT,
    AST_FLOAT,
    AST_STRING,
    AST_BOOL,
} astType_t;

typedef struct ast_t ast_t;

struct ast_t {
    astType_t type;

    void (*Print) (ast_t* self, size_t indent);
};

void Ast(ast_t* self, astType_t type);
ast_t* newAst(astType_t type);

const char* astTypeToStr(astType_t type);