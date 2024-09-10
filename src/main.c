#include "mars.h"
#include "parse/parse.h"

int main() {
    
    FILE* f = fopen("examples/fn.mars", "rb");
    
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buf = malloc(size + 1);
    memset(buf, 0, size + 1);
    printf("lex %d bytes\n", size);

    fread(buf, 1, size, f);

    TokenBuf tb = lex_string(str(buf), 0);

    foreach(Token t, tb) {
        // if (t.kind == TOK_NEWLINE) continue;
        printf("%s\n", token_kind_name[t.kind]);
    }

    printf("lex %d bytes\n", tb.cap * sizeof(Token));
    da_shrink(&tb);
    printf("lex %d bytes\n", tb.cap * sizeof(Token));
}