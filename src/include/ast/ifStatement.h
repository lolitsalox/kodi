#pragma once

#include "statement.h"
#include "compound.h"

typedef struct astIfStatement_t astIfStatement_t;
struct astIfStatement_t {
    astStatement_t base;
    
    ast_t* condition;       // false-y or truthy
    astCompound_t* body;    // a compound of anything
};

void AstIfStatement(astIfStatement_t* self, ast_t* condition, astCompound_t* body);
astIfStatement_t* newAstIfStatement(ast_t* condition, astCompound_t* body);