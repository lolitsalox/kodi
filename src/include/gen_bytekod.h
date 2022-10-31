#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ByteCode {
    size_t size;
    uint8_t* code;
} ByteCode;

ByteCode* gen_bytekod(const char* filename);

// typedef struct CodeObjectInfo {
//     uint16_t tag; // code object
//     uint16_t size; // size of the bytecode
//     uint8_t* bytecode;
// } CodeObjectInfo;