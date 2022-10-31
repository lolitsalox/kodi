#pragma once

#include "ast.h"

typedef struct astName_t astName_t;
struct astName_t {
    ast_t base;
    
    char* name;
};

void AstName(astName_t* self, char* name);
astName_t* newAstName(char* name);