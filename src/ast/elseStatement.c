#include "ast/elseStatement.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Print(astElseStatement_t* self, size_t indent) {
    INDENT(indent)
    printf("%s:\n", stype_to_str(self->base.stype));

    ((ast_t*)self->body)->Print((ast_t*)self->body, indent + 1);
}

void AstElseStatement(astElseStatement_t* self, astCompound_t* body) {
    AstStatement(&self->base, STATEMENT_ELSE);
    self->base.base.Print = (void (*)(ast_t*, size_t)) Print;

    self->body = body;
}

astElseStatement_t* newAstElseStatement(astCompound_t* body) {
    astElseStatement_t* self = (astElseStatement_t*) malloc(sizeof(astElseStatement_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstElseStatement(self, body);
    return self;
}