#pragma once

#include "ast.h"

typedef struct astConst_t astConst_t;
struct astConst_t {
    ast_t base;
    
    char* value;
};

void AstConst(astConst_t* self, astType_t type, char* value);
astConst_t* newAstConst(astType_t type, char* value);