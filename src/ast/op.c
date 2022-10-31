#include "ast/op.h"

#include <stdio.h>
#include <stdlib.h>

void AstOp(astOp_t* self, tokenType_t op) {
    Ast(&self->base, AST_OP);

    self->op = op;
}

astOp_t* newAstOp(tokenType_t op) {
    astOp_t* self = (astOp_t*) malloc(sizeof(astOp_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstOp(self, op);
    return self;
}