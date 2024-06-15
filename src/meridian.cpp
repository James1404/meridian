#include "meridian.hpp"

#include "meridian_allocator.hpp"
#include "meridian_common.hpp"
#include "meridian_env.hpp"
#include "meridian_log.hpp"
#include "meridian_eval.hpp"
#include "meridian_printer.hpp"
#include "meridian_reader.hpp"

void Meridian_init() {
    MainAllocator_init();
    Env_init();
}

void Meridian_free() {
    Env_free();
    MainAllocator_free();
}

#define BUILTIN(name, fn) Env_set(String(name), ATOM::INTRINSIC(&fn))
#define GLOBAL(name, value) Env_set(String(name), value)

Atom meridian_add(List args) {
    Atom result = Atom::Number(0);

    for(u64 i = 0; i < args.size(); i++) {
        if(args[i].ty != ATOM_NUMBER)
            return Atom::NIL();

        // TODO: Add type checking for args (aka, make sure their all numbers)
        GET_ATOM_NUMBER(result) += GET_ATOM_NUMBER(args[i]);
    }

    return result;
}

Atom meridian_sub(List args) {
    Atom result = args[0];

    for(u64 i = 1; i < args.size(); i++) {
        if(args[i].ty != ATOM_NUMBER)
            return Atom::NIL();

        // TODO: Add type checking for args (aka, make sure their all numbers)
        GET_ATOM_NUMBER(result) -= GET_ATOM_NUMBER(args[i]);
    }

    return result;
}

Atom meridian_mul(List args) {
    Atom result = args[0];

    for(u64 i = 1; i < args.size(); i++) {
        if(args[i].ty != ATOM_NUMBER)
            return Atom::NIL();

        // TODO: Add type checking for args (aka, make sure their all numbers)
        GET_ATOM_NUMBER(result) *= GET_ATOM_NUMBER(args[i]);
    }

    return result;
}

Atom meridian_div(List args) {
    Atom result = args[0];

    for(u64 i = 1; i < args.size(); i++) {
        if(args[i].ty != ATOM_NUMBER)
            return Atom::NIL();

        // TODO: Add type checking for args (aka, make sure their all numbers)
        GET_ATOM_NUMBER(result) /= GET_ATOM_NUMBER(args[i]);
    }

    return result;
}

Atom meridian_eq(List args) {
    if(args.size() != 2) {
        Meridian_error("expected two arguments to compare");
        return Atom::NIL();
    }

    Atom lhs = args[0];
    Atom rhs = args[1];

    if(lhs.ty != rhs.ty) {
        Meridian_error("Arguments must be the same type");
        return Atom::NIL();
    }

    switch(lhs.ty) {
        case ATOM_NUMBER: return Atom::Boolean(GET_ATOM_NUMBER(lhs) == GET_ATOM_NUMBER(rhs));
        case ATOM_BOOLEAN: return Atom::Boolean(GET_ATOM_BOOLEAN(lhs) == GET_ATOM_BOOLEAN(rhs));
        case ATOM_STRING: return Atom::Boolean(String_cmp(GET_ATOM_STRING(lhs), GET_ATOM_STRING(rhs)));
        case ATOM_SYMBOL: return Atom::Boolean(String_cmp(GET_ATOM_STRING(lhs), GET_ATOM_STRING(rhs)));
        case ATOM_KEYWORD: return Atom::Boolean(String_cmp(GET_ATOM_STRING(lhs), GET_ATOM_STRING(rhs)));
        default: return Atom::Boolean(false);
    }

    return Atom::Boolean(false);
}

Atom meridian_not(List args) {
    if(args.size() != 1) {
        Meridian_error("expected one argument");
        return Atom::NIL();
    }

    Atom arg = args[0];

    if(arg.ty != ATOM_BOOLEAN) {
        Meridian_error("Arguments must be a boolean");
        return Atom::NIL();
    }

    return Atom::Boolean(!GET_ATOM_BOOLEAN(arg));
}

Atom meridian_head(List args) {
    if(args.size() != 1) {
        Meridian_error("expected a single list");
        return Atom::NIL();
    }

    Atom arg = args[0];

    if(arg.ty != ATOM_LIST) return arg;

    return GET_ATOM_LIST(arg).data[0];
}

Atom meridian_tail(List args) {
    if(args.size() != 1) {
        Meridian_error("expected a single list");
        return Atom::NIL();
    }

    Atom arg = args[0];

    if(arg.ty != ATOM_LIST) return arg;

    return GET_ATOM_LIST(arg).data[GET_ATOM_LIST(arg).size() - 1];
}

Atom meridian_concat(List args) {
    if(args.size() != 1) {
        Meridian_error("expected one arg");
        return Atom::NIL();
    }

    Atom arg = args[0];

    if(arg.ty != ATOM_LIST) return arg;

    return GET_ATOM_LIST(arg).data[GET_ATOM_LIST(arg).size() - 1];
}

Atom meridian_println(List args) {
    if(args.size() == 0) {
        Meridian_error("expected one arg");
        return Atom::NIL();
    }

    for(u64 i = 0; i < args.size(); i++) {
        Printer_Atom(args[i]);
    }

    printf("\n");

    return Atom::NIL();
}

void Meridian_builtin() {
    BUILTIN("+", meridian_add);
    BUILTIN("-", meridian_sub);
    BUILTIN("*", meridian_mul);
    BUILTIN("/", meridian_div);

    BUILTIN("=", meridian_eq);
    BUILTIN("!", meridian_not);

    GLOBAL("true", ATOM_BOOLEAN(true));
    GLOBAL("false", ATOM_BOOLEAN(false));

    BUILTIN("println", meridian_println);

    BUILTIN("head", meridian_head);
    BUILTIN("tail", meridian_tail);
}

#define DEBUG_PRINT
#undef DEBUG_PRINT

void Meridian_run(const char* src) {
#ifdef DEBUG_PRINT
    printf("--- INPUT ---\n");
    printf("%s\n", src);
#endif//DEBUG_PRINT

    Reader reader = Reader_make(String_make(src));

    Reader_run(&reader);

    Atom final = Eval_Atom(reader.global);
#ifdef DEBUG_PRINT
    printf("--- Output ---\n");

    Printer_Atom(final);
    printf("\n");
#else
    (void)final;
#endif//DEBUG_PRINT

    Reader_free(&reader);
}

void Meridian_run_file(const char* path) {
    FILE* file = fopen(path, "r");

    if(!file) {
        Meridian_error("Couldnt not find file");
        return;
    }

    fseek(file, 0, SEEK_END);
    u64 size() = ftell(file);

    fseek(file, 0, SEEK_SET);
    char* buffer = MainAllocator_malloc(size());

    fread(buffer, 1, size(), file);
    fclose(file);

    Meridian_run(buffer);
}
