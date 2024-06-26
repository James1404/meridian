#ifndef MERIDIAN_READER_H
#define MERIDIAN_READER_H

#include "meridian_atom.h"
#include "meridian_string.h"
#include "meridian_common.h"

typedef struct {
    String src;
    u64 start, position, line, line_pos;

    Atom global;
} Reader;

Reader Reader_make(String src);
void Reader_free(Reader* reader);

void Reader_run(Reader* reader);

#endif//MERIDIAN_READER_H
