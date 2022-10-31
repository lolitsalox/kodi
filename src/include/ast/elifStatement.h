#pragma once

#include "statement.h"
#include "compound.h"

typedef struct astElifStatement_t astElifStatement_t;
struct astElifStatement_t {
    astStatement_t base;
    
    ast_t* condition;   
    astCompound_t* body;
};

void AstElifStatement(astElifStatement_t* self, ast_t* condition, astCompound_t* body);
astElifStatement_t* newAstElifStatement(ast_t* condition, astCompound_t* body);