#include "ast/unOp.h"

#include <stdio.h>
#include <stdlib.h>

void AstUnOp(astUnOp_t* self, tokenType_t op, ast_t* first) {
    AstOp(&self->base, op);
    self->base.base.type = AST_UNARY_OP;

    self->first = first;
}

astUnOp_t* newAstUnOp(tokenType_t op, ast_t* first) {
    astUnOp_t* self = (astUnOp_t*) malloc(sizeof(astUnOp_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstUnOp(self, op, first);
    return self;
}