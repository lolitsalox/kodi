#include "ast/call.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Print(astCall_t* self, size_t indent) {
    INDENT(indent)
    printf("%s\n", astTypeToStr(self->base.type));
    
    INDENT(indent + 1)
    puts("callable:");
    self->callable->Print(self->callable, indent + 2);
    
    INDENT(indent + 1)
    puts("Arguments:");

    for (size_t i = 0; i < self->arguments->children->size; ++i) {
        ast_t* child = (ast_t*) self->arguments->children->items[i];
        child->Print(child, indent + 2);
    }
}

void AstCall(astCall_t* self, ast_t* callable, astCompound_t* arguments) {
    Ast(&self->base, AST_CALL);
    self->base.Print = (void (*)(ast_t*, size_t)) Print;

    self->callable = callable;
    self->arguments = arguments;
}

astCall_t* newAstCall(ast_t* callable, astCompound_t* arguments) {
    astCall_t* self = (astCall_t*) malloc(sizeof(astCall_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstCall(self, callable, arguments);
    return self;
}