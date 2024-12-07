#pragma once

#include "parse.h"

typedef u32 Type; // handle

enum {
    TYPE_NONE,
    TYPE_BOOL,
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
    TYPE_ENUM,
    TYPE_POINTER,
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
        Type* equiv;
        TNode** at;
        u32 len;
        u32 cap;
    } handles;
} TypeGraph;

void type_init();
Type type_new(u8 kind);
TNode* type(Type t);


typedef struct SemaExpr {

} SemaExpr;