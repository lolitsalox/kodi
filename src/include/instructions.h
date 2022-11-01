#pragma once

typedef enum op_t {
    OP_LOAD_CONST,
    OP_LOAD_NAME,
    OP_LOAD_STORE,
    OP_BIN_OP,
    OP_UN_OP,
    OP_CJMP,
    OP_JMP,
    OP_FUNCTION,
    OP_RETURN,
    OP_CALL,
} op_t;

/*

LOAD_CONST 
 - u4 index to the constant in the constants pool

LOAD_NAME
- u4 index to the names

STORE_NAME
- u4 index to the names

BIN_OP
 - u1 operator // pops two

UN_OP
 - u1 operator // pops one

CJMP
 - u2 index inside code // pops one

JMP
 - u2 index inside code

FUNCTION // pops two (name, code object)

RETURN // pops last value on the local stack and pushes into the global stack

LOAD_NAME 0  ; function name
LOAD_CONST 0 ; one argument
LOAD_CONST 1 ; other argument
CALL 2       ; call with those two arguments

CALL // pops , pops n



load_const 0 ; loads 1
jz after
load_const 1 ; loads the code object
load_const 2 ; loads its name
function     ; pops the name and code object makes it a function

load_const 2 ; loads the name of the function loads a utf8
load_const 3 ; loads "f" loads a str
"f"()
call         ; calls the callable


load_const 3 -> loads a string object
"f"()

load_const 2 -> loads a function object
f()


{
    0 - type: UTF8, utf8: "f"
    1 - type: STRING, utf8_index: 0
}



load_const 0 ; loads 1
cnjmp after  ; if pop is false, jump to after

if 1 {
    f() {
        if 1 {
            return 0
        }
        else {
            return 1
        }
    }
    f()
}
else {

}






*/

