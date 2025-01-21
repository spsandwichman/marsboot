#define ORBIT_IMPLEMENTATION

#include "mars.h"
#include "crash.h"
#include "sema.h"
#include "emitjkl.h"
#include "emitc.h"

int main(int argc, char** argv) {
#ifndef _WIN32
    init_signal_handler();
#endif

    if (argc == 1) {
        printf("no file provided\n");
        exit(0);
    }

    FILE* f = fopen(argv[1], "rb");

    if (f == NULL) {
        printf("cannot open file '%s'\n", argv[1]);
        exit(0);
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buf = malloc(size + 1);
    memset(buf, 0, size + 1);

    fread(buf, 1, size, f);

    ctx_add_file(str(argv[1]), str(buf));

    TokenBuf tb = lex_string(str(buf));
    da_shrink(&tb);
    printf("lex %d tokens (%d B)\n", tb.len, tb.len * sizeof(Token));
    
    PNode* top = parse_file(tb, NULL_STR);

    type_init();
    Module mod = sema_check_module(top);

    // -----------------------------------


    Type vec3 = type_new_record(&mod, TYPE_STRUCT, 3);
    
    type_attach_name(vec3, constr("Vec3"));
    type(vec3)->as_record.at[0].name = constr("x");
    type(vec3)->as_record.at[0].type = TYPE_I64;
    type(vec3)->as_record.at[1].name = constr("y");
    type(vec3)->as_record.at[1].type = TYPE_I64;
    type(vec3)->as_record.at[2].name = constr("z");
    type(vec3)->as_record.at[2].type = TYPE_I64;

    // Type llnode = type_new_record(TYPE_STRUCT, 2);
    // type_attach_name(llnode, constr("LLNode"));
    // type(llnode)->as_record.at[0].name = constr("data");
    // type(llnode)->as_record.at[0].type = TYPE_I64;
    // type(llnode)->as_record.at[1].name = constr("next");
    // type(llnode)->as_record.at[1].type = type_new_ref(TYPE_POINTER, llnode, true);

    // Type foo = type_new_record(TYPE_STRUCT, 2);
    // type_attach_name(foo, constr("Foo"));
    // type(foo)->as_record.at[0].name = constr("vec");
    // type(foo)->as_record.at[0].type = vec3;
    // type(foo)->as_record.at[1].name = constr("node");
    // type(foo)->as_record.at[1].type = llnode;

    // -----------------------------------

    string header = c_header(&mod);

    printf("---------- C ----------\n");
    printf(str_fmt, str_arg(header));
}

Context ctx;

SourceFile ctx_add_file(string path, string text) {
    if (ctx.files.cap == 0) {
        da_init(&ctx.files, 16);
    }
    SourceFile f = {path, text};
    da_append(&ctx.files, f);
    return f;
}

// requires 'text' to be a strict substring of source
SourceFile* ctx_find_from_substring(char* raw, usize len) {
    for_n(i, 0, ctx.files.len) {
        SourceFile* src = &ctx.files.at[i];
        if (src->text.raw <= raw && raw <= src->text.raw + src->text.len) {
            return src;
        }
    }
    return NULL;
}