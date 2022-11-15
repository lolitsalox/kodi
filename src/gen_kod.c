#include "gen_kod.h"
#include "kodio.h"
#include "lexer.h"
#include "kodparser.h"

kod_t* gen_kod(const char* filename) {
    kod_t* context = (kod_t*)malloc(sizeof(kod_t));
    size_t filename_size = strlen(filename) + 1;

    context->filename = (char*) malloc(sizeof(char) * filename_size);

    strcpy_s(context->magic, 4, "kod");
    strcpy_s(context->filename, filename_size, filename);

    context->filename_size = filename_size;
    context->major_ver = 0;
    context->minor_ver = 1;
    context->const_pool_size = 0;
    context->const_pool = NULL;

    char *src = io_read(filename);
    
    if (!src) {
        free(context);
        return NULL;
    }
    
    lexer_t* lexer = newLexer(src);
    parser_t* parser = newParser(lexer);

    ast_t* root = parser->Parse(parser);

    free(parser);
    free(lexer);

    root->Print(root, 0);

    ++context->const_pool_size;
    context->const_pool = malloc(sizeof(constInfo_t*));

    context->const_pool[0] = malloc(sizeof(codeInfo_t*));
    context->const_pool[0]->type = CONST_CODE;
    ((codeInfo_t*)context->const_pool[0])->code_size = 0;
    ((codeInfo_t*)context->const_pool[0])->code = malloc(1);
    ((codeInfo_t*)context->const_pool[0])->code[0] = 0;

    root->CodeGen(root, context);

    free(root);


    FILE* fp = NULL;

    if (!(fp = fopen("main.bkod", "wb"))) {
        fprintf(stderr, "ERROR: Couldn't open main.bkod\n");
    }

    fprintf(fp, "%s%c%c%s%c", 
        context->magic,
        0,
        context->filename_size,
        context->filename,
        0
    );

    fwrite(&context->major_ver, 2, 1, fp);
    fwrite(&context->minor_ver, 2, 1, fp);
    fwrite(&context->const_pool_size, 4, 1, fp);

    for (size_t i = 0; i < context->const_pool_size; ++i) {
        switch (context->const_pool[i]->type) {

            case CONST_CODE: {
                uint32_t code_size = ((codeInfo_t*)context->const_pool[i])->code_size;
                fwrite(&context->const_pool[i]->type, sizeof(constType_t), 1, fp);
                fwrite(&code_size, sizeof(uint32_t), 1, fp);
                fwrite(((codeInfo_t*)context->const_pool[i])->code, code_size, 1, fp);
                break;
            }

            case CONST_NAME: {
                uint32_t name_size = ((nameInfo_t*)context->const_pool[i])->name_size;
                fwrite(&context->const_pool[i]->type, sizeof(constType_t), 1, fp);
                fwrite(&name_size, sizeof(uint16_t), 1, fp);
                fwrite(((nameInfo_t*)context->const_pool[i])->name, name_size, 1, fp);
                break;
            }

            default: printf("can't generate const info for %d\n", context->const_pool[i]->type); break;
        }
        // fwrite(context->const_pool[i], , 1, fp);
    }

    
    fclose(fp);
    
    return context;
}