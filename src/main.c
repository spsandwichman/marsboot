#define ORBIT_IMPLEMENTATION

#include "mars.h"
#include "crash.h"
#include "sema.h"
#include "emitc.h"

char* strconcat(const char* a, const char* b) {
    char* out = malloc(strlen(a) + strlen(b) + 1);
    memcpy(out, a, strlen(a));
    memcpy(out + strlen(a), b, strlen(b));
    out[strlen(a) + strlen(b)] = '\0';
    return out;
}

struct {
    bool check_only;
} flags;

int main(int argc, char** argv) {
#ifndef _WIN32
    init_signal_handler();
#endif

    if (argc == 1) {
        printf("no file provided\n");
        exit(0);
    }

    for_n(i, 2, argc) {
        char* flag = argv[i];
        if (strcmp(flag, "-check") == 0) {
            flags.check_only = true;
        }
    }

    errno = 0;
    FILE* f = fopen(argv[1], "rb");

    if (f == NULL || errno != 0) {
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
    
    PNode* top = parse_file(tb, NULL_STR);
    string module_name = pnode_span(top->list.at[0]->module_decl.ident);

    // TODO look through this directory for other mars files

    type_init();
    Module* m = sema_check(top);

    string c_code = c_gen(m);
    if (flags.check_only) {
        return 0;
    }
    size_t path_len = strlen(argv[1]);
    char* c_path = strcpy(malloc(path_len + 1), argv[1]);
    c_path[path_len - 4] = 'c';
    c_path[path_len - 3] = '\0';
    FILE* c_file = fopen(c_path, "w+");
    fwrite(c_code.raw, c_code.len, 1, c_file);
    fclose(c_file);
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