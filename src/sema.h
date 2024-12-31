#pragma once

#include "parse.h"

typedef u32 Type; // handle

enum TypeKind {
    TYPE_NONE,
    TYPE_BOOL,
    TYPE_DYN,
    TYPE_TYPEID,
    TYPE_I8,
    TYPE_U8,
    TYPE_I16,
    TYPE_U16,
    TYPE_I32,
    TYPE_U32,
    TYPE_I64,
    TYPE_U64,
    TYPE_F16,
    TYPE_F32,
    TYPE_F64,
    TYPE_UNTYPED_INT,
    TYPE_UNTYPED_FLOAT,
    TYPE_UNTYPED_STRING,

    _TYPE_SIMPLE_END,

    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_ARRAY,
    TYPE_ARRAY_LEN_UNKNOWN,
    TYPE_ENUM,
    TYPE_POINTER,
    TYPE_HEADLESS_SLICE,
    TYPE_SLICE,
    TYPE_DISTINCT,

    TYPE_TUPLE,
};

typedef struct TypeRecordField {
    string name;
    Type type;
    usize offset;
} TypeRecordField;

typedef struct TypeEnumVariant {
    string name;
    usize value;
} TypeEnumVariant;

typedef struct TNode {
    u8 kind;
    bool visited; // for shit
    u32 num_a;
    u32 num_b;

    union {
        Type as_distinct;
        struct {
            Type pointee;
            bool mutable;
        } as_ref;
        struct {
            Type sub;
            usize len;
        } as_array;
        struct {
            TypeRecordField* at;
            usize len;
            usize size;
            usize align;
        } as_record;
        struct {
            TypeEnumVariant* at;
            usize len;
            Type underlying;
        } as_enum;
    };
} TNode;

typedef struct TypeGraph {
    struct {
        string* names; // attach a type handle to a name for better error printing
        Type* equiv;
        TNode** at;
        u32 len;
        u32 cap;
    } handles;
} TypeGraph;

void type_init();
Type type_new(u8 kind);
TNode* type(Type t);
void type_condense();

// ---------------------------------------------------------

typedef struct SemaStmt SemaStmt;
typedef struct Entity Entity;
typedef struct EntityTable EntityTable;

// ---------------------------------------------------------

typedef struct ConstVal {
    Type type;
    union {
        u8  u8;
        u16 u16;
        u32 u32;
        u64 u64;
        i8  i8;
        i16 i16;
        i32 i32;
        i64 i64;
        f16 f16;
        f32 f32;
        f64 f64;
    };
} ConstVal;

enum EntityStorageKind {
    STORAGE_EXTERN,
    STORAGE_GLOBAL,
    STORAGE_LOCAL,
    STORAGE_DEF,
};

typedef struct Entity {
    Type type;
    u8 storage;
    bool mutable;
    string name;
    EntityTable* tbl;
    SemaStmt* decl;
    ConstVal constval;
} Entity;

typedef struct EntityTable {
    Entity** at;
    u32 len;
    u32 cap;

    EntityTable* parent;
} EntityTable;

// ---------------------------------------------------------

typedef struct Module {
    string name;
    EntityTable* global;
} Module;

enum SemaExprKind {
    SEXPR_INVALID = 0,

    SEXPR_CONSTVAL,
    SEXPR_ENTITY,
};

typedef struct SemaExpr {
    u8 kind;
    bool mutable;
    Type type;
    PNode* pnode;
    union {
        ConstVal constval;
    };
} SemaExpr;

enum SemaStmtKind {
    SSTMT_INVALID = 0,

    SSTMT_DECL,
};

typedef struct SemaStmt {
    u8 kind;
    PNode* pnode;

    union {
        struct {
            Entity* entity;
            SemaExpr* value;
        } decl;
    };
} SemaStmt;