#include "kodio.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

char* io_read(const char* filename) {
    FILE* fp; 
    long fsize = 0;

    if (!(fp = fopen(filename, "rb"))) {
        fprintf(stderr, "ERROR: Couldn't open %s\n", filename);
        return NULL;   
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    rewind(fp);

    char* buffer = (char*) malloc(fsize * sizeof(char) + 1);
    if (fread(buffer, fsize, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Couldn't read %s\n", filename);
        fclose(fp);
        free(buffer);
        return NULL;
    }

    buffer[fsize] = 0;

    fclose(fp);
    return buffer;
}

void io_write(const char* filename, const char* src) {
    FILE* fp = NULL;

    if (!(fp = fopen(filename, "wb"))) {
        fprintf(stderr, "ERROR: Couldn't open %s\n", filename);
        return;
    }

    if (fwrite(src, strlen(src), 1, fp) != 1)
        fprintf(stderr, "ERROR: Couldn't write to %s\n", filename);
    
    fclose(fp);
}
