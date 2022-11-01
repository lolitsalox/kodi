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