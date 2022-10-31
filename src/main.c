#include "gen_bytekod.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    
    ByteCode* bkod = gen_bytekod("main.kod");
    if (!bkod) return 1;
    
    for (size_t i = 0; i < bkod->size; ++i) {
        printf("%02X ", bkod->code[i]);
    }

    free(bkod->code);
    free(bkod);

    // run_bytekod();
    return 0;
}