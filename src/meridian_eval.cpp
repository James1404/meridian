#include "meridian_eval.hpp"
#include "meridian_error.hpp"
#include "meridian_env.hpp"

#include <stdlib.hpp>

void Eval_run(Atom atom) {
    Eval_Atom(atom);
}

Atom Eval_Atom(Atom atom) {
    switch(atom.ty) {
        case ATOM_SYMBOL: {
            return Eval_Atom(Env_get(GET_ATOM_SYMBOL(atom)));
        } break;
        case ATOM_LIST: return Eval_List(atom);

        case ATOM_NUMBER:
        case ATOM_BOOLEAN:
        case ATOM_STRING:
        case ATOM_NIL:
        case ATOM_KEYWORD:
        case ATOM_FN:
        case ATOM_INTRINSIC:
        case ATOM_FFI:
            return atom;
    }

    return ATOM_NIL();
}

Atom Eval_List(Atom atom) {
    Atom predicate = List_at(&GET_ATOM_LIST(atom), 0);

    if(predicate.ty == ATOM_SYMBOL) {
        if(String_is(GET_ATOM_SYMBOL(predicate), "def")) {
            return Eval_Def(atom);
        }

        if(String_is(GET_ATOM_SYMBOL(predicate), "fn")) {
            return Eval_Fn(atom);
        }
    }

    for(u64 i = 0; i < GET_ATOM_LIST(atom).length; i++) {
        GET_ATOM_LIST(atom).data[i] = Eval_Atom(GET_ATOM_LIST(atom).data[i]);
    }

    Atom fn = GET_ATOM_LIST(atom).data[0];
    if(fn.ty == ATOM_INTRINSIC) {
        List list = List_make();

        for(u64 i = 1; i < GET_ATOM_LIST(atom).length; i++) {
            List_push(&list, GET_ATOM_LIST(atom).data[i]);
        }

        return GET_ATOM_INTRINSIC(fn)(list);
    }

    if(fn.ty == ATOM_FN) {
        List list = List_make();

        for(u64 i = 1; i < GET_ATOM_LIST(atom).length; i++) {
            List_push(&list, GET_ATOM_LIST(atom).data[i]);
        }

        Env_inc();

        Atom result = ATOM_NIL();

        if(list.length != GET_ATOM_FN(fn).args_length) {
            MERIDIAN_ERROR("Invalid amount of arguments");
            return ATOM_NIL();
        }

        for(u64 i = 0; i < list.length; i++) {
            String name = GET_ATOM_FN(fn).args[i];
            Env_set(name, list.data[i]);
        }

        result = Eval_Atom(*GET_ATOM_FN(fn).body);

        Env_dec();

        return result;
    }

    if(GET_ATOM_LIST(atom).length == 1)
        return GET_ATOM_LIST(atom).data[0];

    return atom;
}

bool Eval_match(Atom atom, const char* expected) {
    if(atom.ty == ATOM_SYMBOL) {
        String sym = GET_ATOM_SYMBOL(atom);
        return String_is(sym, expected);
    }

    return false;
}

Atom Eval_Def(Atom atom) {
    if(atom.ty != ATOM_LIST) return ATOM_NIL();

    List list = GET_ATOM_LIST(atom);

    if(list.length != 3) {
        MERIDIAN_ERROR("invalid definition");
        return ATOM_NIL();
    }

    if(Eval_match(list.data[0], "def")) {
        if(list.data[1].ty == ATOM_SYMBOL) {
            Env_set(GET_ATOM_SYMBOL(list.data[1]), list.data[2]);
        }
    }
    
    return ATOM_NIL();
}

Atom Eval_Fn(Atom atom) {
    if(atom.ty != ATOM_LIST) return ATOM_NIL();
    if(!Eval_match(GET_ATOM_LIST(atom).data[0], "fn")) return ATOM_NIL();

    Atom args = GET_ATOM_LIST(atom).data[1];
    Atom body = GET_ATOM_LIST(atom).data[2];
    if(args.ty != ATOM_LIST) return ATOM_NIL();

    Atom result = ATOM_FN();

    for(u64 i = 0; i < GET_ATOM_LIST(args).length; i++) {
        Atom arg = GET_ATOM_LIST(args).data[i];
        if(arg.ty != ATOM_SYMBOL) {
            MERIDIAN_ERROR("function parameters must all be symbols");
            return ATOM_NIL();
        }

        Fn_push(&GET_ATOM_FN(result), GET_ATOM_STRING(arg));
    }

    GET_ATOM_FN(result).body = malloc(sizeof(Atom));
    GET_ATOM_FN(result).body = memcpy(GET_ATOM_FN(result).body, &body, sizeof(Atom));
    
    return result;
}