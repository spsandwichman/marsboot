#include "parse.h"

typedef struct Lexer {
    TokenBuf* tb;
    string text;
    u16 file_index;
    u32 cursor;
    char current;
} Lexer;

const char* token_kind_name[] = {
#define TOKEN(enum, str) [enum] = str,
    TOKEN_LIST
#undef TOKEN
};

static void tokenize(Lexer* l);

TokenBuf lex_string(string text, u16 file_index) {
    if (text.len > UINT32_MAX) {
        CRASH("cannot lex string larger than 4GiB");
    }

    TokenBuf tokenbuf = {0};
    da_init(&tokenbuf, 128);

    Lexer l = {0};
    l.file_index = file_index;
    l.tb = &tokenbuf;
    l.text = text;
    l.cursor = 0;
    l.current = text.raw[0];

    tokenize(&l);

    return tokenbuf;
}

static void add_token(Lexer* l, u8 kind) {
    Token t;
    t.file_index = l->file_index;
    t.src_offset = l->cursor;
    t.kind = kind;
    da_append(l->tb, t);
}

static forceinline void advance(Lexer* l) {
    l->cursor++;
    if (l->cursor > l->text.len) l->cursor = l->text.len;
    l->current = l->text.raw[l->cursor];
}

static forceinline void advance_n(Lexer* l, int n) {
    l->cursor += n;
    if (l->cursor > l->text.len) {
        l->cursor = l->text.len;
        l->current = '\0';
    } else {
        l->current = l->text.raw[l->cursor];    
    }
}

static forceinline bool cursor_eof(Lexer* l) {
    return l->cursor >= l->text.len;
}

static void skip_whitespace(Lexer* l) {
    while (!cursor_eof(l)) {
        switch (l->current) {
        case ' ':
        case '\t':
        case '\r':
        case '\v':
        // case '\n':
        case '\f':
            advance(l);
            break;
        default: 
            return;
        }
    }
}

static forceinline char peek(Lexer* l, i64 n) {
    if (l->cursor + n >= l->text.len) return '\0';
    return l->text.raw[l->cursor + n];
}

static forceinline bool can_begin_ident(char c) {
    return ('A' <= c && c <= 'Z') ||
           ('a' <= c && c <= 'z') ||
           c == '_';
}

static forceinline bool can_ident(char c) {
    return ('A' <= c && c <= 'Z') ||
           ('a' <= c && c <= 'z') ||
           ('0' <= c && c <= '9') ||
           c == '_';
}

static forceinline bool is_numeric(char c) {
    return ('0' <= c && c <= '9');
}

static forceinline bool is_numeric_or_(char c) {
    return ('0' <= c && c <= '9') || c == '_';
}

#define n_compare(lit, kind) if (!strncmp(s, lit, sizeof(lit)-1)) return kind

