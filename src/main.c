#include "mars.h"
#include "parse/parse.h"

int main() {
    
    FILE* f = fopen("examples/fn.mars", "rb");
    static char buf[5000] = {0};
    fread(buf, 1, sizeof(buf), f);

    TokenBuf tb = lex_string(str(buf), 0);

    foreach(Token t, tb) {
        // if (t.kind == TOK_NEWLINE) continue;
        printf("%s\n", token_kind_name[t.kind]);
    }
}