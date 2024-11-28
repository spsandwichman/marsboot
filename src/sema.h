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
    TYPE_UNTYPED_INT,
    TYPE_F16,
    TYPE_F32,
    TYPE_F64,
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
};

typedef struct TypeNodeBase {
    u8 kind;
    u32 number;
} TypeNodeBase;

typedef struct TypeRecordField {
    string name;
    Type type;
    usize offset;
} TypeRecordField;

typedef struct TypeEnumVariant {

} TypeEnumVariant;

typedef struct TypeNode {
    TypeNodeBase x;
    union {
        Type distinct;
        struct {
            bool mutable;
            Type subtype;
        } as_ref;
        struct {
            Type sub;
            usize len;
        } as_array;
        struct {
            TypeRecordField* at;
            usize len;
            usize cap;

            usize size;
            usize align;
        } as_record;
        struct {
            Type underlying;
        } as_enum;
        
    };
} TypeNode;

typedef struct TypeGraph {
    struct {
        TypeNode** at;
        u32 len;
        u32 cap;
    } handle2node;
} TypeGraph;