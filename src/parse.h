#pragma once

#include "mars.h"

typedef struct Token {
    char* raw;
    u32 len;
    u8 kind;
} Token;

da_typedef(Token);
typedef da(Token) TokenBuf;


#define TOKEN_LIST \
    TOKEN(TOK_INVALID, "INVALID") \
    TOKEN(TOK_EOF,     "EOF") \
\
    TOKEN(TOK_IDENTIFIER, "identifier") \
    TOKEN(TOK_IDENTIFIER_DISCARD, "_") \
\
    TOKEN(TOK_LITERAL_INT, "integer") \
    TOKEN(TOK_LITERAL_STRING, "string") \
    TOKEN(TOK_LITERAL_CHAR, "character") \
\
    TOKEN(TOK_HASH,      "#") \
    TOKEN(TOK_UNINIT,    "---") \
    TOKEN(TOK_EQUAL,     "=") \
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
\
    TOKEN(TOK_KEYWORD_ASM,       "asm") \
    TOKEN(TOK_KEYWORD_BITCAST,   "bitcast") \
    TOKEN(TOK_KEYWORD_BREAK,     "break") \
    TOKEN(TOK_KEYWORD_CASE,      "case") \
    TOKEN(TOK_KEYWORD_CAST,      "cast") \
    TOKEN(TOK_KEYWORD_CONTINUE,  "continue") \
    TOKEN(TOK_KEYWORD_DEFER,     "defer") \
    TOKEN(TOK_KEYWORD_DEF,       "def") \
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
    TOKEN(TOK_KEYWORD_WHEN,      "when") \
    TOKEN(TOK_KEYWORD_WHICH,     "which") \
    TOKEN(TOK_KEYWORD_IMPORT,    "import") \
    TOKEN(TOK_KEYWORD_MODULE,    "module") \
    TOKEN(TOK_KEYWORD_RETURN,    "return") \
    TOKEN(TOK_KEYWORD_STRUCT,    "struct") \
    TOKEN(TOK_KEYWORD_SWITCH,    "switch") \
    TOKEN(TOK_KEYWORD_UNION,     "union") \
    TOKEN(TOK_KEYWORD_WHILE,     "while") \
\
    TOKEN(TOK_KEYWORD_TYPEOF,    "typeof") \
    TOKEN(TOK_KEYWORD_SIZEOF,    "sizeof") \
    TOKEN(TOK_KEYWORD_ALIGNOF,   "alignof") \
    TOKEN(TOK_KEYWORD_OFFSETOF,  "offsetof") \
\
    TOKEN(TOK_KEYWORD_INT,  "int") \
    TOKEN(TOK_KEYWORD_I8,   "i8") \
    TOKEN(TOK_KEYWORD_I16,  "i16") \
    TOKEN(TOK_KEYWORD_I32,  "i32") \
    TOKEN(TOK_KEYWORD_I64,  "i64") \
\
    TOKEN(TOK_KEYWORD_UINT, "uint") \
    TOKEN(TOK_KEYWORD_U8,   "u8") \
    TOKEN(TOK_KEYWORD_U16,  "u16") \
    TOKEN(TOK_KEYWORD_U32,  "u32") \
    TOKEN(TOK_KEYWORD_U64,  "u64") \
\
    TOKEN(TOK_KEYWORD_BOOL, "bool") \
    TOKEN(TOK_KEYWORD_DYN,  "dyn") \
    TOKEN(TOK_KEYWORD_TYPEID,  "typeid") \
\
    TOKEN(TOK_KEYWORD_FLOAT,"float") \
    TOKEN(TOK_KEYWORD_F16,  "f16") \
    TOKEN(TOK_KEYWORD_F32,  "f32") \
    TOKEN(TOK_KEYWORD_F64,  "f64") \
\
    TOKEN(TOK_KEYWORD_NULL,  "null") \
    TOKEN(TOK_KEYWORD_TRUE,  "true") \
    TOKEN(TOK_KEYWORD_FALSE,  "false") \
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

