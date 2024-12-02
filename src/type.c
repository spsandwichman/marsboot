#include "sema.h"

TypeGraph tg;

static usize align_forward(usize ptr, usize align) {
    assert(is_pow_2(align));
    return (ptr + align - 1) & ~(align - 1);
}

TNode* type(Type t) {
    return tg.handle2node.at[t];
}

Type type_new(u8 kind) {
    // fuck it, dont care about size optimization
    TNode* t = malloc(sizeof(TNode));
    memset(t, 0, sizeof(*t));
    t->kind = kind;

    da_append(&tg.handle2node, t);
    return tg.handle2node.len - 1;
}

Type type_new_record(u8 kind, usize len) {
    Type t = type_new(kind);
    type(t)->as_record.at = malloc(sizeof(TypeRecordField) * len);
    memset(type(t)->as_record.at, 0, sizeof(TypeRecordField) * len);
    type(t)->as_record.len = len;
    return t;
}

Type type_new_ref(u8 kind, Type pointee, bool mutable) {
    Type t = type_new(kind);
    type(t)->as_ref.pointee = pointee;
    type(t)->as_ref.mutable = mutable;
    return t;
}

void type_print_graph() {
    for_range(t, 0, tg.handle2node.len) {
        printf("(%6x) % 3d : ", type(t), t);
        switch(type(t)->kind) {
        case TYPE_NONE: printf("none"); break;
        case TYPE_BOOL: printf("bool"); break;
        case TYPE_I8: printf("i8"); break;
        case TYPE_U8: printf("u8"); break;
        case TYPE_I16: printf("i16"); break;
        case TYPE_U16: printf("u16"); break;
        case TYPE_I32: printf("i32"); break;
        case TYPE_U32: printf("u32"); break;
        case TYPE_I64: printf("i64"); break;
        case TYPE_U64: printf("u64"); break;
        case TYPE_UNTYPED_INT: printf("untyped_int"); break;
        case TYPE_F16: printf("f16"); break;
        case TYPE_F32: printf("f32"); break;
        case TYPE_F64: printf("f64"); break;
        case TYPE_UNTYPED_FLOAT: printf("untyped_float"); break;
        case TYPE_UNTYPED_STRING: printf("untyped_string"); break;
        case TYPE_POINTER:
            printf("^%s %d", type(t)->as_ref.mutable ? "mut" : "let", type(t)->as_ref.pointee);
            break;
        case TYPE_SLICE:
            printf("[]%s %d", type(t)->as_ref.mutable ? "mut" : "let", type(t)->as_ref.pointee);
            break;
        case TYPE_DISTINCT:
            printf("distinct %d", type(t)->as_distinct);

        default:
            crash("cannot print type");
        }
        printf("\n");
    }
}

static usize inc(usize n) {
    return n ? (n + 1) : 0;
}

static void set(Type a, Type b, usize num) {
    type(a)->num = num;
    type(b)->num = num;
}

// merge type b into type a
static void merge(Type a, Type b) {
    tg.handle2node.at[b] = tg.handle2node.at[a];
}

// if compress != 0, merge type b to type a
bool type_compare(Type a, Type b, usize compress) {
    if (a == b) 
        return true;
    if (type(a) == type(b)) 
        return true;
    if (type(a)->kind != type(b)->kind) 
        return false;

    bool eq = false;
    if (type(a)->num != 0 || type(b)->num != 0) {
        if (type(a)->num != type(b)->num)
            return false;
        eq = true;
    }
    if (eq) {
        if (compress) merge(a, b);
        return true;
    }

    switch (type(a)->kind) {
    case TYPE_DISTINCT:
        return false;
    case TYPE_POINTER:
        if (type(a)->as_ref.mutable != type(b)->as_ref.mutable) {
            return false;
        }
        set(a, b, compress);
        eq = type_compare(type(a)->as_ref.pointee, type(b)->as_ref.pointee, inc(compress));
        set(a, b, 0);
        break;
    default:
        return false;
    }

    if (eq) {
        if (compress) merge(a, b);
        return true;
    }
    return false;
}

// condense typegraph
// O(fucking horrible)
void type_condense() {
    redo:
    bool eq = false;
    for_range(a, _TYPE_SIMPLE_END, tg.handle2node.len) {
        for_range(b, a, tg.handle2node.len) {
            if (type(a) == type(b)) continue;
            eq = type_compare(a, b, true);
            // if (eq) goto redo;
            TODO("change fron merging on the fly to creating a list and merging equivalences");
        }
    }
    // assert(!eq);
}

void type_init() {
    da_init(&tg.handle2node, 128);

    for_range(i, TYPE_NONE, _TYPE_SIMPLE_END) {
        assert(i == type_new(i));
    }

    printf("typesystem init\n");

    {
        Type a = type_new_ref(TYPE_POINTER, TYPE_NONE, false);
        Type link = type_new_ref(TYPE_POINTER, a, false);
        link = type_new_ref(TYPE_POINTER, link, false);
        link = type_new_ref(TYPE_POINTER, link, false);
        link = type_new_ref(TYPE_POINTER, link, false);
        link = type_new_ref(TYPE_POINTER, link, false);
        link = type_new_ref(TYPE_POINTER, link, false);
        link = type_new_ref(TYPE_POINTER, link, false);
        type(a)->as_ref.pointee = link;
    }

    // type_compare(a, b, true);
    type_print_graph();
    printf("\n");
    type_condense();

    type_print_graph();
}