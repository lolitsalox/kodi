#pragma once

typedef enum objectType_t {
    OBJECT,
    OBJECT_CLASS,
    OBJECT_INT,
    OBJECT_FLOAT,
    OBJECT_STRING,
    OBJECT_CODE,
    OBJECT_FUNCTION,
    OBJECT_FIELD,
} objectType_t;

typedef struct class_t class_t;

struct class_t {
    objectType_t base;
    char name[];
    size_t attributeSize;
    attribute_t attributes[];
}

/*


struct attribute {
    object_t base;
    flags_t flags; // static, public, private, protected
    
    char name[];
    codeObject_t code;
};

struct flags_t {
    uint8_t static : 1;
    uint8_t publicity : 2; // 00 01 10
    uint8_t 
};

object_t, class_t, int_t, float_t, str_t, T
0, 1, 2, 3, 4, 5
class T:  -> class_t T{1}
    pass
t = T() -> object_t{5}
del T
*/