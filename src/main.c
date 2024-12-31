#define ORBIT_IMPLEMENTATION
#include "mars.h"
#include "crash.h"
#include "sema.h"

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

    // foreach(Token t, tb) {
    //     printf("%s ", token_kind_name[t.kind]);
    // }
    // printf("\n");

    parse_file(tb, NULL_STR);

    type_init();
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
    for_range(i, 0, ctx.files.len) {
        SourceFile* src = &ctx.files.at[i];
        if (src->text.raw <= raw && raw <= src->text.raw + src->text.len) {
            return src;
        }
    }
    return NULL;
}
