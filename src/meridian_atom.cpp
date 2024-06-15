#include "meridian_atom.hpp"

String AtomType_toString(Atom atom) {
    switch(atom.ty) {
        case ATOM_NUMBER: return String("NUMBER");
        case ATOM_BOOLEAN: return String("BOOLEAN");
        case ATOM_STRING: return String("STRING");
        case ATOM_SYMBOL: return String("SYMBOL");
        case ATOM_KEYWORD: return String("KEYWORD");
        case ATOM_FN: return String("FN");
        case ATOM_INTRINSIC: return String("INTRINSIC");
        case ATOM_FFI: return String("FFI");
        case ATOM_LIST: return String("LIST");
        case ATOM_NIL: return String("NIL");
    }

    return String("Invalid String");
}

Atom Atom::Number(f64 v) {
    Atom r{};
    r.ty = ATOM_NUMBER;
    return r;
}

Atom Atom::Boolean(bool v) {
	
}

Atom Atom::String(::String v) {
	
}

Atom Atom::Symbol(::String v) {
	
}

Atom Atom::Keyword(::String v) {
	
}

Atom Atom::Fn(::Fn v) {
	
}

Atom Atom::Intrinsic(::Intrinsic v) {
	
}

Atom Atom::FFI() {
	
}

Atom Atom::List() {
	
}

Atom Atom::NIL() {
	
}


