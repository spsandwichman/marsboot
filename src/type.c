#include "sema.h"
#include "ptrmap.h"
#include "strbuilder.h"
#include "orbit/hash.h"

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
        // tg.handles.equiv = realloc(tg.handles.equiv, tg.handles.cap * sizeof(tg.handles.equiv[0]));
        tg.handles.names = realloc(tg.handles.names, tg.handles.cap * sizeof(tg.handles.names[0]));
        // memset(tg.handles.equiv, 0, tg.handles.cap * sizeof(tg.handles.equiv[0]));
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

Type type_new_enum(Module* m, Type underlying, usize len) {
    Type t = type_new(m, TYPE_ENUM);
    type(t)->as_enum.at = malloc(sizeof(TypeEnumVariant) * len);
    memset(type(t)->as_enum.at, 0, sizeof(TypeEnumVariant) * len);
    type(t)->as_enum.len = len;
    type(t)->as_enum.underlying = underlying;
    return t;
}

Type type_new_array(Module* m, Type elem_type, usize len) {
    // for_n (i, 0, tg.handles.len) {
        // if (type(i)->kind != TYPE_ARRAY) continue;
        // if (type(i)->as_array.len != len) continue;
        // Type t_elem_type = type(i)->as_array.sub;
        // if (!type_equal(t_elem_type, elem_type, false, false)) continue;
        // return i;
    // }


    Type t = type_new(m, TYPE_ARRAY);
    type(t)->as_array.len = len;
    type(t)->as_array.sub = elem_type;
    return t;
}

Type type_new_array_len_unknown(Module* m, Type elem_type) {
    Type t = type_new(m, TYPE_ARRAY_LEN_UNKNOWN);
    type(t)->as_array.sub = elem_type;
    return t;
}

// convert an unknown array len type into a known one
void type_canon_array(Module* m, Type array_len_unknown, usize len) {
    type(array_len_unknown)->kind = TYPE_ARRAY;
    type(array_len_unknown)->as_array.len = len;
}

Type type_new_ref(Module* m, u8 kind, Type pointee, bool mutable) {

    for_n (i, 0, tg.handles.len) {
        if (type(i)->kind != kind) continue;
        if (type(i)->as_ref.mutable != mutable) continue;
        Type t_pointee = type(i)->as_ref.pointee;
        if (!type_equal(t_pointee, pointee, false, false)) continue;
        return i;
    }

    Type t = type_new(m, kind);
    type(t)->as_ref.pointee = pointee;
    type(t)->as_ref.mutable = mutable;
    return t;
}

Type type_new_alias(Module* m, Type t) {
    Type alias = type_new(m, TYPE_UNKNOWN);
    tg.handles.at[alias] = type(t); // make the handle point to the same type node
    return alias;
}

void type_print_graph() {
    PtrMap has_printed;
    ptrmap_init(&has_printed, 128);
    for_n(t, 0, tg.handles.len) {
        TNode* type_node = type(t);
        // if (ptrmap_get(&has_printed, type_node) != PTRMAP_NOT_FOUND) {
        //     // continue;
        // }
        // ptrmap_put(&has_printed, type_node, (void*)1234);

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
        case TYPE_UNTYPED_INT: printf("untyped int"); break;
        case TYPE_UNTYPED_FLOAT: printf("untyped float"); break;
        case TYPE_UNTYPED_STRING: printf("untyped string"); break;
        case TYPE_POINTER:
            printf("^%s %d", type(t)->as_ref.mutable ? "mut" : "let", type(t)->as_ref.pointee);
            break;
        case TYPE_SLICE:
            printf("[]%s %d", type(t)->as_ref.mutable ? "mut" : "let", type(t)->as_ref.pointee);
            break;
        case TYPE_DISTINCT:
            printf("distinct %d", type(t)->as_distinct);
            break;
        case TYPE__TEMP_ALIAS:
            printf("temp alias");
            break;
        case TYPE_FUNCTION:
            break;
        default:
            crash("cannot print type %d", type(t)->kind);
        }
        printf("\n");
    }
}

static void set(Type a, Type b, usize num) {
    type(a)->num_a = num;
    type(b)->num_b = num;
}

