#include "sema.h"
#include "ptrmap.h"
#include "strbuilder.h"

TypeGraph tg;

static usize align_forward(usize ptr, usize align) {
    assert(is_pow_2(align));
    return (ptr + align - 1) & ~(align - 1);
}

TNode* type(Type t) {
    return tg.handles.at[t];
}

Type type_new(Module* m, u8 kind) {
    // fuck it, dont care about size optimization
    TNode* t = malloc(sizeof(TNode));
    memset(t, 0, sizeof(*t));
    t->kind = kind;

    // da_append(&tg.handles, t);
    tg.handles.len++;
    if (tg.handles.len > tg.handles.cap) {
        tg.handles.cap *= 2;
        tg.handles.at = realloc(tg.handles.at, tg.handles.cap * sizeof(tg.handles.at[0]));
        tg.handles.mods = realloc(tg.handles.mods, tg.handles.cap * sizeof(tg.handles.mods[0]));
        tg.handles.equiv = realloc(tg.handles.equiv, tg.handles.cap * sizeof(tg.handles.equiv[0]));
        tg.handles.names = realloc(tg.handles.names, tg.handles.cap * sizeof(tg.handles.names[0]));
        memset(tg.handles.equiv, 0, tg.handles.cap * sizeof(tg.handles.equiv[0]));
    }
    tg.handles.at[tg.handles.len - 1] = t;
    tg.handles.mods[tg.handles.len - 1] = m;
    return tg.handles.len - 1;
}

Type type_new_record(Module* m, u8 kind, usize len) {
    Type t = type_new(m, kind);
    type(t)->as_record.at = malloc(sizeof(TypeRecordField) * len);
    memset(type(t)->as_record.at, 0, sizeof(TypeRecordField) * len);
    type(t)->as_record.len = len;
    return t;
}

Type type_new_ref(Module* m, u8 kind, Type pointee, bool mutable) {
    Type t = type_new(m, kind);
    type(t)->as_ref.pointee = pointee;
    type(t)->as_ref.mutable = mutable;
    return t;
}

