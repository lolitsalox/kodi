#pragma once

#include "op.h"

typedef struct astUnOp_t astUnOp_t;
struct astUnOp_t {
    astOp_t base;

    ast_t* first;
};

void AstUnOp(astUnOp_t* self, tokenType_t op, ast_t* first);
astUnOp_t* newAstUnOp(tokenType_t op, ast_t* first);