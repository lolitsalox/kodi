#pragma once

#include "libs.h"

kod_t* gen_kod(const char* filename);

// typedef struct CodeObjectInfo {
//     uint16_t tag; // code object
//     uint16_t size; // size of the bytecode
//     uint8_t* bytecode;
// } CodeObjectInfo;