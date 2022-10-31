#include "ast/unOp.h"

#include <stdio.h>
#include <stdlib.h>

static void Print(astUnOp_t* self, size_t indent) {
    INDENT(indent)
    printf("%s - %s\n", astTypeToStr(self->base.base.type), tokenTypeToStr(self->base.op));

    INDENT(indent + 1)
    printf("first:\n");
    self->first->Print(self->first, indent + 2);
}

void AstUnOp(astUnOp_t* self, tokenType_t op, ast_t* first) {
    AstOp(&self->base, op);
    self->base.base.type = AST_UNARY_OP;
    self->base.base.Print = (void (*)(ast_t*, size_t)) Print;

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