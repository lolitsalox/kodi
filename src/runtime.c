#include "runtime/runtime.h"

#include "gen_kod.h"
#include "kodio.h"

#include <stdio.h>
#include <stdlib.h>

void run(const char* filename) {
    kod_t context;

    FILE* fp;

    if (!(fp = fopen(filename, "rb"))) {
        fprintf(stderr, "ERROR: Couldn't open %s\n", filename);
    }

    fread(context.magic, 4, 1, fp);
    fread(&context.filename_size, 1, 1, fp);

    context.filename = malloc(context.filename_size);

    fread(context.filename, context.filename_size, 1, fp);
    fread(&context.major_ver, sizeof(uint16_t), 1, fp);
    fread(&context.minor_ver, sizeof(uint16_t), 1, fp);
    fread(&context.const_pool_size, sizeof(uint32_t), 1, fp);

    context.const_pool = malloc(context.const_pool_size);

    for (size_t i = 0; i < context.const_pool_size; ++i) {
        constInfo_t constInfo;
        fread(&constInfo.type, sizeof(constInfo_t), 1, fp);

        switch (constInfo.type) {
            case CONST_CODE: 
                context.const_pool[i] = malloc(sizeof(codeInfo_t));
                fread(&((codeInfo_t*)context.const_pool[i])->code_size, sizeof(uint32_t), 1, fp);
                ((codeInfo_t*)context.const_pool[i])->code = malloc(((codeInfo_t*)context.const_pool[i])->code_size);
                fread(&((codeInfo_t*)context.const_pool[i])->code, ((codeInfo_t*)context.const_pool[i])->code_size, 1, fp);
                break;

            case CONST_NAME: 
                context.const_pool[i] = malloc(sizeof(nameInfo_t));
                fread(&((nameInfo_t*)context.const_pool[i])->name_size, sizeof(uint16_t), 1, fp);
                ((nameInfo_t*)context.const_pool[i])->name = malloc(((nameInfo_t*)context.const_pool[i])->name_size);
                fread(((nameInfo_t*)context.const_pool[i])->name, ((nameInfo_t*)context.const_pool[i])->name_size, 1, fp);
                break;

            default: printf("can't read const info %d\n", constInfo.type); exit(1);
        }

        context.const_pool[i]->type = constInfo.type;
    }

    fclose(fp);
    
    printf("magic: %s\nfilename_size: %d\nfilename: %s\nmajor_ver: %hu\nminor_ver: %hu\nconst_pool_size: %u\n",
        context.magic,
        context.filename_size,
        context.filename,
        context.major_ver,
        context.minor_ver,
        context.const_pool_size
    );

    for (size_t i = 0; i < context.const_pool_size; ++i) {
        constInfo_t* constInfo = context.const_pool[i];

        switch (constInfo->type) {
            case CONST_CODE: 
                printf("CODE: {\n  size: %d,\ncode: ", ((codeInfo_t*)constInfo)->code_size);
                // printf("%d ", ((codeInfo_t*)constInfo)->code[0]);

                // for (size_t i = 0; i < ((codeInfo_t*)constInfo)->code_size; ++i) {
                //     printf("%c ", ((codeInfo_t*)constInfo)->code[i]);
                // }
                // printf("\n}\n");
                break;

            case CONST_NAME: 
                printf("NAME: {\n  size: %d,\n  name: %s\n}\n", ((nameInfo_t*)constInfo)->name_size, ((nameInfo_t*)constInfo)->name);
                break;

            default: printf("can't read const info %d\n", constInfo->type); break;
        }
    }

}