#ifndef SEMA_H
#define SEMA_H

#include "parse.h"

typedef struct Module Module;

typedef u32 Type; // handle

enum TypeKind {
    TYPE_UNKNOWN,

    TYPE_VOID,
    TYPE_NEVER, // currently unused
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
    TYPE_BOUNDLESS_SLICE,
    TYPE_SLICE,
    TYPE_DISTINCT,

    TYPE_TUPLE, // apparently not implemented yet?? lmao
};

typedef struct TypeRecordField {
    string name;
    Type type;
    usize offset;
} TypeRecordField;

typedef struct TypeEnumVariant {
    string name;
    isize value;
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
        Module** mods; // every type has a module attached to it, where it was declared
        TNode** at;
        Type* equiv;
        u32 len;
        u32 cap;
    } handles;
} TypeGraph;
extern TypeGraph tg;

void type_init();
Type type_new(Module* m, u8 kind);
Type type_new_record(Module* m, u8 kind, usize len);
Type type_new_ref(Module* m, u8 kind, Type pointee, bool mutable);
TNode* type(Type t);
void type_condense();

bool type_has_name(Type t);
string type_get_name(Type t);
void type_attach_name(Type t, string name);

string type_to_string(Type t, bool use_names);



// ---------------------------------------------------------

typedef struct SemaNode SemaNode;
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
    STORAGE_COMPTIME, // 'def' decl
};

enum EntityCheckStatus {
    CHK_NONE, // has not been checked yet
    CHK_IN_PROGRESS, // currently being checked. value references to this entity are invalid
    CHK_IN_PROGRESS_TYPE_AVAILABLE,
    CHK_DONE,
};

typedef struct Entity {
    Type type;
    u8 storage;
    u8 check_status;
    bool mutable;

    string name;
    EntityTable* tbl;

    SemaNode* decl;
    PNode* decl_pnode;

    ConstVal constval;
} Entity;

typedef struct EntityTable {
    Entity** at;
    u32 len;
    u32 cap;

    EntityTable* parent;
} EntityTable;

// ---------------------------------------------------------

da_typedef(SemaNode);

typedef struct Module {
    string name;
    EntityTable* global;
    da(SemaNode) decls;
} Module;

enum SemaExprKind {
    SEXPR_INVALID = 0,
};

typedef struct SemaNode {
    u8 kind;
    bool mutable;
    Type type;
    PNode* pnode;
    union {
        ConstVal constval;
        struct {
            Entity* entity;
            SemaNode* value;
        } decl;
    };
} SemaNode;

enum SemaNodeKind {
    SN_INVALID = 0,

    SN_CONSTVAL,
    SN_ENTITY,

    SN_DECL,
};

Module sema_check_module(PNode* top);

#endif // SEMA_H