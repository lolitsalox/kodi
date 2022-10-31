#pragma once

#include "op.h"

typedef struct astBinOp_t astBinOp_t;
struct astBinOp_t {
    astOp_t base;

    ast_t* first;
    ast_t* second;
};

void AstBinOp(astBinOp_t* self, tokenType_t op, ast_t* first, ast_t* second);
astBinOp_t* newAstBinOp(tokenType_t op, ast_t* first, ast_t* second);