void type_print_graph() {
    PtrMap has_printed;
    ptrmap_init(&has_printed, 128);
    for_n(t, 0, tg.handles.len) {
        TNode* type_node = type(t);
        if (ptrmap_get(&has_printed, type_node) != PTRMAP_NOT_FOUND) {
            continue;
        }
        ptrmap_put(&has_printed, type_node, (void*)1234);

        printf("(%d, %d) (%6x) % 3d : ", type(t)->num_a, type(t)->num_b, type(t), t);
        switch(type(t)->kind) {
        case TYPE_UNKNOWN: printf("[unknown]"); break;
        case TYPE_VOID: printf("unit"); break;
        case TYPE_NEVER: printf("never"); break;
        case TYPE_BOOL: printf("bool"); break;
        case TYPE_DYN: printf("dyn"); break;
        case TYPE_TYPEID: printf("typeid"); break;
        case TYPE_I8: printf("i8"); break;
        case TYPE_U8: printf("u8"); break;
        case TYPE_I16: printf("i16"); break;
        case TYPE_U16: printf("u16"); break;
        case TYPE_I32: printf("i32"); break;
        case TYPE_U32: printf("u32"); break;
        case TYPE_I64: printf("i64"); break;
        case TYPE_U64: printf("u64"); break;
        case TYPE_F16: printf("f16"); break;
        case TYPE_F32: printf("f32"); break;
        case TYPE_F64: printf("f64"); break;
        case TYPE_UNTYPED_INT: printf("untyped_int"); break;
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

bool type_compare(Type a, Type b, bool ignore_idents, bool ignore_distinct) {
    bool is_eq = _type_compare(a, b, 1, ignore_idents, ignore_distinct);
    type_reset_num(a);
    type_reset_num(b);
    return is_eq;
}

static bool _type_compare(Type a, Type b, usize n, bool ignore_idents, bool ignore_distinct) {
    if (a == b) 
        return true;
    if (type(a) == type(b)) 
        return true;
    if (type(a)->kind != type(b)->kind) 
        return false;
    if (a < _TYPE_SIMPLE_END)
        return true;

    if (type(a)->num_a != 0 || type(b)->num_b != 0) {
        return (type(a)->num_a == type(b)->num_b);
    }

    switch (type(a)->kind) {
    case TYPE_DISTINCT:
        return false;
    case TYPE_POINTER:
    case TYPE_SLICE:
    case TYPE_BOUNDLESS_SLICE:
        if (type(a)->as_ref.mutable != type(b)->as_ref.mutable) {
            return false;
        }
        set(a, b, n);
        bool eq = _type_compare(
            type(a)->as_ref.pointee, type(b)->as_ref.pointee, 
            inc(n), ignore_idents, ignore_distinct
        );
        return eq;
    default:
        return false;
    }
    return false;
}

void type_reset_num(Type a) {
    if (type(a)->num_a == 0 && type(a)->num_b == 0) {
        return;
    }

    type(a)->num_a = 0;
    type(a)->num_b = 0;

    switch (type(a)->kind) {
    case TYPE_DISTINCT:
    case TYPE_POINTER:
    case TYPE_SLICE:
    case TYPE_BOUNDLESS_SLICE:
        type_reset_num(type(a)->as_ref.pointee);
        break;
    }
}


// condense typegraph
// might simplify the type system so this isnt necessary

// void type_condense() {
//     bool eq = true;
//     while (eq) {
//         eq = false;
//         for_n(a, _TYPE_SIMPLE_END, tg.handles.len) {
//             for_n(b, a, tg.handles.len) {
//                 if (type(a) == type(b)) continue;
//                 bool is_eq = type_compare(a, b, 1);
//                 type_reset_num(a);
//                 type_reset_num(b);
//                 if (is_eq) merge(a, b);
//                 eq = eq || is_eq;
//             }
//         }
//         for_n(from, 0, tg.handles.len) {
//             Type to = tg.handles.equiv[from];
//             if (to == 0) continue;
//             tg.handles.at[from] = tg.handles.at[to];
//             tg.handles.equiv[from] = 0;
//         }
//     }
// }

void type_attach_name(Type t, string name) {
    tg.handles.names[t] = name;
}

bool type_has_name(Type t) {
    return tg.handles.names[t].raw != NULL;
}

string type_get_name(Type t) {
    return tg.handles.names[t];
}

void type_init() {
    tg.handles.len = 0;
    tg.handles.cap = 128;
    tg.handles.at = malloc(tg.handles.cap * sizeof(tg.handles.at[0]));
    tg.handles.mods = malloc(tg.handles.cap * sizeof(tg.handles.mods[0]));
    tg.handles.equiv = malloc(tg.handles.cap * sizeof(tg.handles.equiv[0]));
    tg.handles.names = malloc(tg.handles.cap * sizeof(tg.handles.names[0]));
    memset(tg.handles.equiv, 0, tg.handles.cap * sizeof(tg.handles.equiv[0]));
    memset(tg.handles.names, 0, tg.handles.cap * sizeof(tg.handles.names[0]));

    for_n(i, TYPE_UNKNOWN, _TYPE_SIMPLE_END) {
        assert(i == type_new(NULL, i));
    }

    tg.handles.names[TYPE_VOID]   = constr("void");
    tg.handles.names[TYPE_NEVER]  = constr("never");
    tg.handles.names[TYPE_BOOL]   = constr("bool");
    tg.handles.names[TYPE_DYN]    = constr("dyn");
    tg.handles.names[TYPE_TYPEID] = constr("typeid");

    tg.handles.names[TYPE_I8]  = constr("i8");
    tg.handles.names[TYPE_U8]  = constr("u8");
    tg.handles.names[TYPE_I16] = constr("i16");
    tg.handles.names[TYPE_U16] = constr("u16");
    tg.handles.names[TYPE_I32] = constr("i32");
    tg.handles.names[TYPE_U32] = constr("u32");
    tg.handles.names[TYPE_I64] = constr("i64");
    tg.handles.names[TYPE_U64] = constr("u64");
    tg.handles.names[TYPE_F16] = constr("f16");
    tg.handles.names[TYPE_F32] = constr("f32");
    tg.handles.names[TYPE_F64] = constr("f64");

    tg.handles.names[TYPE_UNTYPED_INT] = constr("untyped_int");
    tg.handles.names[TYPE_UNTYPED_FLOAT] = constr("untyped_float");
    tg.handles.names[TYPE_UNTYPED_STRING] = constr("untyped_string");

    printf("typegraph init\n");
    // type_condense();
}

static void type_to_string_internal(StringBuilder* sb, Type t, bool use_names, usize rec_num) {
    // if this type has a name to it, use that
    if ((use_names && type_has_name(t)) || t < _TYPE_SIMPLE_END) {
        sb_append(sb, type_get_name(t));
        return;
    }

    if (type(t)->num_a != 0) {
        sb_printf(sb, "<%zu>", type(t)->num_a);
        return;
    }

    // mark this node. we may need to use its number for recursive structure indication
    type(t)->num_a = rec_num++;

    switch (type(t)->kind) {
    case TYPE_STRUCT:
    case TYPE_UNION:
        if (type(t)->kind == TYPE_STRUCT) {
            sb_append_c(sb, "struct {");
        } else {
            sb_append_c(sb, "union {");
        }

        for_n(i, 0, type(t)->as_record.len) {
            if (i != 0) sb_append_c(sb, ", ");

            TypeRecordField* field = &type(t)->as_record.at[i];
            sb_append(sb, field->name);
            sb_append_c(sb, ": ");
            type_to_string_internal(sb, field->type, use_names, rec_num);
        }
        sb_append_c(sb, "}");
        break;
    case TYPE_ARRAY:
        sb_append_c(sb, "[");
        sb_printf(sb, "%zu", type(t)->as_array.len);
        sb_append_c(sb, "]");
        type_to_string_internal(sb, type(t)->as_array.sub, use_names, rec_num);
        break;
    case TYPE_ARRAY_LEN_UNKNOWN:
        sb_append_c(sb, "[_]");
        type_to_string_internal(sb, type(t)->as_array.sub, use_names, rec_num);
        break;
    case TYPE_ENUM:
        sb_append_c(sb, "enum ");
        type_to_string_internal(sb, type(t)->as_enum.underlying, use_names, rec_num);
        sb_append_c(sb, " {");

        for_n(i, 0, type(t)->as_enum.len) {
            if (i != 0) sb_append_c(sb, ", ");

            TypeEnumVariant* variant = &type(t)->as_enum.at[i];
            sb_append(sb, variant->name);
            sb_printf(sb, " = %zi", variant->value);
        }
        sb_append_c(sb, "}");
        break;
    case TYPE_POINTER:
        sb_append_c(sb, type(t)->as_ref.mutable ? "^mut " : "^let ");
        type_to_string_internal(sb, type(t)->as_ref.pointee, use_names, rec_num);
        break;
    case TYPE_BOUNDLESS_SLICE:
        sb_append_c(sb, type(t)->as_ref.mutable ? "[^]mut " : "[^]let ");
        type_to_string_internal(sb, type(t)->as_ref.pointee, use_names, rec_num);
        break;
    case TYPE_SLICE:
        sb_append_c(sb, type(t)->as_ref.mutable ? "[]mut " : "[]let ");
        type_to_string_internal(sb, type(t)->as_ref.pointee, use_names, rec_num);
        break;
    case TYPE_DISTINCT:
        sb_append_c(sb, "distinct ");
        type_to_string_internal(sb, type(t)->as_distinct, use_names, rec_num);
        break;
    default:
        UNREACHABLE;
    }

    // unmark
    type(t)->num_a = 0;
}

string type_to_string(Type t, bool use_names) {
    StringBuilder sb;
    sb_init(&sb);
    type_to_string_internal(&sb, t, use_names, 1);

    string s = string_alloc(sb.len);
    sb_write(&sb, s.raw);

    return s;
}

bool type_is_solid_integer(Type t) {
    switch (t) {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
        return true;
    default:
        return false;
    }
}

Type type_unwrap_distinct(Type t) {
    while (type(t)->kind == TYPE_DISTINCT) {
        t = type(t)->as_distinct;
    }
    return t;
}

bool type_is_solid_float(Type t) {
    switch (t) {
    case TYPE_F16:
    case TYPE_F32:
    case TYPE_F64:
        return true;
    default:
        return false;
    }
}

bool type_is_signed_integer(Type t) {
    switch (t) {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
        return true;
    default:
        return false;
    }
}

isize type_binary_implicit_cast_priority(Type t) {
    switch (t) {
    case TYPE_BOUNDLESS_SLICE:
        return 6;
    case TYPE_POINTER:
        if (type(t)->as_ref.pointee == TYPE_VOID)
            return 5;
        return 6;
    case TYPE_I64:
    case TYPE_U64:
    case TYPE_F64:
        return 4;
    case TYPE_I32:
    case TYPE_U32:
    case TYPE_F32:
        return 3;
    case TYPE_I16:
    case TYPE_U16:
    case TYPE_F16:
        return 2;
    case TYPE_I8:
    case TYPE_U8:
        return 1;
    default:
        UNREACHABLE;
    }
}

static bool is_let_ptr(Type t) {
    return type(t)->kind == TYPE_POINTER && !type(t)->as_ref.mutable;
}

static bool is_mut_ptr(Type t) {
    return type(t)->kind == TYPE_POINTER && type(t)->as_ref.mutable;
}

static bool is_mut(Type t) {
    return type(t)->as_ref.mutable;
}

static bool is_kind(Type t, u8 kind) {
    return type(t)->kind == kind;
}

static Type pointee(Type t) {
    return type(t)->as_ref.pointee;
}

static bool type_is_numeric(Type t) {
    switch (t) {
    case TYPE_I8:
    case TYPE_U8:
    case TYPE_I16:
    case TYPE_U16:
    case TYPE_I32:
    case TYPE_U32:
    case TYPE_I64:
    case TYPE_U64:
    case TYPE_F16:
    case TYPE_F32:
    case TYPE_F64:
    case TYPE_UNTYPED_INT:
    case TYPE_UNTYPED_FLOAT:
        return true;
    default:
        return false;
    }
}

bool type_can_implicit_cast(Type from, Type to) {
    Type to_indistinct = type_unwrap_distinct(to);
    
    // T -> dyn
    if (to == TYPE_DYN) {
        return true;
    }

    // never -> T
    if (from == TYPE_NEVER) {
        return true;
    }

    // enum T -> T
    if (type(from)->kind == TYPE_ENUM && type(from)->as_enum.underlying == to) {
        return true;
    }

    // i<N> -> i<M>    where N < M
    // u<N> -> u<M>    where N < M
    if (type_is_solid_integer(from) && type_is_solid_integer(to)) {
        // cannot implicitly between signed and unsigned integers
        if (type_is_signed_integer(from) != type_is_signed_integer(to)) {
            return false;
        }
        return from < to;
    }

    // untyped_string -> []let u8  // default
    // untyped_string -> []mut u8
    // untyped_string -> [_]u8
    if (from == TYPE_UNTYPED_STRING) {
        if (type(to_indistinct)->kind == TYPE_SLICE && type(to_indistinct)->as_ref.pointee == TYPE_U8) {
            return true;
        }
        TODO("untyped_string -> [_]u8");
    }

    // untyped_int -> i<N>
    // untyped_int -> u<N>
    // untyped_int -> f<N>
    if (from == TYPE_UNTYPED_INT) {
        if (type_is_solid_integer(to_indistinct) || type_is_solid_float(to_indistinct)) {
            return true;
        }
    }

    // untyped_float -> f<N>
    if (from == TYPE_UNTYPED_FLOAT) {
        if (type_is_solid_float(to)) {
            return true;
        }
    }

    // []mut T -> []let T
    if (type(from)->kind == TYPE_SLICE && type(to)->kind == TYPE_SLICE) {
        return 
            type(from)->as_ref.mutable && 
            !type(to)->as_ref.mutable &&
            type(from)->as_ref.pointee == type(to)->as_ref.pointee;
    }

    if (is_mut_ptr(from)) {
        if (pointee(from) != TYPE_VOID) {
            // ^mut T -> ^let T
            // ^mut T -> ^let
            if (is_let_ptr(to)) return true;
            // ^mut T -> ^mut
            if (is_mut_ptr(to) && pointee(to) == TYPE_VOID) return true;

            // ^mut [N]T -> [^]mut T
            // ^mut [N]T -> [^]let T
            if (pointee(from) == TYPE_ARRAY) {
                Type T = type(pointee(from))->as_array.sub;
                if (is_kind(to, TYPE_BOUNDLESS_SLICE)) return true;
            }
        } else {
            // ^mut -> ^mut T
            // ^mut -> ^let T
            // ^mut -> ^let
            if (is_kind(to, TYPE_POINTER)) return true;
            // ^mut -> [^]mut T
            // ^mut -> [^]let T
            if (is_kind(to, TYPE_BOUNDLESS_SLICE)) return true;
        }
    } else {
        // ^let T -> ^let
        if (pointee(from) != TYPE_VOID && pointee(to) == TYPE_VOID) return true;
        // ^let   -> ^let T
        if (pointee(from) == TYPE_VOID && pointee(to) != TYPE_VOID) return true; 
        // ^let   -> [^]let T
        if (pointee(from) == TYPE_VOID && is_kind(to, TYPE_BOUNDLESS_SLICE)) return true; 
        // ^let [N]T -> [^]let T
        if (pointee(from) == TYPE_ARRAY) {
            Type T = type(pointee(from))->as_array.sub;
            if (is_kind(to, TYPE_BOUNDLESS_SLICE) && !type(to)->as_ref.mutable) return true;
        }
    }

    // fn -> ^let
    if (is_kind(from, TYPE_FUNCTION) && is_let_ptr(to) && pointee(to) == TYPE_VOID) {
        return true;
    }

    if (is_kind(from, TYPE_BOUNDLESS_SLICE)) {
        Type T = pointee(from);
        bool mut = type(from)->as_ref.mutable;
        if (mut) {
            // [^]mut T -> [^]let T
            if (is_kind(to, TYPE_BOUNDLESS_SLICE) && !is_mut(to)) {
                return pointee(to) == T;
            }
            // [^]mut T -> ^mut T
            // [^]mut T -> ^let T
            // [^]mut T -> ^mut
            // [^]mut T -> ^let
            if (is_kind(to, TYPE_POINTER)) {
                return T == TYPE_VOID || T == pointee(to);
            }
        } else {
            // [^]let T -> ^let T
            // [^]let T -> ^let 
            if (is_let_ptr(to)) {
                return T == TYPE_VOID || T == pointee(to);
            }
        }
    }

    return false;
}

bool type_can_explicit_cast(Type from, Type to) {
    if (type_can_implicit_cast(from, to)) {
        return true;
    }

    Type to = type_unwrap_distinct(to);
    Type from = type_unwrap_distinct(from);
    
    // T -> enum T

    // dyn -> T
    if (from == TYPE_DYN) {
        return true;
    }

    // any float -> any int
}