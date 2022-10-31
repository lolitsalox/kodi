#pragma once

#include "statement.h"
#include "compound.h"

typedef struct astWhileStatement_t astWhileStatement_t;
struct astWhileStatement_t {
    astStatement_t base;
    
    ast_t* condition;   
    astCompound_t* body;
};

void AstWhileStatement(astWhileStatement_t* self, ast_t* condition, astCompound_t* body);
astWhileStatement_t* newAstWhileStatement(ast_t* condition, astCompound_t* body);