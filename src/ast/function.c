#include "ast/function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Print(astFunction_t* self, size_t indent) {
    INDENT(indent)
    printf("%s\n", astTypeToStr(self->base.type));
    
    INDENT(indent + 1)
    printf("name: %s\n", self->name);
    
    INDENT(indent + 1)
    printf("Parameters:\n");

    for (size_t i = 0; i < self->parameters->children->size; ++i) {
        ast_t* child = (ast_t*) self->parameters->children->items[i];
        child->Print(child, indent + 2);
    }

    ((ast_t*)self->body)->Print((ast_t*)self->body, indent + 1);
}

static void CodeGen(astFunction_t* self, kod_t* context) {
    // load const code

    // load const name
    // search if name in const pool, if not, add it
    uint32_t index = -1;

    for (size_t i = 0; i < context->const_pool_size; ++i) {
        if (context->const_pool[i]->type == CONST_NAME && 
            strcmp(((nameInfo_t*)context->const_pool[i])->name, self->name) == 0) {
            index = i;
        }
    }

    if (index == -1) {
        index = ++context->const_pool_size;
        context->const_pool = realloc(context->const_pool, index * sizeof(constInfo_t*));
        context->const_pool[index - 1] = malloc(sizeof(nameInfo_t*));
        context->const_pool[index - 1]->type = CONST_NAME;
        ((nameInfo_t*)context->const_pool[index - 1])->name_size = strlen(self->name) + 1;
        ((nameInfo_t*)context->const_pool[index - 1])->name = self->name;
    }

    ((codeInfo_t*)context->const_pool[0])->code_size += OPSIZE + 4; // name_index
    ((codeInfo_t*)context->const_pool[0])->code = realloc(((codeInfo_t*)context->const_pool[0])->code, ((codeInfo_t*)context->const_pool[0])->code_size);

    ((codeInfo_t*)context->const_pool[0])->code[((codeInfo_t*)context->const_pool[0])->code_size - 5] = OP_LOAD_CONST;
    ((codeInfo_t*)context->const_pool[0])->code[((codeInfo_t*)context->const_pool[0])->code_size - 4] = index & 0x000000FF;
    ((codeInfo_t*)context->const_pool[0])->code[((codeInfo_t*)context->const_pool[0])->code_size - 3] = index & 0x0000FF00;
    ((codeInfo_t*)context->const_pool[0])->code[((codeInfo_t*)context->const_pool[0])->code_size - 2] = index & 0x00FF0000;
    ((codeInfo_t*)context->const_pool[0])->code[((codeInfo_t*)context->const_pool[0])->code_size - 1] = index & 0xFF000000;
    // function
}

void AstFunction(astFunction_t* self, char* name, astCompound_t* parameters, astCompound_t* body) {
    Ast(&self->base, AST_FUNCTION);
    self->base.Print = (void (*)(ast_t*, size_t)) Print;
    self->base.CodeGen = (void (*)(ast_t*, kod_t*)) CodeGen;

    self->name = name ? name : "<anonymoys>";

    self->parameters = parameters;
    self->body = body;
}

astFunction_t* newAstFunction(char* name, astCompound_t* parameters, astCompound_t* body) {
    astFunction_t* self = (astFunction_t*) malloc(sizeof(astFunction_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    AstFunction(self, name, parameters, body);
    return self;
}