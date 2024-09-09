#pragma once

#include "mars.h"

// typedef struct Token {
//     u16 file_index;
//     u16 len : 9;
//     u16 kind : 7;
//     u32 src_offset;
// } Token;
typedef struct Token {
    u16 file_index;
    u8  kind;
    u32 src_offset;
} Token;
static_assert(sizeof(Token) == 8);

da_typedef(Token);
typedef da(Token) TokenBuf;


#define TOKEN_LIST \
    TOKEN(TOK_INVALID, "INVALID") \
    TOKEN(TOK_EOF,     "EOF") \
\
    TOKEN(TOK_IDENTIFIER, "identifier") \
    TOKEN(TOK_IDENTIFIER_DISCARD, "_") \
\
    TOKEN(TOK_LITERAL_INT, "integer literal") \
    TOKEN(TOK_LITERAL_FLOAT, "float literal") \
    TOKEN(TOK_LITERAL_BOOL, "bool literal") \
    TOKEN(TOK_LITERAL_STRING, "string literal") \
    TOKEN(TOK_LITERAL_CHAR, "char literal") \
    TOKEN(TOK_LITERAL_NULL, "null") \
\
    TOKEN(TOK_NEWLINE,      "\\n") \
\
    TOKEN(TOK_HASH,      "#") \
    TOKEN(TOK_UNINIT,    "---") \
    TOKEN(TOK_EQUAL,     "=") \
    /*TOKEN(TOK_DOLLAR,    "$") \ */ \
    TOKEN(TOK_COLON,     ":") \
    TOKEN(TOK_COLON_COLON, "::") \
    TOKEN(TOK_SEMICOLON, ";") \
    TOKEN(TOK_PERIOD,    ".") \
    TOKEN(TOK_RANGE_LESS,"..<") \
    TOKEN(TOK_RANGE_EQ,  "..=") \
    TOKEN(TOK_COMMA,     ",") \
    TOKEN(TOK_EXCLAM,    "!") \
    TOKEN(TOK_QUESTION,  "?") \
    TOKEN(TOK_CARET,     "^") \
    /* TOKEN(TOK_AT,        "@") */ \
    TOKEN(TOK_ADD,       "+") \
    TOKEN(TOK_SUB,       "-") \
    TOKEN(TOK_MUL,       "*") \
    TOKEN(TOK_DIV,       "/") \
    TOKEN(TOK_MOD,       "%") \
    TOKEN(TOK_MOD_MOD,   "%%") \
    TOKEN(TOK_TILDE,     "~") \
    TOKEN(TOK_AND,       "&") \
    TOKEN(TOK_OR,        "|") \
    TOKEN(TOK_NOR,       "~|") \
    TOKEN(TOK_LSHIFT,    "<<") \
    TOKEN(TOK_RSHIFT,    ">>") \
\
    TOKEN(TOK_AND_AND,       "&&") \
    TOKEN(TOK_OR_OR,         "||") \
\
    TOKEN(TOK_ARROW_RIGHT,   "->") \
    TOKEN(TOK_ARROW_LEFT,    "<-") \
    TOKEN(TOK_ARROW_BIDIR,   "<->") \
\
    TOKEN(TOK_ADD_EQUAL,     "+=") \
    TOKEN(TOK_SUB_EQUAL,     "-=") \
    TOKEN(TOK_MUL_EQUAL,     "*=") \
    TOKEN(TOK_DIV_EQUAL,     "/=") \
    TOKEN(TOK_MOD_EQUAL,     "%=") \
    TOKEN(TOK_MOD_MOD_EQUAL, "%%=") \
\
    TOKEN(TOK_AND_EQUAL,     "&=") \
    TOKEN(TOK_OR_EQUAL,      "|=") \
    TOKEN(TOK_NOR_EQUAL,     "~|=") \
    TOKEN(TOK_XOR_EQUAL,     "~=") \
    TOKEN(TOK_LSHIFT_EQUAL,  "<<=") \
    TOKEN(TOK_RSHIFT_EQUAL,  ">>=") \
\
    TOKEN(TOK_EQUAL_EQUAL,   "==") \
    TOKEN(TOK_NOT_EQUAL,     "!=") \
    TOKEN(TOK_LESS_THAN,     "<") \
    TOKEN(TOK_LESS_EQUAL,    "<=") \
    TOKEN(TOK_GREATER_THAN,  ">") \
    TOKEN(TOK_GREATER_EQUAL, ">=") \
\
    TOKEN(TOK_OPEN_PAREN,    "(") \
    TOKEN(TOK_CLOSE_PAREN,   ")") \
    TOKEN(TOK_OPEN_BRACE,    "{") \
    TOKEN(TOK_CLOSE_BRACE,   "}") \
    TOKEN(TOK_OPEN_BRACKET,  "[") \
    TOKEN(TOK_CLOSE_BRACKET, "]") \
