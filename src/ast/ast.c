#include "ast/ast.h"

#include <stdio.h>
#include <stdlib.h>

static const char* astTypeNames[] = {
    "AST_NOOP",
    "AST_ROOT",
    "AST_COMPOUND",
    "AST_CALL",
    "AST_OP",
    "AST_UNARY_OP",
    "AST_BIN_OP",
    "AST_FUNCTION",
    "AST_ASSIGNMENT",
    "AST_NAME",
    "AST_STATEMENT",
    "AST_CONDITIONAL_STATEMENT",
    "AST_UNARY_STATEMENT",
    "AST_INT",
    "AST_FLOAT",
    "AST_STRING",
    "AST_BOOL",
};

static void Print(ast_t* self, size_t indent) {
    for (size_t i = 0; i < indent * INDENT_SCALAR; ++i)
        printf(" ");
    printf("%s\n", astTypeToStr(self->type));
}

void Ast(ast_t* self, astType_t type) {
    self->type = type;

    self->Print = Print;
}

ast_t* newAst(astType_t type) {
    ast_t* self = (ast_t*) malloc(sizeof(ast_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    Ast(self, type);
    return self;
}

const char* astTypeToStr(astType_t type) {
    if (type < ARRAYSIZE(astTypeNames) - 1)
        return astTypeNames[type];

    return astTypeNames[ARRAYSIZE(astTypeNames) - 1];
}