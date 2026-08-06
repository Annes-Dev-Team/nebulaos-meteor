#include "fsextra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool import_to_file(const char* realpath, const char* name, const char* ext) {
    FILE* fptr;
    fptr = fopen(realpath, "rb");

    if (!fptr) {
        printf("File %s doesnt exist.\n", realpath);
        return false;
    }

    // get size
    fseek(fptr, 0, SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr); 

    // alloc buffer
    unsigned char *buffer = (unsigned char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        printf("Failed to allocate %li chunks\n", fileSize+1);
        fclose(fptr);
        return false;
    }

    // read real file
    size_t bytesRead = fread(buffer, 1, fileSize, fptr);

    printf("Successfully read %zu bytes.\n", bytesRead);

    // make virtual file
    File *vfile = malloc(sizeof(File));

    vfile->contents = buffer;
    strcpy(vfile->name, name);
    strcpy(vfile->ext, ext);
    vfile->size = bytesRead;

    File_add(vfile);

    fclose(fptr);

    return true;
}
