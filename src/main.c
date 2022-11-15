#include "gen_kod.h"
#include "runtime/runtime.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    
    kod_t* kod = gen_kod("main.kod");
    if (!kod) return 1;
    
    // for (size_t i = 0; i < kod->size; ++i) {
    //     printf("%02X ", kod->code[i]);
    // }

    free(kod);

    run("main.bkod");

    return 0;
}