// set up B to be merged into A on the next pass
// static void merge(Type a, Type b) {
//     tg.handles.equiv[b] = a;
// }

static void type_reset_num(Type a) {
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

static bool type_equal_internal(Type a, Type b, usize n, bool ignore_idents, bool ignore_distinct);

bool type_equal(Type a, Type b, bool ignore_idents, bool ignore_distinct) {
    bool is_eq = type_equal_internal(a, b, 1, ignore_idents, ignore_distinct);
    type_reset_num(a);
    type_reset_num(b);
    return is_eq;
}

static bool type_equal_internal(Type a, Type b, usize n, bool ignore_idents, bool ignore_distinct) {
    a = type_unwrap_alias(a);
    b = type_unwrap_alias(b);
    
    if (ignore_distinct) {
        a = type_unwrap_distinct(a);
        b = type_unwrap_distinct(b);
    }
    
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
        bool eq = type_equal_internal(
            type(a)->as_ref.pointee,
            type(b)->as_ref.pointee, 
            n+1, 
            ignore_idents, ignore_distinct
        );
        set(a, b, 0);
        return eq;
    case TYPE_ARRAY:
        if (type(a)->as_array.len != type(b)->as_array.len) {
            return false;
        }
        set(a, b, n);
        eq = type_equal_internal(
            type(a)->as_array.sub,
            type(b)->as_array.sub, 
            n+1, 
            ignore_idents, ignore_distinct
        );
        set(a, b, 0);
        return eq;
    case TYPE_STRUCT:
        if (type(a)->as_record.len != type(b)->as_record.len) {
            return false;
        }
        if (!ignore_idents) for_n (i, 0, type(a)->as_record.len) {
            TypeRecordField rf_a = type(a)->as_record.at[i];
            TypeRecordField rf_b = type(b)->as_record.at[i];
            if (!string_eq(rf_a.name, rf_b.name)) {
                return false;
            }
        }
        set(a, b, n);
        eq = true;
        for_n (i, 0, type(a)->as_record.len) {
            TypeRecordField rf_a = type(a)->as_record.at[i];
            TypeRecordField rf_b = type(b)->as_record.at[i];
            eq = eq && type_equal_internal(
                rf_a.type,
                rf_b.type, 
                n+1, 
                ignore_idents, ignore_distinct
            );
        }
        set(a, b, 0);
        return eq;
    default:
        return false;
    }
    return false;
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
//                 bool is_eq = type_equal(a, b, 1);
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
    // tg.handles.equiv = malloc(tg.handles.cap * sizeof(tg.handles.equiv[0]));
    tg.handles.names = malloc(tg.handles.cap * sizeof(tg.handles.names[0]));
    // memset(tg.handles.equiv, 0, tg.handles.cap * sizeof(tg.handles.equiv[0]));
    memset(tg.handles.names, 0, tg.handles.cap * sizeof(tg.handles.names[0]));

    for_n(i, TYPE_UNKNOWN, _TYPE_SIMPLE_END) {
        assert(i == type_new(NULL, i));
    }

    tg.handles.names[TYPE_UNKNOWN] = constr("!UNKNOWN!");

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

    // printf("typegraph init\n");
    // type_condense();
}

