#include "meridian_reader.hpp"

#include "meridian_error.hpp"

#include <stdlib.hpp>
#include <stdio.hpp>

bool Reader::eof() {
    return position >= src.length();
}

char Reader::Current() {
    return src[position];
}

void Reader::Advance() {
    position++;
}

bool Reader::Match(char expected) {
    if(Current() == expected) {
        Advance();
        return true;
    }

    return false;
}


static bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

static bool isSymbolCharStart(char c) {
    switch(c) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '_':
        case '#':
        case '@':
        case '?':
        case '>':
        case '<':
        case '!':
        case '=':
        case '$':
        case '%':
        case '^':
        case '&':
            return true;
        default:
            return isLetter(c);
    }
}

static bool isSymbolChar(char c) {
    return isSymbolCharStart(c) || isNumber(c);
}

Reader Reader::make(String src) {
    return (Reader) {
        .src = src,
        .start = 0,
        .position = 0,
        .line = 0,
    };
}

void Reader::free() {
}

bool Reader::SkipWhitespace() {
    char c = Current();

    switch(c) {
        case '\n':
            line++;
        case ' ':
        case '\t':
        case '\r':
            Advance();
            return true;
        default:
            return false;
    }
}

void Reader::SkipAllWhitespace() {
    while(SkipWhitespace());
}

Atom Reader::ReadSymbol() {
    SkipAllWhitespace();

    char c = Current();
    start = position;

    if(isSymbolCharStart(c)) {
        while(isSymbolChar(c)) {
            Advance();
            c = Current();
        }

        String text = src.substr(start, position - start);

        return Atom::Symbol(text);
    }

    return Atom::NIL();
}

Atom Reader::ReadAtom() {
    SkipAllWhitespace();

    char c = Current();
    start = position;

    if(c == '"') {
        do {
            Advance();
        } while(Current());

        u64 strStart = start + 1;
        u64 strLength = (position - start) - 1;

        Advance();

        String text = src.substr(strStart, strLength);
        return Atom::String(text);
    }
    
    if(isNumber(c)) {
        while(isNumber(c) || c == '.') {
            Advance();
            c = Current();
        }

        String text = src.substr(start, position - start);

        return Atom::Number(stold(text, NULL));
    }

    if(c == '(') {
        return ReadList();
    }

    if(isSymbolCharStart(c)) {
        return ReadSymbol();
    }

    printf("ERROR INVALID ATOM :: '%02x'\n", (unsigned char) c);
    return Atom::NIL();
}

Atom Reader::ReadList() {
    SkipAllWhitespace();

    Atom list = Atom::List();

    if(Match('(') {
        while(!Match(')') {
            List_push(&GET_ATOM_LIST(list), ReadAtom();
            SkipAllWhitespace();
        }
    }

    return list;
}

Atom Reader::ReadTopLevel() {
    Atom list = Atom::List();

    while(!eof() {
        List_push(&GET_ATOM_LIST(list), ReadAtom();
        SkipAllWhitespace();
    }

    return list;
}

void Reader::run() {
    global = ReadTopLevel();
}

