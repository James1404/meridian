#ifndef MERIDIAN_READER_H
#define MERIDIAN_READER_H

#include "meridian_atom.hpp"
#include "meridian_common.hpp"

struct Reader {
    String src;
    u64 start, position, line;

    Atom global;

    static Reader make(String src);

    void free();
    void run();

private:
    bool eof();

    bool SkipWhitespace();
    void SkipAllWhitespace();

    Atom ReadList();
    Atom ReadSymbol();

    Atom ReadTopLevel();

    bool Match(char expected);

    char Current();

    void Advance();
};

#endif//MERIDIAN_READER_H