TokenBuf lex_string(string text);

enum {
    PN_INVALID,

    PN_STMT_MODULE_DECL,
    PN_STMT_IMPORT_DECL,
    PN_STMT_DECL,
    PN_STMT_FN_DECL,
    PN_STMT_EXTERN_DECL,

    PN_STMT_EMPTY,

    PN_STMT_ASSIGN,
    PN_STMT_ASSIGN_ADD,
    PN_STMT_ASSIGN_SUB,
    PN_STMT_ASSIGN_MUL,
    PN_STMT_ASSIGN_DIV,
    PN_STMT_ASSIGN_MOD,
    PN_STMT_ASSIGN_REM,
    PN_STMT_ASSIGN_BIT_AND,
    PN_STMT_ASSIGN_BIT_OR,
    PN_STMT_ASSIGN_NOR,
    PN_STMT_ASSIGN_XOR,
    PN_STMT_ASSIGN_LSHIFT,
    PN_STMT_ASSIGN_RSHIFT,

    PN_STMT_FOR_CSTYLE,
    PN_STMT_FOR_RANGED,
    PN_STMT_WHILE,
    PN_STMT_IF,
    PN_STMT_WHEN,
    PN_STMT_RETURN,
    PN_STMT_SWITCH,
    PN_STMT_WHICH,
    PN_STMT_DEFER,
    PN_STMT_FALLTHROUGH,
    PN_STMT_BREAK,
    PN_STMT_CONTINUE,
    PN_STMT_LABEL,

    PN_IDENT,
    PN_DISCARD,
    PN_EXPR_STRING,
    PN_EXPR_CHAR,
    PN_EXPR_INTEGER,
    PN_EXPR_COMPOUND,

    PN_EXPR_TRUE,
    PN_EXPR_FALSE,
    PN_EXPR_NULL,

    // binop
    PN_EXPR_ADD,
    PN_EXPR_SUB,
    PN_EXPR_MUL,
    PN_EXPR_DIV,
    PN_EXPR_MOD,
    PN_EXPR_REM,
    PN_EXPR_BIT_AND,
    PN_EXPR_BIT_OR,
    PN_EXPR_NOR,
    PN_EXPR_XOR,
    PN_EXPR_LSHIFT,
    PN_EXPR_RSHIFT,
    PN_EXPR_BOOL_AND,
    PN_EXPR_BOOL_OR,
    PN_EXPR_EQ,
    PN_EXPR_NEQ,
    PN_EXPR_LESS,
    PN_EXPR_LESS_EQ,
    PN_EXPR_GREATER,
    PN_EXPR_GREATER_EQ,
    PN_EXPR_RANGE_LESS,
    PN_EXPR_RANGE_EQ,
    PN_EXPR_IN,

    // unop
    PN_EXPR_ADDR,
    PN_EXPR_BOOL_NOT,
    PN_EXPR_BOOL_COERCE,
    PN_EXPR_BIT_NOT,
    PN_EXPR_NEG,
    PN_EXPR_POS,
    PN_EXPR_SIZEOF,
    PN_EXPR_ALIGNOF,
    PN_EXPR_OFFSETOF,
    PN_EXPR_DEREF,

    PN_EXPR_IMPLICIT_SELECTOR,

    PN_EXPR_NAMESPACE, // binop
    PN_EXPR_SELECTOR, // binop
    PN_EXPR_INDEX, // binop
    PN_EXPR_CAST, // binop
    PN_EXPR_BITCAST, // binop
    PN_EXPR_SLICE,
    PN_EXPR_CALL,

    PN_EXPR_IF,
    PN_EXPR_WHEN,
    PN_EXPR_SWITCH,
    PN_EXPR_WHICH,

    PN_EXPR_ANON_FN,

