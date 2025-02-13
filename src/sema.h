#ifndef SEMA_H
#define SEMA_H

#include "parse.h"
#include "orbit/vec.h"

typedef struct Module Module;
typedef struct Entity Entity;

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

    TYPE_FUNCTION,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_ARRAY,
    TYPE_ARRAY_LEN_UNKNOWN,
    TYPE_ENUM,
    TYPE_POINTER,
    TYPE_BOUNDLESS_SLICE,
    TYPE_SLICE,
    TYPE_DISTINCT,

    TYPE__TEMP_ALIAS, 
    // temporary type node for circular structure resolution
    // after the type is constructed and the declaration is finished
    // these should be resolved
};

typedef struct TypeRecordField {
    string name;
    Type type;
    usize offset;
} TypeRecordField;

typedef struct TypeFnParam {
    string name;
    Type type;
} TypeFnParam;

typedef struct TypeEnumVariant {
    string name;
    isize value;
} TypeEnumVariant;

typedef struct TNode {
    u8 kind;
    u8 flags; // for shit
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
            struct {
                TypeFnParam* at;
                usize len;
            } params;
            Type ret_type;
        } as_function;
        struct {
            TypeEnumVariant* at;
            usize len;
            Type underlying;
        } as_enum;
        struct {
            Entity* ent;
        } as_temp_alias;
    };
} TNode;

typedef struct TypeGraph {
    struct {
        string* names; // attach a type handle to a name for better error printing
        Module** mods; // every type has a module attached to it, where it was declared
        TNode** at;
        // Type* equiv;
        u32 len;
        u32 cap;
    } handles;
} TypeGraph;
extern TypeGraph tg;

void type_init();
Type type_new(Module* m, u8 kind);
Type type_new_record(Module* m, u8 kind, usize len);
Type type_new_ref(Module* m, u8 kind, Type pointee, bool mutable);
Type type_new_array(Module* m, Type elem_type, usize len);
Type type_new_alias(Module* m, Type t);
Type type_new_array_len_unknown(Module* m, Type elem_type);
TNode* type(Type t);
// void type_condense();

bool type_is_numeric(Type t);
bool type_is_signed_integer(Type t);
bool type_is_solid_float(Type t);
bool type_is_float(Type t);
bool type_is_solid_integer(Type t);
bool type_is_integer(Type t);
bool type_is_untyped(Type t);

isize type_integer_min(Type t);
usize type_integer_max(Type t);

bool type_has_name(Type t);
string type_get_name(Type t);
void type_attach_name(Type t, string name);

string type_gen_string(Type t, bool use_names);
u64 type_gen_typeid(Type t);

Type type_unwrap_distinct(Type t);
bool type_equal(Type a, Type b, bool ignore_idents, bool ignore_distinct);
isize type_binary_implicit_cast_priority(Type t);
bool type_can_implicit_cast(Type from, Type to);
bool type_can_explicit_cast(Type from, Type to);


// ---------------------------------------------------------

typedef struct SemaNode SemaNode;
typedef struct Entity Entity;
typedef struct EntityTable EntityTable;
typedef struct ConstVal ConstVal;

// ---------------------------------------------------------

typedef struct ConstVal {
    Type type;
    union {
        bool bool;
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
        Type typeid;

        struct {
            ConstVal* at;
            u32 len;
        } compound;
    };
} ConstVal;

enum EntityStorageKind {
    STORAGE_LOCAL,
    STORAGE_PARAMETER,

    STORAGE_EXTERN,
    STORAGE_GLOBAL,
    STORAGE_COMPTIME, // 'def' decl

    STORAGE_FUNCTION,
};

enum EntityCheckStatus {
    ENT_CHECK_NONE, // has not been checked yet
    ENT_CHECK_IN_PROGRESS, // currently being checked. value references to this entity are invalid
    ENT_CHECK_IN_PROGRESS_TYPE_AVAILABLE,
    ENT_CHECK_DONE,
};

typedef struct Entity {
    Type type;
    u8 storage;
    u8 check_status;
    bool mutable;
    bool uninit;

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


typedef struct Module {
    string name;
    EntityTable* global;
    struct {
        SemaNode** at;
        u32 len;
        u32 cap;
    } decls;
} Module;

enum SemaNodeKind {
    SN_INVALID = 0,

    SN_CONSTVAL,
    SN_ENTITY,

    SN_ADD, // binop
    SN_SUB, // binop
    SN_MUL, // binop
    SN_DIV, // binop
    SN_MOD, // binop


    SN_LESS, // binop
    SN_LESS_EQ, // binop
    SN_GREATER, // binop
    SN_GREATER_EQ, // binop

    SN_IMPLICIT_CAST, // unop
    
    SN_ARRAY_INDEX, // binop
    SN_SLICE_INDEX, // binop
    SN_BOUNDLESS_SLICE_INDEX, // binop

    SN_DEREF, // unop
    SN_ADDR_OF, // unop

    SN_STMT_BLOCK,
    SN_STMT_RETURN,
    SN_STMT_ASSIGN,

    SN_STMT_IF,

    SN_VAR_DECL,

    SN_FN_DECL,
};

typedef struct SemaNode {
    u8 kind;
    bool mutable : 1; // can it be modified?
    bool addressable : 1; // can its pointer be taken?
    Type type;
    PNode* pnode;
    union {
        ConstVal constval;

        Entity* entity;

        struct {
            SemaNode* lhs;
            SemaNode* rhs;
        } binop;

        struct {
            Entity* entity;
            SemaNode* value;
        } decl;

        struct {
            SemaNode* value;
        } return_stmt;

        struct {
            SemaNode* lhs;
            SemaNode* rhs;
        } assign;

        struct {
            SemaNode* sub;
        } unop;

        struct {
            SemaNode** at;
            u32 len;
            u32 cap;
        } list;

        struct {
            SemaNode* cond;
            SemaNode* if_true;
            SemaNode* if_false;
        } if_stmt;

        // better to abstract this more
        // when anonymous functions come into play
        // but whatever
        struct {
            Entity* entity;
            EntityTable* scope;
            SemaNode* body;
        } fn_decl;
    };
} SemaNode;


VecPtr_typedef(SemaNode);

typedef struct Analyzer {
    Module* m;
    Entity* current_fn;
    Type return_type;
    VecPtr(SemaNode) defer_stack;
} Analyzer;

Module* sema_check_module(PNode* top);
SemaNode* check_expr(Analyzer* an, EntityTable* scope, PNode* pn, Type expected);
SemaNode* check_stmt(Analyzer* an, EntityTable* scope, PNode* pstmt);
SemaNode* check_var_decl(Analyzer* an, EntityTable* scope, PNode* pstmt);
Type ingest_type(Analyzer* an, EntityTable* scope, PNode* pn);

#endif // SEMA_H