#include "ast/function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Print(astFunction_t* self, size_t indent) {
    INDENT(indent)
    printf("%s\n", astTypeToStr(self->base.type));
    
    INDENT(indent + 1)
    printf("name: %s\n", self->name);
    
    INDENT(indent + 1)
    printf("Parameters:\n");

    for (size_t i = 0; i < self->parameters->children->size; ++i) {
        ast_t* child = (ast_t*) self->parameters->children->items[i];
        child->Print(child, indent + 2);
    }

    ((ast_t*)self->body)->Print((ast_t*)self->body, indent + 1);
}

void AstFunction(astFunction_t* self, char* name, astCompound_t* parameters, astCompound_t* body) {
    Ast(&self->base, AST_FUNCTION);
    self->base.Print = (void (*)(ast_t*, size_t)) Print;

    self->name = name ? name : "<anonymoys>";

    self->parameters = parameters;
    self->body = body;
}

astFunction_t* newAstFunction(char* name, astCompound_t* parameters, astCompound_t* body) {
    astFunction_t* self = (astFunction_t*) malloc(sizeof(astFunction_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstFunction(self, name, parameters, body);
    return self;
}