    PN_TYPE_TYPEOF,
    PN_TYPE_POINTER,
    PN_TYPE_SLICE,
    PN_TYPE_ARRAY,
    PN_TYPE_DISTINCT,

    PN_TYPE_STRUCT,
    PN_TYPE_UNION,
    PN_TYPE_ENUM,

    PN_TYPE_I8,
    PN_TYPE_I16,
    PN_TYPE_I32,
    PN_TYPE_I64,
    PN_TYPE_INT,
    PN_TYPE_U8,
    PN_TYPE_U16,
    PN_TYPE_U32,
    PN_TYPE_U64,
    PN_TYPE_UINT,
    PN_TYPE_F16,
    PN_TYPE_F32,
    PN_TYPE_F64,
    PN_TYPE_FLOAT,
    PN_TYPE_BOOL,
    PN_TYPE_DYN,
    PN_TYPE_TYPEID,

    PN_ITEM, // for param/return lists. many identifiers, one type
    PN_FNPROTO,
    PN_LIST,
    PN_DO,
    PN_CASE_BLOCK,

    // binop
    PN_COMP_ITEM_INDEX, // [expr] = expr
    PN_COMP_ITEM_SELECTOR, // .ident = expr
    PN_ENUM_VARIANT_VALUED, // ident = expr

};

typedef struct PNodeBase {
    char* raw; // source span for error reporting
    u32 len;
    u8 kind;
} PNodeBase;

enum {
    DECLKIND_LET,
    DECLKIND_MUT,
    DECLKIND_DEF,
};

typedef struct PNode PNode;
typedef struct PNode {
    PNodeBase base;

    union {
        struct {} _;
        struct {
            PNode* ident;
        } module_decl;
        struct {
            PNode* ident;
            PNode* path;
        } import_decl;
        struct {
            PNode** at;
            u32 len;
        } list;
        struct {
            PNode* ident; // could be IDENT or IDENT_LIST
            PNode* type;
            PNode* value;
            u8 kind;
        } decl;
        struct {
            PNode* ident;
            PNode* stmt;
        } label;
        struct {
            PNode* label;
        } cflow;
        struct {
            PNode* first;
            PNode* second;
        } fallthrough;
        struct {
            PNode* sub;
        } unop;
        struct {
            PNode* lhs;
            PNode* rhs;
        } binop;
        struct {
            PNode* len;
            PNode* sub;
        } array_type;
        struct {
            PNode* sub;
            bool mutable;
        } ref_type;
        struct {
            PNode* sub;
            PNode* left_bound;
            PNode* right_bound;
        } slice;
        struct {
            PNode* type;
            PNode* expr_list;
        } compound;
        struct {
            PNode* ident;
            PNode* param_list;
            PNode* returns; // may be a single type, may be a item_set list
        } fnproto;
        struct {
            PNode* ident; // may be list
            PNode* type;
        } item;
        struct {
            PNode* proto;
            PNode* stmts;
        } fn_decl;
        struct {
            PNode* expr;
        } expr_stmt;
        struct {
            PNode* cond;
            PNode* if_true;
            PNode* if_false;
        } ternary;
        struct {
            PNode* cond;
            PNode* cases;
        } switch_stmt;
        struct {
            PNode* matches; // list of exprs
            PNode* sub;
        } switch_case;
        struct {
            PNode* callee;
            PNode* param_list;
        } call;
        struct {
            PNode* init;
            PNode* cond;
            PNode* post;
            PNode* block;
        } for_cstyle;
        struct {
            PNode* decl;
            PNode* range;
            PNode* block;
        } for_ranged;
        struct {
            PNode* fields;
        } record_type;
        struct {
            PNode* type;
            PNode* variants;
        } enum_type;
    };
} PNode;

PNode* parse_file(TokenBuf tb, string expected_module_name);
PNode* parse_ident();
PNode* parse_string();
PNode* parse_stmt();

PNode* parse_expr();