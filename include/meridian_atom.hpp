#ifndef MERIDIAN_ATOM_H
#define MERIDIAN_ATOM_H

#include <functional>

#include "meridian_common.hpp"

struct Atom;

using List = ArrayList<Atom>;

struct Fn {
    ArrayList<Atom*> args;
    Atom* body = NULL;
}; 

using Intrinsic = std::function<Atom(List)>;

struct FFI_Func {
    void* ptr;
};

enum AtomType {
    ATOM_NUMBER,
    ATOM_BOOLEAN,
    ATOM_STRING,
    ATOM_SYMBOL,
    ATOM_KEYWORD,
    ATOM_FN,
    ATOM_INTRINSIC,
    ATOM_FFI,
    ATOM_LIST,
    ATOM_NIL,
};

struct Atom {
    AtomType ty = ATOM_NIL;
    union {
        f64 number;
        bool boolean;
        String string;
        Fn fn;
        List list;
        Intrinsic intrinsic;
        FFI_Func ffi;
    } as;

    Atom() = default;
    Atom(Atom& other) = default;

    static Atom Number(f64 v);
    static Atom Boolean(bool v);
    static Atom String(::String v);
    static Atom Symbol(::String v);
    static Atom Keyword(::String v);
    static Atom Fn(::Fn v);
    static Atom Intrinsic(::Intrinsic v);
    static Atom FFI();
    static Atom List();
    static Atom NIL();
};
//
// --- Getters ---
//

#define GET_ATOM_NUMBER(atom) ((atom).as.number)
#define GET_ATOM_BOOLEAN(atom) ((atom).as.boolean)
#define GET_ATOM_STRING(atom) ((atom).as.string)
#define GET_ATOM_SYMBOL(atom) ((atom).as.string)
#define GET_ATOM_KEYWORD(atom) ((atom).as.string)
#define GET_ATOM_FN(atom) ((atom).as.fn)
#define GET_ATOM_INTRINSIC(atom) ((atom).as.intrinsic)
#define GET_ATOM_FFI(atom) ((atom).as.ffi)
#define GET_ATOM_LIST(atom) ((atom).as.list)

#endif//MERIDIAN_ATOM_H
