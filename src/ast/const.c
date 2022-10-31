#include "ast/const.h"

#include <stdio.h>
#include <stdlib.h>

void AstConst(astConst_t* self, astType_t type, char* value) {
    Ast(&self->base, type);

    self->value = value;
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