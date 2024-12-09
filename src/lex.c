#include "parse.h"

typedef struct Lexer {
    TokenBuf* tb;
    string text;
    u32 cursor;
    char current;
} Lexer;

const char* token_kind_name[] = {
#define TOKEN(enum, str) [enum] = str,
    TOKEN_LIST
#undef TOKEN
};

static void tokenize(Lexer* l);

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
    l.current = text.raw[0];

    tokenize(&l);

    return tokenbuf;
}

static void add_token(Lexer* l, u8 kind, usize len) {
    Token t;
    t.raw = &l->text.raw[l->cursor];
    t.len = len;
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
        case '\n':
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

static bool is_base_digit(char c, u64 base) {
    if (c == '_') return true;

    if (base <= 10) {
        return '0' <= c && c <= ('0' + base - 1);
    } else {
        return 
            ('0' <= c && c <= '9') || 
            ('a' <= c && c <= ('a' + base - 10)) ||
            ('A' <= c && c <= ('A' + base - 10));
    }
}

static u64 scan_integer_base(Lexer* l, u64 base, u64 start_at) {
    u64 len = start_at;
    while (is_base_digit(peek(l, len), base)) {
        len++;
    }
    if (can_ident(peek(l, len))) {
        CRASH("invalid digit");
    }
    return len;
}

static u64 scan_numeric(Lexer* l) {
    if (l->current == '0') {
        switch (peek(l, 1)){
        case 'x':
        case 'X': return scan_integer_base(l, 16, 2);
        case 'd':
        case 'D': return scan_integer_base(l, 10, 2);
        case 'o':
        case 'O': return scan_integer_base(l, 8, 2);
        case 'b':
        case 'B': return scan_integer_base(l, 2, 2);
        default:
            if (is_numeric_or_(peek(l, 1))) {
                return scan_integer_base(l, 10, 2);
            }
            return 1;
            break;
        }
    }
    if (is_numeric(l->current)) {
        return scan_integer_base(l, 10, 1);
    }
    CRASH("what?");
}

#define return_if_eq(lit, kind) if (!strncmp(s, lit, sizeof(lit)-1)) return kind

static u8 identify_keyword(char* s, size_t len) {
    if (len == 1 && s[0] == '_') return TOK_IDENTIFIER_DISCARD;

    switch (len) {
    case 1:
        if (s[0] == '_') return TOK_IDENTIFIER_DISCARD;
        break;
    case 2:
        return_if_eq("do", TOK_KEYWORD_DO);
        return_if_eq("fn", TOK_KEYWORD_FN);
        return_if_eq("if", TOK_KEYWORD_IF);
        return_if_eq("in", TOK_KEYWORD_IN);
        return_if_eq("i8", TOK_KEYWORD_I8);
        return_if_eq("u8", TOK_KEYWORD_U8);
        break;
    case 3:
        return_if_eq("let", TOK_KEYWORD_LET);
        return_if_eq("mut", TOK_KEYWORD_MUT);
        return_if_eq("asm", TOK_KEYWORD_ASM);
        return_if_eq("def", TOK_KEYWORD_DEF);
        return_if_eq("dyn", TOK_KEYWORD_DYN);
        return_if_eq("for", TOK_KEYWORD_FOR);
        return_if_eq("int", TOK_KEYWORD_INT);
        return_if_eq("i16", TOK_KEYWORD_I16);
        return_if_eq("i32", TOK_KEYWORD_I32);
        return_if_eq("i64", TOK_KEYWORD_I64);
        return_if_eq("u16", TOK_KEYWORD_U16);
        return_if_eq("u32", TOK_KEYWORD_U32);
        return_if_eq("u64", TOK_KEYWORD_U64);
        return_if_eq("f16", TOK_KEYWORD_F16);
        return_if_eq("f32", TOK_KEYWORD_F32);
        return_if_eq("f64", TOK_KEYWORD_F64);
        break;
    case 4:
        return_if_eq("when", TOK_KEYWORD_WHEN);
        return_if_eq("true", TOK_KEYWORD_TRUE);
        return_if_eq("null", TOK_KEYWORD_NULL);
        return_if_eq("case", TOK_KEYWORD_CASE);
        return_if_eq("cast", TOK_KEYWORD_CAST);
        return_if_eq("enum", TOK_KEYWORD_ENUM);
        return_if_eq("else", TOK_KEYWORD_ELSE);
        return_if_eq("uint", TOK_KEYWORD_UINT);
        return_if_eq("bool", TOK_KEYWORD_BOOL);
        break;
    case 5:
        return_if_eq("false", TOK_KEYWORD_FALSE);
        return_if_eq("break", TOK_KEYWORD_BREAK);
        return_if_eq("defer", TOK_KEYWORD_DEFER);
        return_if_eq("union", TOK_KEYWORD_UNION);
        return_if_eq("while", TOK_KEYWORD_WHILE);
        return_if_eq("which", TOK_KEYWORD_WHICH);
        return_if_eq("float", TOK_KEYWORD_FLOAT);
        break;
    case 6:
        return_if_eq("extern", TOK_KEYWORD_EXTERN);
        return_if_eq("import", TOK_KEYWORD_IMPORT);
        return_if_eq("typeid", TOK_KEYWORD_TYPEID);
        return_if_eq("return", TOK_KEYWORD_RETURN);
        return_if_eq("struct", TOK_KEYWORD_STRUCT);
        return_if_eq("switch", TOK_KEYWORD_SWITCH);
        return_if_eq("sizeof", TOK_KEYWORD_SIZEOF);
        return_if_eq("module", TOK_KEYWORD_MODULE);
        break;
    case 7:
        return_if_eq("bitcast", TOK_KEYWORD_BITCAST);
        return_if_eq("alignof", TOK_KEYWORD_ALIGNOF);
        break;
    case 8:
        return_if_eq("continue", TOK_KEYWORD_CONTINUE);
        return_if_eq("distinct", TOK_KEYWORD_DISTINCT);
        return_if_eq("offsetof", TOK_KEYWORD_OFFSETOF);
        break;
    case 11:
        return_if_eq("fallthrough", TOK_KEYWORD_FALLTHROUGH);
        break;
    }
    return TOK_IDENTIFIER;
}

#undef n_compare

#define push_simple_token(kind) do { add_token(l, kind, 1); advance(l); goto next_token;} while (0)
#define push_token(kind, len) do { add_token(l, kind, len); advance_n(l, len); goto next_token;} while (0)

static void tokenize(Lexer* l) {
    while (!cursor_eof(l)) {
        skip_whitespace(l);
        if (cursor_eof(l)) {
            add_token(l, TOK_EOF, 1);
            return;
        }

        // printf("%d '%c'\n", l->cursor, l->current);

        switch (l->current) {        
        // case '\n': push_simple_token(TOK_NEWLINE);
        case '(':  push_simple_token(TOK_OPEN_PAREN);
        case ')':  push_simple_token(TOK_CLOSE_PAREN);
        case '[':  push_simple_token(TOK_OPEN_BRACKET);
        case ']':  push_simple_token(TOK_CLOSE_BRACKET);
        case '{':  push_simple_token(TOK_OPEN_BRACE);
        case '}':  push_simple_token(TOK_CLOSE_BRACE);
        case ';':  push_simple_token(TOK_SEMICOLON);
        case '#':  push_simple_token(TOK_HASH);
        case ',':  push_simple_token(TOK_COMMA);
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
        case '!':
            if (peek(l, 1) == '=')
                push_token(TOK_NOT_EQUAL, 2);
            push_simple_token(TOK_EXCLAM);
        case '+':
            if (peek(l, 1) == '=')
                push_token(TOK_ADD_EQUAL, 2);
            push_simple_token(TOK_ADD);
        case '*':
            if (peek(l, 1) == '=')
                push_token(TOK_MUL_EQUAL, 2);
            push_simple_token(TOK_MUL);
        case '~':
            if (peek(l, 1) == '|' && peek(l, 2) == '=')
                push_token(TOK_NOR_EQUAL, 3);
            if (peek(l, 1) == '|')
                push_token(TOK_NOR, 2);
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
                advance_n(l, 2);
                int level = 1;
                while (level != 0 && l->current != '\0') {
                    if (l->current == '/' && peek(l, 1) == '*') {
                        level += 1;
                        advance_n(l, 2);
                        continue;
                    } else if (l->current == '*' && peek(l, 1) == '/') {
                        level -= 1;
                        advance_n(l, 2);
                        continue;
                    }
                    advance(l);
                }
                goto next_token;
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
        case '\'':
            u64 len = 1;
            while (!(peek(l, len) == '\'' && peek(l, len - 1) != '\\')) {
                if (peek(l, len) == '\0') {
                    CRASH("unterminated char literal");
                }
                len++;
            }
            // validity will be checked at sema
            add_token(l, TOK_LITERAL_CHAR, len + 1);
            advance_n(l, len + 1);
            goto next_token;
        case '\"':
            len = 1;
            while (!(peek(l, len) == '\"' && peek(l, len - 1) != '\\')) {
                if (peek(l, len) == '\0') {
                    CRASH("unterminated string literal");
                }
                len++;
            }
            // validity will be checked at sema-time
            add_token(l, TOK_LITERAL_STRING, len + 1);
            advance_n(l, len + 1);
            goto next_token;
        }

        if (can_begin_ident(l->current)) {
            u64 len = 1;
            while (can_ident(peek(l, len))) {
                len++;
            }
            u8 kind = identify_keyword(&l->text.raw[l->cursor], len);
            add_token(l, kind, len);
            advance_n(l, len);
            goto next_token;
        }

        if (is_numeric(l->current)) {
            u64 len = scan_numeric(l);
            add_token(l, TOK_LITERAL_INT, len);
            advance_n(l, len);
            goto next_token;
        }

        printf("unrecognized '%c' %d\n", l->current, l->cursor);
        CRASH("unrecognized");
        return;
        

        next_token:
        if (cursor_eof(l)) {
            add_token(l, TOK_EOF, 1);
            return;
        }
    }

    
}