static u8 identify_keyword(char* s, size_t len) {
    switch (len) {
    case 1:
        if (s[0] == '_') return TOK_IDENTIFIER_DISCARD;
        break;
    case 2:
        n_compare("do", TOK_KEYWORD_DO);
        n_compare("fn", TOK_KEYWORD_FN);
        n_compare("if", TOK_KEYWORD_IF);
        n_compare("in", TOK_KEYWORD_IN);
        n_compare("i8", TOK_TYPE_KEYWORD_I8);
        n_compare("u8", TOK_TYPE_KEYWORD_U8);
        break;
    case 3:
        n_compare("let", TOK_KEYWORD_LET);
        n_compare("mut", TOK_KEYWORD_MUT);
        n_compare("def", TOK_KEYWORD_DEF);
        n_compare("asm", TOK_KEYWORD_ASM);
        n_compare("for", TOK_KEYWORD_FOR);
        n_compare("int", TOK_TYPE_KEYWORD_INT);
        n_compare("i16", TOK_TYPE_KEYWORD_I16);
        n_compare("i32", TOK_TYPE_KEYWORD_I32);
        n_compare("i64", TOK_TYPE_KEYWORD_I64);
        n_compare("u16", TOK_TYPE_KEYWORD_U16);
        n_compare("u32", TOK_TYPE_KEYWORD_U32);
        n_compare("u64", TOK_TYPE_KEYWORD_U64);
        n_compare("f16", TOK_TYPE_KEYWORD_F16);
        n_compare("f32", TOK_TYPE_KEYWORD_F32);
        n_compare("f64", TOK_TYPE_KEYWORD_F64);
        break;
    case 4:
        n_compare("type", TOK_KEYWORD_TYPE);
        n_compare("case", TOK_KEYWORD_CASE);
        n_compare("cast", TOK_KEYWORD_CAST);
        n_compare("enum", TOK_KEYWORD_ENUM);
        n_compare("else", TOK_KEYWORD_ELSE);
        n_compare("uint", TOK_TYPE_KEYWORD_UINT);
        n_compare("bool", TOK_TYPE_KEYWORD_BOOL);
        break;
    case 5:
        n_compare("break", TOK_KEYWORD_BREAK);
        n_compare("defer", TOK_KEYWORD_DEFER);
        n_compare("union", TOK_KEYWORD_UNION);
        n_compare("while", TOK_KEYWORD_WHILE);
        n_compare("float", TOK_TYPE_KEYWORD_FLOAT);
        break;
    case 6:
        n_compare("extern", TOK_KEYWORD_EXTERN);
        n_compare("import", TOK_KEYWORD_IMPORT);
        n_compare("return", TOK_KEYWORD_RETURN);
        n_compare("struct", TOK_KEYWORD_STRUCT);
        n_compare("switch", TOK_KEYWORD_SWITCH);
        n_compare("sizeof", TOK_KEYWORD_SIZEOF);
        n_compare("module", TOK_KEYWORD_MODULE);
        break;
    case 7:
        n_compare("bitcast", TOK_KEYWORD_BITCAST);
        n_compare("alignof", TOK_KEYWORD_ALIGNOF);
        break;
    case 8:
        n_compare("continue", TOK_KEYWORD_CONTINUE);
        n_compare("distinct", TOK_KEYWORD_DISTINCT);
        n_compare("offsetof", TOK_KEYWORD_OFFSETOF);
        break;
    case 11:
        n_compare("fallthrough", TOK_KEYWORD_FALLTHROUGH);
        break;
    }
    return TOK_IDENTIFIER;
}

#undef n_compare

#define push_simple_token(kind) do { add_token(l, kind); advance(l); goto next_token;} while (0)
#define push_token(kind, len) do { add_token(l, kind); advance_n(l, len); goto next_token;} while (0)

