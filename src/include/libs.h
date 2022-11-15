#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

// make clean if u want to update this
#define INDENT_SCALAR 4
#define INDENT(x) for (size_t i = 0; i < (x) * INDENT_SCALAR; ++i) \
                    printf(" ");

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define OPSIZE 1

typedef enum {
    CONST_OBJECT, // any object that is not primitive
    CONST_NAME, // name constant (utf8)
    CONST_CODE, // code object
    CONST_INT,  // int object
    CONST_FLOAT,  // float object
    CONST_STRING,  // string object
    CONST_CLASS, // definition of class
    CONST_METHOD, // some attribute
    CONST_FIELD, // some attribute
} constType_t;

typedef struct {
    constType_t type;
} constInfo_t;

typedef struct {
    char magic[4]; // 'kod\0'
    uint8_t filename_size;
    char* filename;
    uint16_t major_ver;
    uint16_t minor_ver;

    uint32_t const_pool_size;
    constInfo_t** const_pool;
} kod_t;

typedef struct ByteCode {
    size_t size;
    uint8_t* code;
} ByteCode;

typedef struct {
    constInfo_t base;
    uint32_t code_size;
    uint8_t* code;
} codeInfo_t;

typedef struct {
    constInfo_t base;
    uint16_t name_size;
    char* name;
} nameInfo_t;

typedef enum {
    OP_LOAD_CONST,
} OP_t;