static void type_gen_string_internal(StringBuilder* sb, Type t, bool use_names, usize rec_num) {
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
    case TYPE_FUNCTION:
        sb_append_c(sb, "fun (");
        for_n(i, 0, type(t)->as_function.params.len) {
            if (i != 0) sb_append_c(sb, ", ");

            TypeFnParam* field = &type(t)->as_function.params.at[i];
            sb_append(sb, field->name);
            sb_append_c(sb, ": ");
            type_gen_string_internal(sb, field->type, use_names, rec_num);
        }
        sb_append_c(sb, ")");
        if (type(t)->as_function.ret_type != TYPE_VOID) {
            sb_append_c(sb, ": ");
            type_gen_string_internal(sb, type(t)->as_function.ret_type, use_names, rec_num);
        }
        break;
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
            type_gen_string_internal(sb, field->type, use_names, rec_num);
        }
        sb_append_c(sb, "}");
        break;
    case TYPE_ARRAY:
        sb_append_c(sb, "[");
        sb_printf(sb, "%zu", type(t)->as_array.len);
        sb_append_c(sb, "]");
        type_gen_string_internal(sb, type(t)->as_array.sub, use_names, rec_num);
        break;
    case TYPE_ARRAY_LEN_UNKNOWN:
        sb_append_c(sb, "[_]");
        type_gen_string_internal(sb, type(t)->as_array.sub, use_names, rec_num);
        break;
    case TYPE_ENUM:
        sb_append_c(sb, "enum ");
        type_gen_string_internal(sb, type(t)->as_enum.underlying, use_names, rec_num);
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
        sb_append_c(sb, type(t)->as_ref.mutable ? "^mut" : "^let");
        if (type(t)->as_ref.pointee != TYPE_VOID) {
            sb_append_char(sb, ' ');
            type_gen_string_internal(sb, type(t)->as_ref.pointee, use_names, rec_num);
        }
        break;
    case TYPE_BOUNDLESS_SLICE:
        sb_append_c(sb, type(t)->as_ref.mutable ? "[^]mut " : "[^]let ");
        type_gen_string_internal(sb, type(t)->as_ref.pointee, use_names, rec_num);
        break;
    case TYPE_SLICE:
        sb_append_c(sb, type(t)->as_ref.mutable ? "[]mut " : "[]let ");
        type_gen_string_internal(sb, type(t)->as_ref.pointee, use_names, rec_num);
        break;
    case TYPE_DISTINCT:
        sb_append_c(sb, "distinct ");
        type_gen_string_internal(sb, type(t)->as_distinct, use_names, rec_num);
        break;
    case TYPE_UNKNOWN:
        sb_append_c(sb, "!UNKNOWN!");
        break;
    default:
        printf("unable %d\n", type(t)->kind);
        UNREACHABLE;
    }

    // unmark
    type(t)->num_a = 0;
}

string type_gen_string(Type t, bool use_names) {
    StringBuilder sb;
    sb_init(&sb);
    type_gen_string_internal(&sb, t, use_names, 1);

    string s = string_alloc(sb.len);
    sb_write(&sb, s.raw);

    return s;
}

