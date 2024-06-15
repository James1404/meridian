#include "meridian.hpp"

#include "meridian_allocator.hpp"
#include "meridian_common.hpp"
#include "meridian_env.hpp"
#include "meridian_error.hpp"
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

#define BUILTIN(name, fn) Env_set(String_make(name), ATOM_INTRINSIC(&fn))
#define GLOBAL(name, value) Env_set(String_make(name), value)

Atom meridian_add(List args) {
    Atom result = ATOM_NUMBER(0);

    for(u64 i = 0; i < args.length; i++) {
        if(args.data->ty != ATOM_NUMBER)
            return ATOM_NIL();

        // TODO: Add type checking for args (aka, make sure their all numbers)
        GET_ATOM_NUMBER(result) += GET_ATOM_NUMBER(args.data[i]);
    }

    return result;
}

Atom meridian_sub(List args) {
    Atom result = args.data[0];

    for(u64 i = 1; i < args.length; i++) {
        if(args.data->ty != ATOM_NUMBER)
            return ATOM_NIL();

        // TODO: Add type checking for args (aka, make sure their all numbers)
        GET_ATOM_NUMBER(result) -= GET_ATOM_NUMBER(args.data[i]);
    }

    return result;
}

Atom meridian_mul(List args) {
    Atom result = args.data[0];

    for(u64 i = 1; i < args.length; i++) {
        if(args.data->ty != ATOM_NUMBER)
            return ATOM_NIL();

        // TODO: Add type checking for args (aka, make sure their all numbers)
        GET_ATOM_NUMBER(result) *= GET_ATOM_NUMBER(args.data[i]);
    }

    return result;
}

Atom meridian_div(List args) {
    Atom result = args.data[0];

    for(u64 i = 1; i < args.length; i++) {
        if(args.data->ty != ATOM_NUMBER)
            return ATOM_NIL();

        // TODO: Add type checking for args (aka, make sure their all numbers)
        GET_ATOM_NUMBER(result) /= GET_ATOM_NUMBER(args.data[i]);
    }

    return result;
}

Atom meridian_eq(List args) {
    if(args.length != 2) {
        MERIDIAN_ERROR("expected two arguments to compare");
        return ATOM_NIL();
    }

    Atom lhs = args.data[0];
    Atom rhs = args.data[1];

    if(lhs.ty != rhs.ty) {
        MERIDIAN_ERROR("Arguments must be the same type");
        return ATOM_NIL();
    }

    switch(lhs.ty) {
        case ATOM_NUMBER: return ATOM_BOOLEAN(GET_ATOM_NUMBER(lhs) == GET_ATOM_NUMBER(rhs));
        case ATOM_BOOLEAN: return ATOM_BOOLEAN(GET_ATOM_BOOLEAN(lhs) == GET_ATOM_BOOLEAN(rhs));
        case ATOM_STRING: return ATOM_BOOLEAN(String_cmp(GET_ATOM_STRING(lhs), GET_ATOM_STRING(rhs)));
        case ATOM_SYMBOL: return ATOM_BOOLEAN(String_cmp(GET_ATOM_STRING(lhs), GET_ATOM_STRING(rhs)));
        case ATOM_KEYWORD: return ATOM_BOOLEAN(String_cmp(GET_ATOM_STRING(lhs), GET_ATOM_STRING(rhs)));
        default: return ATOM_BOOLEAN(false);
    }

    return ATOM_BOOLEAN(false);
}

Atom meridian_not(List args) {
    if(args.length != 1) {
        MERIDIAN_ERROR("expected one argument");
        return ATOM_NIL();
    }

    Atom arg = args.data[0];

    if(arg.ty != ATOM_BOOLEAN) {
        MERIDIAN_ERROR("Arguments must be a boolean");
        return ATOM_NIL();
    }

    return ATOM_BOOLEAN(!GET_ATOM_BOOLEAN(arg));
}

Atom meridian_head(List args) {
    if(args.length != 1) {
        MERIDIAN_ERROR("expected a single list");
        return ATOM_NIL();
    }

    Atom arg = args.data[0];

    if(arg.ty != ATOM_LIST) return arg;

    return GET_ATOM_LIST(arg).data[0];
}

Atom meridian_tail(List args) {
    if(args.length != 1) {
        MERIDIAN_ERROR("expected a single list");
        return ATOM_NIL();
    }

    Atom arg = args.data[0];

    if(arg.ty != ATOM_LIST) return arg;

    return GET_ATOM_LIST(arg).data[GET_ATOM_LIST(arg).length - 1];
}

Atom meridian_concat(List args) {
    if(args.length != 1) {
        MERIDIAN_ERROR("expected one arg");
        return ATOM_NIL();
    }

    Atom arg = args.data[0];

    if(arg.ty != ATOM_LIST) return arg;

    return GET_ATOM_LIST(arg).data[GET_ATOM_LIST(arg).length - 1];
}

Atom meridian_println(List args) {
    if(args.length == 0) {
        MERIDIAN_ERROR("expected one arg");
        return ATOM_NIL();
    }

    for(u64 i = 0; i < args.length; i++) {
        Printer_Atom(args.data[i]);
    }

    printf("\n");

    return ATOM_NIL();
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
        MERIDIAN_ERROR("Couldnt not find file");
        return;
    }

    fseek(file, 0, SEEK_END);
    u64 length = ftell(file);

    fseek(file, 0, SEEK_SET);
    char* buffer = MainAllocator_malloc(length);

    fread(buffer, 1, length, file);
    fclose(file);

    Meridian_run(buffer);
}
