#include "meridian_atom.hpp"

#include <stdlib.hpp>
#include <stdio.hpp>

#define DEFINE_ATOMTYPE_TOSTRING(name, type)\
    case ATOM_TYPE_ ## name: return String_make(#name); break;

String AtomType_toString(Atom atom) {
    switch(atom.ty) {
        case ATOM_NUMBER: return String_make("NUMBER");
        case ATOM_BOOLEAN: return String_make("BOOLEAN");
        case ATOM_STRING: return String_make("STRING");
        case ATOM_SYMBOL: return String_make("SYMBOL");
        case ATOM_KEYWORD: return String_make("KEYWORD");
        case ATOM_FN: return String_make("FN");
        case ATOM_INTRINSIC: return String_make("INTRINSIC");
        case ATOM_FFI: return String_make("FFI");
        case ATOM_LIST: return String_make("LIST");
        case ATOM_NIL: return String_make("NIL");
    }

    return String_make("Invalid String");
}

List List_make() {
    return (List) {
        .length = 0,
        .allocated = 0,
        .data = NULL
    };
}

void List_free(List* list) {
    if(list->data) free(list->data);
    *list = List_make();
}

void List_push(List* list, Atom atom) {
    if(!list->data) {
        list->allocated = 8;
        list->data = malloc(sizeof(Atom)*list->allocated);
    }

    list->data[list->length] = atom;
    list->length++;

    if(list->length >= list->allocated) {
        list->allocated *= 2;
        list->data = realloc(list->data, sizeof(Atom)*list->allocated);
    }
}

Atom List_at(List* list, u64 index) {
    return list->data[index];
}

Fn Fn_make() {
    return (Fn) {
        .args_length = 0,
        .args_allocated = 0,
        .args = NULL,
        .body = NULL,
    };
}

void Fn_free(Fn* fn) {
    if(fn->args) free(fn->args);
    *fn = Fn_make();
}

void Fn_push(Fn* fn, String arg) {
    if(!fn->args) {
        fn->args_allocated = 8;
        fn->args = malloc(sizeof(String)*fn->args_allocated);
    }

    fn->args[fn->args_length] = arg;
    fn->args_length++;

    if(fn->args_length >= fn->args_allocated) {
        fn->args_allocated *= 2;
        fn->args = realloc(fn->args, sizeof(String)*fn->args_allocated);
    }
}