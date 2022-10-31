#include "ast/compound.h"

#include <stdio.h>
#include <stdlib.h>

static void Print(astCompound_t* self, size_t indent) {
    INDENT(indent)
    printf("%s\n", astTypeToStr(self->base.type));
    
    for (size_t i = 0; i < self->children->size; ++i) {
        ast_t* child = (ast_t*) self->children->items[i];
        child->Print(child, indent + 1);
    }
}

void AstCompound(astCompound_t* self) {
    Ast(&self->base, AST_COMPOUND);    
    self->base.Print = (void (*)(ast_t*, size_t)) Print;

    self->children = newList(sizeof(ast_t*));
}

astCompound_t* newAstCompound() {
    astCompound_t* self = (astCompound_t*) malloc(sizeof(astCompound_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstCompound(self);
    return self;
}