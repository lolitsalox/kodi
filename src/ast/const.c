#include "ast/const.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Print(astConst_t* self, size_t indent) {
    INDENT(indent)
    printf("%s - %s\n", astTypeToStr(self->base.type), self->value);
}

void AstConst(astConst_t* self, astType_t type, char* value) {
    Ast(&self->base, type);
    self->base.Print = (void (*)(ast_t*, size_t)) Print;

    self->value = (char*) malloc(strlen(value) + 1);
    strcpy(self->value, value);
}

astConst_t* newAstConst(astType_t type, char* value) {
    astConst_t* self = (astConst_t*) malloc(sizeof(astConst_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstConst(self, type, value);
    return self;
}