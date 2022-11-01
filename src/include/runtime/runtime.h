#pragma once

#include "map.h"

#include <stddef.h>
#include <stdint.h>

typedef struct runtime_t runtime_t;

struct runtime_t {
    // built-ins get poured in, then runtime stuff
    map_t* globals; // <char*, object_t*>
    // kod_t* kod;
    size_t ip;
};

// void Runtime(runtime_t* self, uint8_t* bytecode, size_t size);
// runtime_t* newRuntime(uint8_t* bytecode, size_t size);

// struct kod_t {
//     uint8_t magic[4]; // "kod\0"
//     uint8_t minor_version;
//     uint8_t major_version;
//     uint16_t constant_pool_size;
//     object_t* constant_pool[];
// };

////////////////////////////////////////////////////////
// typedef struct attribute_t attribute_t;
// struct attribute_t {
    
// };

// struct BaseObj_t {
//     objectType_t type;
//     size_t attributesSize;
//     attribute_t attributes[];
// };

// attribute_t IntObjAttributes[] = {};
// struct BaseObj_t IntObj = {
//     OBJECT_INT,
//     sizeof(IntObjAttributes),
//     IntObjAttributes
// };
// globals->Set("int", IntObj);
///////////////////////////////////////////////////////////////////////