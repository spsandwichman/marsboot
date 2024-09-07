#include "mars.h"

typedef struct Lexer {
    TokenBuf* tb;
    string text;

    u32 cursor;
} Lexer;

TokenBuf lex_string(string text) {
    if (text.len > UINT32_MAX) {
        CRASH("cannot lex string larger than 4GiB");
    }

    TokenBuf tokenbuf = {0};
    da_init(&tokenbuf, 128);

    Lexer l = {0};
    l.tb = &tokenbuf;
    l.text = text;
    l.cursor = 0;

    TODO("lex");

    return tokenbuf;
}

const char* token_kind_name[] = {
#define TOKEN(enum, str) [enum] = str,
    TOKEN_LIST
#undef TOKEN
};