bool type_is_solid_integer(Type t) {
    switch (type(t)->kind) {
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

isize type_integer_min(Type t) {
    if (!type_is_integer(t)) {
        return 0;
    }

    static const usize minimums[] = {
        [TYPE_I8]  = INT8_MIN,
        [TYPE_I16] = INT16_MIN,
        [TYPE_I32] = INT32_MIN,
        [TYPE_I64] = INT64_MIN,
        [TYPE_UNTYPED_INT] = INT64_MIN,

        [TYPE_U8]  = 0,
        [TYPE_U16] = 0,
        [TYPE_U32] = 0,
        [TYPE_U64] = 0,
    };
    return minimums[t];
}

usize type_integer_max(Type t) {
    if (!type_is_integer(t)) {
        return 0;
    }

    static const usize minimums[] = {
        [TYPE_I8]  = INT8_MAX,
        [TYPE_I16] = INT16_MAX,
        [TYPE_I32] = INT32_MAX,
        [TYPE_I64] = INT64_MAX,
        [TYPE_UNTYPED_INT] = UINT64_MAX,

        [TYPE_U8]  = UINT8_MAX,
        [TYPE_U16] = UINT16_MAX,
        [TYPE_U32] = UINT32_MAX,
        [TYPE_U64] = UINT64_MAX,
    };
    return minimums[t];
}

bool type_is_integer(Type t) {
    return t == TYPE_UNTYPED_INT || type_is_solid_integer(t);
}

Type type_unwrap_distinct(Type t) {
    while (type(t)->kind == TYPE_DISTINCT) {
        t = type(t)->as_distinct;
    }
    return t;
}

bool type_is_solid_float(Type t) {
    switch (type(t)->kind) {
    case TYPE_F16:
    case TYPE_F32:
    case TYPE_F64:
        return true;
    default:
        return false;
    }
}

bool type_is_float(Type t) {
    return t == TYPE_UNTYPED_FLOAT || type_is_solid_float(t);
}

bool type_is_unsigned_integer(Type t) {
    switch (type(t)->kind) {
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
        return true;
    default:
        return false;
    }
}

bool type_is_signed_integer(Type t) {
    switch (type(t)->kind) {
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
    switch (type(t)->kind) {
    case TYPE_BOUNDLESS_SLICE:
        return 6;
    case TYPE_POINTER:
        if (type(t)->as_ref.pointee == TYPE_VOID)
            return 5;
        return 6;
    case TYPE_I64:
    case TYPE_U64:
        return 4;
    case TYPE_I32:
    case TYPE_U32:
        return 3;
    case TYPE_I16:
    case TYPE_U16:
        return 2;
    case TYPE_I8:
    case TYPE_U8:
    case TYPE_UNTYPED_INT:
        return 1;
    default:
        return 0;
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

bool type_is_equalable(Type t) {
    switch (type(t)->kind) {
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE:
    case TYPE_TYPEID:
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
    case TYPE_ENUM:
        return true;
    default:
        return false;
    }
}

bool type_is_pointer(Type t) {
    return type(t)->kind == TYPE_POINTER || type(t)->kind == TYPE_BOUNDLESS_SLICE;
}

bool type_is_numeric(Type t) {
    switch (type(t)->kind) {
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE:
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
    case TYPE_ENUM:
        return true;
    default:
        return false;
    }
}

bool type_is_untyped(Type t) {
    switch (type(t)->kind) {
    case TYPE_UNTYPED_INT:
    case TYPE_UNTYPED_FLOAT:
    case TYPE_UNTYPED_STRING:
        return true;
    default:
        return false;
    }
}

Type type_unwrap_alias(Type t) {
    if (t >= _TYPE_SIMPLE_END && type(t)->kind < _TYPE_SIMPLE_END) {
        t = type(t)->kind;
    }
    return t;
}

bool type_can_implicit_cast(Type from, Type to) {
    from = type_unwrap_alias(from);
    to   = type_unwrap_alias(to);
    if (from == to) return true;

    if (type_equal(from, to, false, false)) {
        return true;
    }
    
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

    to = type_unwrap_distinct(to);
    from = type_unwrap_distinct(from);

    if (type_can_implicit_cast(from, to)) {
        return true;
    }
    
    // enum unwrapping
    if (is_kind(to, TYPE_ENUM)) {
        return type_can_explicit_cast(from, type(to)->as_enum.underlying);
    }
    if (is_kind(from, TYPE_ENUM)) {
        return type_can_explicit_cast(type(to)->as_enum.underlying, to);
    }

    // dyn -> T
    if (from == TYPE_DYN) {
        return true;
    }

    // any float <-> any int
    if (type_is_numeric(from) && type_is_numeric(to)) {
        return true;
    }

    // any numeric/intlike -> bool
    if (to == TYPE_BOOL && (type_is_numeric(from) || type(from)->kind == TYPE_POINTER || type(from)->kind == TYPE_BOUNDLESS_SLICE)) {
        return true;
    }

    // ^let/mut T -> uint
    // ^let/mut T -> int
    // [^]let/mut T -> uint
    // [^]let/mut T -> int
    if (is_kind(from, TYPE_POINTER) || is_kind(from, TYPE_BOUNDLESS_SLICE)) {
        return to == TYPE_I64 || to == TYPE_U64;
    }

    return false;
}

static void type_gen_typeid_internal(Hash* h, Type t, u64 n) {
    if (type(t)->num_a) {
        hash_fnv1a_u64(h, (u64)type(t)->num_a);
        return;
    }
    switch (type(t)->kind) {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
    case TYPE_F16:
    case TYPE_F32:
    case TYPE_F64:
        string typename = type_get_name(t);
        hash_fnv1a(h, typename.raw, typename.len);
        break;
    case TYPE_DISTINCT:
        // this will probably break as soon as 
        // mutliple module compilation gets working
        //
        // `distinct` types need to have a unique 
        // type hash, but that hash needs to be unique
        // and consistent across multiple compilations.
        // it shouldn't change based on import or analysis order.
        // but somehow needs to be unique from everything else.
        hash_fnv1a(h, "distinct", strlen("distinct"));
        // maybe hash the module name as well?
        hash_fnv1a_u64(h, (u64)t);
        break;
    case TYPE_SLICE:
        hash_fnv1a(h, type(t)->as_ref.mutable ? "[]mut" : "[]let", 5);
        type(t)->num_a = n;
        type_gen_typeid_internal(h, type(t)->as_ref.pointee, n+1);
        type(t)->num_a = 0;
        return;
    case TYPE_POINTER:
        hash_fnv1a(h, type(t)->as_ref.mutable ? "^mut" : "^let", 4);
        type(t)->num_a = n;
        type_gen_typeid_internal(h, type(t)->as_ref.pointee, n+1);
        type(t)->num_a = 0;
        return;
    default:
        UNREACHABLE;
    }
}

u64 type_gen_typeid(Type t) {
    Hash h = hash_new();
    type_gen_typeid_internal(&h, t, 1);
    return (u64) h;
}

// when calling for the first time, use n = 1;
bool type_is_infinite(Type t) {
    if (type(t)->num_a) return true;

    switch (type(t)->kind) {
    case TYPE_ARRAY:
    case TYPE_ARRAY_LEN_UNKNOWN:
        type(t)->num_a = 1;
        bool is_infinite = type_is_infinite(type(t)->as_array.sub);
        type(t)->num_a = 0;
        return is_infinite;
    case TYPE_STRUCT:
    case TYPE_UNION:
        type(t)->num_a = 1;
        is_infinite = false;
        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField* rf = &type(t)->as_record.at[i];
            is_infinite = is_infinite || type_is_infinite(rf->type);
        }

        type(t)->num_a = 0;
        return is_infinite;
    default:
        return false;
    }
}

usize type_calculate_size(Type t) {
    switch (type(t)->kind) {
    case TYPE_I8:  case TYPE_U8: return 1;
    case TYPE_I16: case TYPE_U16: return 2;
    case TYPE_I32: case TYPE_U32: return 4;

    case TYPE_POINTER: 
    case TYPE_BOUNDLESS_SLICE:
    case TYPE_TYPEID:
    case TYPE_I64: case TYPE_U64: return 8;

    case TYPE_F16: return 2;
    case TYPE_F32: return 4;
    case TYPE_F64: return 8;
    case TYPE_SLICE: case TYPE_DYN: return 16;

    case TYPE_ENUM:
        return type_calculate_size(type(t)->as_enum.underlying);
    case TYPE_ARRAY:
        return type_calculate_size(type(t)->as_array.sub) * type(t)->as_array.len;
    case TYPE_STRUCT:
        if (type(t)->as_record.size != 0) {
            return type(t)->as_record.size;
        }
        if (type(t)->as_record.len == 0) {
            return 0;
        }
        usize size = 0;
        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField* rf = &type(t)->as_record.at[i];
            size = align_forward(size, type_calculate_align(rf->type));
            size += type_calculate_size(rf->type);
        }

        size = align_forward(size, type_calculate_align(t));
        type(t)->as_record.size = size;
        return size;
    default:
        UNREACHABLE;
    }
}

usize type_calculate_align(Type t) {
    switch (type(t)->kind) {
    case TYPE_I8:  case TYPE_U8: return 1;
    case TYPE_I16: case TYPE_U16: return 2;
    case TYPE_I32: case TYPE_U32: return 4;

    case TYPE_POINTER: 
    case TYPE_BOUNDLESS_SLICE:
    case TYPE_TYPEID:
    case TYPE_I64: case TYPE_U64: return 8;

    case TYPE_F16: return 2;
    case TYPE_F32: return 4;
    case TYPE_F64:
    case TYPE_SLICE: 
    case TYPE_DYN: return 8;

    case TYPE_ENUM:
        return type_calculate_align(type(t)->as_enum.underlying);

    case TYPE_ARRAY:
        return type_calculate_align(type(t)->as_array.sub);

    case TYPE_STRUCT:
    case TYPE_UNION:
        if (type(t)->as_record.align != 0) {
            return type(t)->as_record.align;
        }
        if (type(t)->as_record.len == 0) {
            return 0;
        }
        usize align = 0;
        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField* rf = &type(t)->as_record.at[i];
            usize field_align = type_calculate_align(rf->type);
            align = max(align, field_align);
        }

        type(t)->as_record.align = align;
        return align;
    default:
        UNREACHABLE;
    }
}