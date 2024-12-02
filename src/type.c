#include "sema.h"

TypeGraph tg;

static usize align_forward(usize ptr, usize align) {
    assert(is_pow_2(align));
    return (ptr + align - 1) & ~(align - 1);
}

TNode* type(Type t) {
    return tg.handles.at[t];
}

Type type_new(u8 kind) {
    // fuck it, dont care about size optimization
    TNode* t = malloc(sizeof(TNode));
    memset(t, 0, sizeof(*t));
    t->kind = kind;

    // da_append(&tg.handles, t);
    tg.handles.len++;
    if (tg.handles.len > tg.handles.cap) {
        tg.handles.cap *= 2;
        tg.handles.at = realloc(tg.handles.at, tg.handles.cap * sizeof(tg.handles.at[0]));
        tg.handles.equiv = realloc(tg.handles.equiv, tg.handles.cap * sizeof(tg.handles.equiv[0]));
        memset(tg.handles.equiv, 0, tg.handles.cap * sizeof(tg.handles.equiv[0]));
    }
    tg.handles.at[tg.handles.len - 1] = t;
    return tg.handles.len - 1;
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
    for_range(t, 0, tg.handles.len) {
        printf("(%d, %d) (%6x) % 3d : ", type(t)->num_a, type(t)->num_b, type(t), t);
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
    type(a)->num_a = num;
    type(b)->num_b = num;
}

// set up B to be merged into A on the next pass
static void merge(Type a, Type b) {
    tg.handles.equiv[b] = a;
}

bool type_compare(Type a, Type b, usize n) {
    if (a == b) 
        return true;
    if (type(a) == type(b)) 
        return true;
    if (type(a)->kind != type(b)->kind) 
        return false;

    if (type(a)->num_a != 0 || type(b)->num_b != 0) {
        return (type(a)->num_a == type(b)->num_b);
    }

    switch (type(a)->kind) {
    case TYPE_DISTINCT:
        return false;
    case TYPE_POINTER:
        if (type(a)->as_ref.mutable != type(b)->as_ref.mutable) {
            return false;
        }
        set(a, b, n);
        bool eq = type_compare(type(a)->as_ref.pointee, type(b)->as_ref.pointee, inc(n));
        set(a, b, 0);
        return eq;
    default:
        return false;
    }
    return false;
}

// condense typegraph
// O(fucking horrible)
void type_condense() {
    bool eq = false;
    for_range(a, _TYPE_SIMPLE_END, tg.handles.len) {
        for_range(b, a, tg.handles.len) {
            if (type(a) == type(b)) continue;
            eq = type_compare(a, b, 1);
            // if (eq) printf("a %d <- b %d\n", a, b);
            if (eq) merge(a, b);
        }
    }
    for_range(from, 0, tg.handles.len) {
        Type to = tg.handles.equiv[from];
        if (to == 0) continue;
        tg.handles.at[from] = tg.handles.at[to];
        tg.handles.equiv[from] = 0;
    }
}

void type_init() {
    tg.handles.len = 0;
    tg.handles.cap = 128;
    tg.handles.at = malloc(tg.handles.cap * sizeof(tg.handles.at[0]));
    tg.handles.equiv = malloc(tg.handles.cap * sizeof(tg.handles.equiv[0]));
    memset(tg.handles.equiv, 0, tg.handles.cap * sizeof(tg.handles.equiv[0]));

    for_range(i, TYPE_NONE, _TYPE_SIMPLE_END) {
        assert(i == type_new(i));
    }

    printf("typesystem init\n");

    {
        Type a = type_new_ref(TYPE_POINTER, TYPE_NONE, false);
        Type link = type_new_ref(TYPE_POINTER, a, false);
        for_range(i, 0, 1000) {
            link = type_new_ref(TYPE_POINTER, link, false);
        }
        // link = type_new_ref(TYPE_POINTER, link, false);
        type(a)->as_ref.pointee = link;
    }

    type_print_graph();
    printf("\n");
    type_condense();

    type_print_graph();
}