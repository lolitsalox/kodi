#include "ast/binOp.h"

#include <stdio.h>
#include <stdlib.h>

static void Print(astBinOp_t* self, size_t indent) {
    INDENT(indent)
    printf("%s - %s\n", astTypeToStr(self->base.base.type), tokenTypeToStr(self->base.op));

    INDENT(indent + 1)
    printf("first:\n");
    self->first->Print(self->first, indent + 2);
    
    INDENT(indent + 1)
    printf("second:\n");
    self->second->Print(self->second, indent + 2);
}

void AstBinOp(astBinOp_t* self, tokenType_t op, ast_t* first, ast_t* second) {
    AstOp(&self->base, op);
    self->base.base.type = AST_BIN_OP;
    self->base.base.Print = (void (*)(ast_t*, size_t)) Print;

    self->first = first;
    self->second = second;
}

astBinOp_t* newAstBinOp(tokenType_t op, ast_t* first, ast_t* second) {
    astBinOp_t* self = (astBinOp_t*) malloc(sizeof(astBinOp_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstBinOp(self, op, first, second);
    return self;
}