#define ORBIT_IMPLEMENTATION

#include "mars.h"
#include "crash.h"
#include "sema.h"
#include "emitjkl.h"
#include "emitc.h"

char* strconcat(const char* a, const char* b) {
    char* out = malloc(strlen(a) + strlen(b) + 1);
    memcpy(out, a, strlen(a));
    memcpy(out + strlen(a), b, strlen(b));
    out[strlen(a) + strlen(b)] = '\0';
    return out;
}

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

    TokenBuf tb = lex_string(str(buf), str(argv[1]));
    da_shrink(&tb);
    // printf("lex %d tokens (%d B)\n", tb.len, tb.len * sizeof(Token));
    
    PNode* top = parse_file(tb, NULL_STR);

    type_init();
    Module* m = sema_check_module(top);

    // -----------------------------------

    // Type selfptr = type_new_ref(m, TYPE_POINTER, TYPE_I64, true);
    // type(selfptr)->as_ref.pointee = selfptr;

    // Type vec3 = type_new_record(m, TYPE_STRUCT, 3);
    
    // type_attach_name(vec3, constr("Vec3"));
    // type(vec3)->as_record.at[0].name = constr("x");
    // type(vec3)->as_record.at[0].type = TYPE_I64;
    // type(vec3)->as_record.at[1].name = constr("y");
    // type(vec3)->as_record.at[1].type = TYPE_I64;
    // type(vec3)->as_record.at[2].name = constr("z");
    // type(vec3)->as_record.at[2].type = TYPE_I64;

    // Type llnode = type_new_record(m, TYPE_STRUCT, 2);
    // type_attach_name(llnode, constr("LLNode"));
    // type(llnode)->as_record.at[0].name = constr("data");
    // type(llnode)->as_record.at[0].type = TYPE_I64;
    // type(llnode)->as_record.at[1].name = constr("next");
    // type(llnode)->as_record.at[1].type = type_new_ref(m, TYPE_POINTER, llnode, true);

    // Type foo = type_new_record(m, TYPE_STRUCT, 2);
    // type_attach_name(foo, constr("Foo"));
    // type(foo)->as_record.at[0].name = constr("vec");
    // type(foo)->as_record.at[0].type = vec3;
    // type(foo)->as_record.at[1].name = constr("node");
    // type(foo)->as_record.at[1].type = llnode;

    // -----------------------------------

    string header = c_gen(m);

    // printf("--------------- C ---------------\n");
    printf(str_fmt, str_arg(header));
    // printf("---------------------------------\n");

    // char* h_path = argv[1];
    // size_t path_len = strlen(argv[1]);

    // h_path[path_len - 4] = 'h';
    // h_path[path_len - 3] = '\0';
    // FILE* h_file = fopen(h_path, "w+");
    // fwrite(header.raw, header.len, 1, h_file);
    // fclose(h_file);

    // char* c_path = argv[1];
    // c_path[path_len - 4] = 'c';
    // FILE* c_file = fopen(c_path, "w+");
    // fwrite(body.raw, body.len, 1, c_file);
    // fclose(c_file);
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