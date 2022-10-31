#include "ast/returnStatement.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Print(astReturnStatement_t* self, size_t indent) {
    INDENT(indent)
    printf("%s:\n", stype_to_str(self->base.stype));
    
    self->value->Print(self->value, indent + 1);
}

void AstReturnStatement(astReturnStatement_t* self, ast_t* value) {
    AstStatement(&self->base, STATEMENT_RETURN);
    self->base.base.Print = (void (*)(ast_t*, size_t)) Print;

    self->value = value;
}

astReturnStatement_t* newAstReturnStatement(ast_t* value) {
    astReturnStatement_t* self = (astReturnStatement_t*) malloc(sizeof(astReturnStatement_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstReturnStatement(self, value);
    return self;
}