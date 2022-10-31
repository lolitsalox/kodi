#include "ast/statement.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *statementTypeNames[] = {
    "STATEMENT_RETURN",
    "STATEMENT_IF",
    "STATEMENT_ELIF",
    "STATEMENT_ELSE",
    "STATEMENT_WHILE",
    "STATEMENT_UNKNOWN"
};

char* stype_to_str(statementType_t stype) {
    if (stype < ARRAYSIZE(statementTypeNames) - 1)
        return statementTypeNames[stype];
    return statementTypeNames[ARRAYSIZE(statementTypeNames) - 1];
}

statementType_t str_to_stype(const char* str) {
    if (strcmp(str, "return") == 0)     return STATEMENT_RETURN;
    if (strcmp(str, "if") == 0)         return STATEMENT_IF;
    if (strcmp(str, "elif") == 0)       return STATEMENT_ELIF;
    if (strcmp(str, "else") == 0)       return STATEMENT_ELSE;
    if (strcmp(str, "while") == 0)      return STATEMENT_WHILE;
    return STATEMENT_UNKNOWN;
}

void AstStatement(astStatement_t* self, statementType_t stype) {
    Ast(&self->base, AST_STATEMENT);
    
    self->stype = stype;
}

astStatement_t* newAstStatement(statementType_t stype) {
    astStatement_t* self = (astStatement_t*) malloc(sizeof(astStatement_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstStatement(self, stype);
    return self;
}