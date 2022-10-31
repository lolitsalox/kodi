#include "gen_bytekod.h"
#include "kodio.h"
#include "lexer.h"
#include "kodparser.h"

ByteCode* gen_bytekod(const char* filename) {
    ByteCode* bkod = (ByteCode*)malloc(sizeof(ByteCode));

    bkod->size = 0;
    bkod->code = NULL;

    char *src = io_read(filename);
    
    if (!src) {
        free(bkod->code);
        free(bkod);
        return NULL;
    }
    
    lexer_t* lexer = newLexer(src);
    parser_t* parser = newParser(lexer);

    ast_t* root = parser->Parse(parser);

    root->Print(root, 0);

    free(root);
    free(parser);
    free(lexer);
    

    return bkod;
}