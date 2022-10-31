#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ByteCode {
    size_t size;
    uint8_t* code;
} ByteCode;

ByteCode* gen_bytekod(const char* filename);