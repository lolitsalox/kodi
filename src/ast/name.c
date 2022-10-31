#include "ast/name.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Print(astName_t* self, size_t indent) {
    INDENT(indent)
    printf("%s - %s\n", astTypeToStr(self->base.type), self->name);
}

void AstName(astName_t* self, char* name) {
    Ast(&self->base, AST_NAME);
    self->base.Print = (void (*)(ast_t*, size_t)) Print;

    self->name = name;
}

astName_t* newAstName(char* name) {
    astName_t* self = (astName_t*) malloc(sizeof(astName_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstName(self, name);
    return self;
}