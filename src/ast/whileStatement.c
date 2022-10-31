#include "ast/whileStatement.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Print(astWhileStatement_t* self, size_t indent) {
    INDENT(indent)
    printf("%s:\n", stype_to_str(self->base.stype));
    
    INDENT(indent + 1)
    puts("Condition:");
    self->condition->Print(self->condition, indent + 2);

    ((ast_t*)self->body)->Print((ast_t*)self->body, indent + 1);
}

void AstWhileStatement(astWhileStatement_t* self, ast_t* condition, astCompound_t* body) {
    AstStatement(&self->base, STATEMENT_WHILE);
    self->base.base.Print = (void (*)(ast_t*, size_t)) Print;

    self->condition = condition;
    self->body = body;
}

astWhileStatement_t* newAstWhileStatement(ast_t* condition, astCompound_t* body) {
    astWhileStatement_t* self = (astWhileStatement_t*) malloc(sizeof(astWhileStatement_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstWhileStatement(self, condition, body);
    return self;
}