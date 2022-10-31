#pragma once

#include "ast.h"
#include "token.h"

typedef struct astOp_t astOp_t;
struct astOp_t {
    ast_t base;
    tokenType_t op;
};

void AstOp(astOp_t* self, tokenType_t op);
astOp_t* newAstOp(tokenType_t op);