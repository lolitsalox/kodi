#pragma once

#include "statement.h"
#include "compound.h"

typedef struct astElseStatement_t astElseStatement_t;
struct astElseStatement_t {
    astStatement_t base;
    
    astCompound_t* body;
};

void AstElseStatement(astElseStatement_t* self, astCompound_t* body);
astElseStatement_t* newAstElseStatement(astCompound_t* body);