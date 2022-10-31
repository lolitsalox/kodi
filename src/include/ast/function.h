#pragma once

#include "compound.h"

typedef struct astFunction_t astFunction_t;

struct astFunction_t {
    ast_t base; // the base is the body of the function

    char* name;
    astCompound_t* parameters;
    astCompound_t* body;
};

void AstFunction(astFunction_t* self, char* name, astCompound_t* parameters, astCompound_t* body);
astFunction_t* newAstFunction(char* name, astCompound_t* parameters, astCompound_t* body);