static void tokenize(Lexer* l) {
    while (!cursor_eof(l)) {
        skip_whitespace(l);
        if (cursor_eof(l)) {
            add_token(l, TOK_EOF);
            return;
        }

        // printf("%d '%c'\n", l->cursor, l->current);

        switch (l->current) {        
        case '\n': push_simple_token(TOK_NEWLINE);
        case '(':  push_simple_token(TOK_OPEN_PAREN);
        case ')':  push_simple_token(TOK_CLOSE_PAREN);
        case '[':  push_simple_token(TOK_OPEN_BRACKET);
        case ']':  push_simple_token(TOK_CLOSE_BRACKET);
        case '{':  push_simple_token(TOK_OPEN_BRACE);
        case '}':  push_simple_token(TOK_CLOSE_BRACE);
        case ';':  push_simple_token(TOK_SEMICOLON);
        case '#':  push_simple_token(TOK_HASH);
        case ',':  push_simple_token(TOK_COMMA);
        case '!':  push_simple_token(TOK_EXCLAM);
        case '?':  push_simple_token(TOK_QUESTION);
        case '^':  push_simple_token(TOK_CARET);
        case '.':
            if (is_numeric(peek(l, 1))) {
                TODO("numbers!");
            }
            if (peek(l, 1) == '.') {
                if (peek(l, 2) == '<') push_token(TOK_RANGE_LESS, 3);
                if (peek(l, 2) == '=') push_token(TOK_RANGE_EQ, 3);
            }
            push_simple_token(TOK_PERIOD);
        case '&':
            if (peek(l, 1) == '&') 
                push_token(TOK_AND_AND, 2);
            if (peek(l, 1) == '=') 
                push_token(TOK_AND_EQUAL, 2);
            push_simple_token(TOK_AND);
        case '|':
            if (peek(l, 1) == '|') 
                push_token(TOK_OR_OR, 2);
            if (peek(l, 1) == '=') 
                push_token(TOK_OR_EQUAL, 2);
            push_simple_token(TOK_OR);
        case ':':
            if (peek(l, 1) == ':') 
                push_token(TOK_COLON_COLON, 2);
            push_simple_token(TOK_COLON);
        case '+':
            if (peek(l, 1) == '=')
                push_token(TOK_ADD_EQUAL, 2);
            push_simple_token(TOK_ADD);
        case '*':
            if (peek(l, 1) == '=')
                push_token(TOK_MUL_EQUAL, 2);
            push_simple_token(TOK_MUL);
        case '~':
            if (peek(l, 1) == '=')
                push_token(TOK_XOR_EQUAL, 2);
            push_simple_token(TOK_TILDE);
        case '/':
            if (peek(l, 1) == '/') {
                advance_n(l, 2);
                while (l->current != '\n' && l->current != '\0') {
                    advance(l);
                }
                goto next_token;
            }
            if (peek(l, 1) == '*') {
                TODO("block comments");
            }
            if (peek(l, 1) == '=')
                push_token(TOK_DIV_EQUAL, 2);
            push_simple_token(TOK_DIV);
        case '%':
            if (peek(l, 1) == '%' && peek(l, 2) == '=')
                push_token(TOK_MOD_MOD_EQUAL, 3);
            if (peek(l, 1) == '%')
                push_token(TOK_MOD_MOD, 2);
            if (peek(l, 1) == '=')
                push_token(TOK_MOD_EQUAL, 2);
            push_simple_token(TOK_MOD);
        case '-':
            if (peek(l, 1) == '-' && peek(l, 2) == '-')
                push_token(TOK_UNINIT, 3);
            if (peek(l, 1) == '=')
                push_token(TOK_SUB_EQUAL, 2);
            if (peek(l, 1) == '>') 
                push_token(TOK_ARROW_RIGHT, 2);
            push_simple_token(TOK_SUB);
        case '<':
            if (peek(l, 1) == '-' && peek(l, 2) == '>')
                push_token(TOK_ARROW_BIDIR, 3);
            if (peek(l, 1) == '-')
                push_token(TOK_ARROW_LEFT, 2);
            if (peek(l, 1) == '<' && peek(l, 2) == '=')
                push_token(TOK_LSHIFT_EQUAL, 3);
            if (peek(l, 1) == '<')
                push_token(TOK_LSHIFT, 2);
            if (peek(l, 1) == '=')
                push_token(TOK_LESS_EQUAL, 2);
            push_simple_token(TOK_LESS_THAN);
        case '>':
            if (peek(l, 1) == '>' && peek(l, 2) == '=')
                push_token(TOK_RSHIFT_EQUAL, 3);
            if (peek(l, 1) == '>')
                push_token(TOK_RSHIFT, 2);
            if (peek(l, 1) == '=')
                push_token(TOK_GREATER_EQUAL, 2);
            push_simple_token(TOK_GREATER_THAN);
        case '=':
            if (peek(l, 1) == '=')
                push_token(TOK_EQUAL_EQUAL, 2);
            push_simple_token(TOK_EQUAL);
        default:
        }

        if (can_begin_ident(l->current)) {
            u64 len = 1;
            while (can_ident(peek(l, len))) {
                len++;
            }
            u8 kind = identify_keyword(&l->text.raw[l->cursor], len);
            add_token(l, kind);
            advance_n(l, len);
            goto next_token;
        }

        printf("unrecognized '%c' %d\n", l->current, l->cursor);
        CRASH("unrecognized");
        return;
        

        next_token:
    }

    
}