\
    TOKEN(_TOK_BEGIN_KEYWORDS, "")\
\
    TOKEN(TOK_KEYWORD_LET,   "let") \
    TOKEN(TOK_KEYWORD_MUT,   "mut") \
    TOKEN(TOK_KEYWORD_DEF,   "def") \
    TOKEN(TOK_KEYWORD_TYPE,  "type") \
\
    TOKEN(TOK_KEYWORD_ASM,       "asm") \
    TOKEN(TOK_KEYWORD_BITCAST,   "bitcast") \
    TOKEN(TOK_KEYWORD_BREAK,     "break") \
    TOKEN(TOK_KEYWORD_CASE,      "case") \
    TOKEN(TOK_KEYWORD_CAST,      "cast") \
    TOKEN(TOK_KEYWORD_CONTINUE,  "continue") \
    TOKEN(TOK_KEYWORD_DEFER,     "defer") \
    TOKEN(TOK_KEYWORD_DISTINCT,  "distinct") \
    TOKEN(TOK_KEYWORD_DO,        "do") \
    TOKEN(TOK_KEYWORD_ENUM,      "enum") \
    TOKEN(TOK_KEYWORD_ELSE,      "else") \
    TOKEN(TOK_KEYWORD_EXTERN,    "extern") \
    TOKEN(TOK_KEYWORD_FALLTHROUGH,"fallthrough") \
    TOKEN(TOK_KEYWORD_FOR,       "for") \
    TOKEN(TOK_KEYWORD_FN,        "fn") \
    TOKEN(TOK_KEYWORD_IF,        "if") \
    TOKEN(TOK_KEYWORD_IN,        "in") \
    TOKEN(TOK_KEYWORD_IMPORT,    "import") \
    TOKEN(TOK_KEYWORD_MODULE,    "module") \
    TOKEN(TOK_KEYWORD_RETURN,    "return") \
    TOKEN(TOK_KEYWORD_STRUCT,    "struct") \
    TOKEN(TOK_KEYWORD_SWITCH,    "switch") \
    TOKEN(TOK_KEYWORD_UNION,     "union") \
    TOKEN(TOK_KEYWORD_WHILE,     "while") \
\
    TOKEN(TOK_KEYWORD_SIZEOF,    "sizeof") \
    TOKEN(TOK_KEYWORD_ALIGNOF,   "alignof") \
    TOKEN(TOK_KEYWORD_OFFSETOF,  "offsetof") \
\
    TOKEN(TOK_TYPE_KEYWORD_INT,  "int") \
    TOKEN(TOK_TYPE_KEYWORD_I8,   "i8") \
    TOKEN(TOK_TYPE_KEYWORD_I16,  "i16") \
    TOKEN(TOK_TYPE_KEYWORD_I32,  "i32") \
    TOKEN(TOK_TYPE_KEYWORD_I64,  "i64") \
\
    TOKEN(TOK_TYPE_KEYWORD_UINT, "uint") \
    TOKEN(TOK_TYPE_KEYWORD_U8,   "u8") \
    TOKEN(TOK_TYPE_KEYWORD_U16,  "u16") \
    TOKEN(TOK_TYPE_KEYWORD_U32,  "u32") \
    TOKEN(TOK_TYPE_KEYWORD_U64,  "u64") \
\
    TOKEN(TOK_TYPE_KEYWORD_BOOL, "bool") \
\
    TOKEN(TOK_TYPE_KEYWORD_FLOAT,"float") \
    TOKEN(TOK_TYPE_KEYWORD_F16,  "f16") \
    TOKEN(TOK_TYPE_KEYWORD_F32,  "f32") \
    TOKEN(TOK_TYPE_KEYWORD_F64,  "f64") \
\
    TOKEN(_TOK_END_KEYWORDS, "")\
\
    TOKEN(_TOK_COUNT, "") \

enum {
#define TOKEN(enum, str) enum,
    TOKEN_LIST
#undef TOKEN
};

extern const char* token_kind_name[];

TokenBuf lex_string(string text, u16 file_index);

// #define NULL_PN (ParseNodeIndex)0
// typedef u32 ParseNodeIndex;
// typedef u32 TokenIndex;

// typedef struct ParseNode {
//     TokenIndex start_token;

//     union {
//         struct {
//             ParseNodeIndex lhs;
//             ParseNodeIndex rhs;
//         };

//         struct {
//             ParseNodeIndex sub;
//             ParseNodeIndex extra;
//         };
//     } data;
// } ParseNode;

// typedef struct ParseTree {

//     TokenBuf tokens;

//     ParseNode* nodes;
//     u8* node_types;
//     size_t cap;
//     size_t len;

//     struct {
//         ParseNodeIndex* at;
//         size_t len;
//         size_t cap;
//     } extra;

// } ParseTree;