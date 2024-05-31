#include "meridian.h"

#include "meridian_common.h"
#include "meridian_error.h"
#include "meridian_parser.h"

#include <stdio.h>
#include <stdlib.h>

void Meridian_run(const char* src) {
    printf("--- INPUT ---\n");
    printf("%s\n", src);

    Parser p = Parser_make(String_make(src));

    Parser_run(&p);

    Parser_free(&p);
}

void Meridian_run_file(const char* path) {
    FILE* file = fopen(path, "r");

    if(!file) {
        Meridian_error("Couldnt not find file");
        return;
    }

    fseek(file, 0, SEEK_END);
    u64 length = ftell(file);

    fseek(file, 0, SEEK_SET);
    char* buffer = malloc(length);

    fread(buffer, 1, length, file);
    fclose(file);

    Meridian_run(buffer);

    free(buffer);
}