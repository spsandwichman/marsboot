#include "sema.h"

static void report_pnode(bool error, PNode* pn, char* message, ...) {
    va_list args;
    va_start(args, message);

    SourceFile* f = ctx_find_from_substring(pn->base.raw, pn->base.len);
    assert(f != NULL);

    string highlight = {pn->base.raw, pn->base.len};

    emit_report(error, f->text, f->path, highlight, message, args);
    va_end(args);
}

EntityTable* etbl_new(EntityTable* parent) {
    EntityTable* scope = malloc(sizeof(*scope));
    memset(scope, 0, sizeof(*scope));
    scope->parent = parent;
    da_init(scope, 32);
    return scope;
}

Entity* etbl_search(EntityTable* scope, string name) {
    if (scope == NULL) return NULL;
    foreach (Entity* e, *scope) {
        if (string_eq(e->name, name)) {
            return e;
        }
    }
    return etbl_search(scope->parent, name);
}

Entity* etbl_put(EntityTable* scope, string name) {
    Entity* ent = malloc(sizeof(*ent));
    memset(ent, 0, sizeof(*ent));
    ent->name = name;
    da_append(scope, ent);
    return ent;
}

SemaNode* new_node(Analyzer* an, PNode* pn, u8 kind) {
    SemaNode* node = malloc(sizeof(*node));
    memset(node, 0, sizeof(*node));
    node->kind = kind;
    node->pnode = pn;
    return node;
}

ConstVal constval_compound(Type t, u32 len) {
    ConstVal cv;
    cv.type = t;
    cv.compound.at = malloc(len * sizeof(ConstVal));
    memset(cv.compound.at, 0, len * sizeof(ConstVal));
    cv.compound.len = len;
    return cv;
} 

static usize eval_integer(PNode* pn, char* raw, usize len) {
    if (len == 1) {
        return raw[0] - '0';
    }
    usize value = 0;
    usize base = 10;
    if (raw[0] == '0') switch (raw[1]) {
    case 'x':
    case 'X':
        base = 16;
        raw += 2; 
        len -= 2; 
        break;
    case 'o':
    case 'O':
        base = 8;
        raw += 2; 
        len -= 2; 
        break;
    case 'b':
    case 'B':
        base = 2; 
        raw += 2; 
        len -= 2; 
        break;
    }

    for_n(i, 0, len) {
        char c = raw[i];
        if (c == '_') continue;
        usize cval = 0;
        if ('0' <= c && c <= '9') {
            cval = c - '0';
        } else if ('a' <= c && c <= 'f') {
            cval = c - 'a' + 10;
        } else if ('A' <= c && c <= 'F') {
            cval = c - 'A' + 10;
        }
        if (cval >= base) {
            report_pnode(true, pn, "'%c' is not a valid base-%d digit", c, base);
        }
        // TODO add overflow check here
        value = value * base + cval;
    }
    return value;
}

static Type normalize_untyped(Module* m, Type t) {

    switch (t) {
    case TYPE_UNTYPED_FLOAT: 
        return TYPE_F64;
    case TYPE_UNTYPED_INT: 
        return TYPE_I64;
    case TYPE_UNTYPED_STRING:
        return type_new_ref(m, TYPE_SLICE, TYPE_U8, false);
    }
    return t;
}

SemaNode* check_expr_integer(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    // get integer value

    i64 value = (i64)eval_integer(pn, pn->base.raw, pn->base.len);

    SemaNode* integer = new_node(an, pn, SN_CONSTVAL);
    integer->constval = (ConstVal){
        .i64 = value,
        .type = TYPE_UNTYPED_INT,
    };
    integer->type = TYPE_UNTYPED_INT;
    return integer;
}


SemaNode* check_expr_string(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    string span = pnode_span(pn);
    string value = {0};
    // trace length
    for_n (i, 1, span.len - 1) {
        char c = span.raw[i];
        if (c == '\\') {
            UNREACHABLE;
        }
        ++value.len;
    }
    value.raw = malloc(value.len);
    usize cursor = 0;
    for_n (i, 1, span.len - 1) {
        char c = span.raw[i];
        if (c == '\\') {
            UNREACHABLE;
        }
        value.raw[cursor] = c;
        ++cursor;
    }

    SemaNode* str = new_node(an, pn, SN_CONSTVAL);
    str->constval = (ConstVal){
        .string = value,
        .type = TYPE_UNTYPED_STRING,
        .is_string = true,
    };
    str->type = TYPE_UNTYPED_STRING;
    return str;
}

bool type_is_intlike(Type t) {
    return type_is_integer(t) 
        || type(t)->kind == TYPE_POINTER 
        || type(t)->kind == TYPE_BOUNDLESS_SLICE;
}

#define apply_unop(result, typekind, sub, op) \
    switch (typekind) { \
    case TYPE_U8:   (result).u8 =  op (sub).u8;  break;\
    case TYPE_U16:  (result).u16 = op (sub).u16; break;\
    case TYPE_U32:  (result).u32 = op (sub).u32; break;\
    case TYPE_U64:  (result).u64 = op (sub).u64; break;\
    case TYPE_I8:   (result).i8 =  op (sub).i8;  break;\
    case TYPE_I16:  (result).i16 = op (sub).i16; break;\
    case TYPE_I32:  (result).i32 = op (sub).i32; break;\
    case TYPE_UNTYPED_INT: \
    case TYPE_POINTER: \
    case TYPE_BOUNDLESS_SLICE: \
    case TYPE_I64:  (result).i64 = op (sub).i64; break;\
    case TYPE_F16:  (result).f16 = op (sub).f16; break;\
    case TYPE_F32:  (result).f32 = op (sub).f32; break;\
    case TYPE_F64:  (result).f64 = op (sub).f64; break;\
    default: UNREACHABLE; \
    }

#define apply_unop_integer(result, typekind, sub, op) \
    switch (typekind) { \
    case TYPE_U8:   (result).u8 =  op (sub).u8;  break;\
    case TYPE_U16:  (result).u16 = op (sub).u16; break;\
    case TYPE_U32:  (result).u32 = op (sub).u32; break;\
    case TYPE_U64:  (result).u64 = op (sub).u64; break;\
    case TYPE_I8:   (result).i8 =  op (sub).i8;  break;\
    case TYPE_I16:  (result).i16 = op (sub).i16; break;\
    case TYPE_I32:  (result).i32 = op (sub).i32; break;\
    case TYPE_UNTYPED_INT: \
    case TYPE_POINTER: \
    case TYPE_BOUNDLESS_SLICE: \
    case TYPE_I64:  (result).i64 = op (sub).i64; break;\
    default: UNREACHABLE; \
    }

#define apply_unop_bool(result, typekind, sub, op) \
    switch (typekind) { \
    case TYPE_U8:   (result).bool = op (sub).u8;  break;\
    case TYPE_U16:  (result).bool = op (sub).u16; break;\
    case TYPE_U32:  (result).bool = op (sub).u32; break;\
    case TYPE_U64:  (result).bool = op (sub).u64; break;\
    case TYPE_I8:   (result).bool = op (sub).i8;  break;\
    case TYPE_I16:  (result).bool = op (sub).i16; break;\
    case TYPE_I32:  (result).bool = op (sub).i32; break;\
    case TYPE_UNTYPED_INT: \
    case TYPE_POINTER: \
    case TYPE_BOUNDLESS_SLICE: \
    case TYPE_I64:  (result).bool = op (sub).i64; break;\
    case TYPE_F16:  (result).bool = op (sub).f16; break;\
    case TYPE_F32:  (result).bool = op (sub).f32; break;\
    case TYPE_F64:  (result).bool = op (sub).f64; break;\
    default: UNREACHABLE; \
    }

#define apply_binop(result, typekind, lhs, op, rhs) \
    switch (typekind) { \
    case TYPE_U8:   (result).u8 =  (lhs).u8  op (rhs).u8;  break;\
    case TYPE_U16:  (result).u16 = (lhs).u16 op (rhs).u16; break;\
    case TYPE_U32:  (result).u32 = (lhs).u32 op (rhs).u32; break;\
    case TYPE_U64:  (result).u64 = (lhs).u64 op (rhs).u64; break;\
    case TYPE_I8:   (result).i8 =  (lhs).i8  op (rhs).i8;  break;\
    case TYPE_I16:  (result).i16 = (lhs).i16 op (rhs).i16; break;\
    case TYPE_I32:  (result).i32 = (lhs).i32 op (rhs).i32; break;\
    case TYPE_UNTYPED_INT: \
    case TYPE_POINTER: \
    case TYPE_BOUNDLESS_SLICE: \
    case TYPE_I64:  (result).i64 = (lhs).i64 op (rhs).i64; break;\
    case TYPE_F16:  (result).f16 = (lhs).f16 op (rhs).f16; break;\
    case TYPE_F32:  (result).f32 = (lhs).f32 op (rhs).f32; break;\
    case TYPE_F64:  (result).f64 = (lhs).f64 op (rhs).f64; break;\
    default: UNREACHABLE; \
    }

#define apply_binop_integer(result, typekind, lhs, op, rhs) \
    switch (typekind) { \
    case TYPE_U8:   (result).u8 =  (lhs).u8  op (rhs).u8;  break;\
    case TYPE_U16:  (result).u16 = (lhs).u16 op (rhs).u16; break;\
    case TYPE_U32:  (result).u32 = (lhs).u32 op (rhs).u32; break;\
    case TYPE_U64:  (result).u64 = (lhs).u64 op (rhs).u64; break;\
    case TYPE_I8:   (result).i8 =  (lhs).i8  op (rhs).i8;  break;\
    case TYPE_I16:  (result).i16 = (lhs).i16 op (rhs).i16; break;\
    case TYPE_I32:  (result).i32 = (lhs).i32 op (rhs).i32; break;\
    case TYPE_UNTYPED_INT: \
    case TYPE_POINTER: \
    case TYPE_BOUNDLESS_SLICE: \
    case TYPE_I64:  (result).i64 = (lhs).i64 op (rhs).i64; break;\
    default: UNREACHABLE; \
    }

#define apply_bool_binop(result, typekind, lhs, op, rhs) \
    switch (typekind) { \
    case TYPE_U8:   (result).bool =  (lhs).u8  op (rhs).u8;  break;\
    case TYPE_U16:  (result).bool = (lhs).u16 op (rhs).u16; break;\
    case TYPE_U32:  (result).bool = (lhs).u32 op (rhs).u32; break;\
    case TYPE_U64:  (result).bool = (lhs).u64 op (rhs).u64; break;\
    case TYPE_I8:   (result).bool =  (lhs).i8  op (rhs).i8;  break;\
    case TYPE_I16:  (result).bool = (lhs).i16 op (rhs).i16; break;\
    case TYPE_I32:  (result).bool = (lhs).i32 op (rhs).i32; break;\
    case TYPE_UNTYPED_INT: \
    case TYPE_POINTER: \
    case TYPE_BOUNDLESS_SLICE: \
    case TYPE_I64:  (result).bool = (lhs).i64 op (rhs).i64; break;\
    case TYPE_F16:  (result).bool = (lhs).f16 op (rhs).f16; break;\
    case TYPE_F32:  (result).bool = (lhs).f32 op (rhs).f32; break;\
    case TYPE_F64:  (result).bool = (lhs).f64 op (rhs).f64; break;\
    default: UNREACHABLE; \
    }

// +, -, *, /, %
void const_eval_expr_arith_binop(Analyzer* m, SemaNode* n, u8 kind) {
    SemaNode* lhs = n->binop.lhs;
    SemaNode* rhs = n->binop.rhs;

    if (lhs->kind != SN_CONSTVAL || rhs->kind != SN_CONSTVAL) {
        return;
    }

    n->kind = SN_CONSTVAL;
    n->constval.type = n->type;

    switch (kind) {
    case PN_EXPR_ADD: apply_binop(n->constval, n->type, lhs->constval, +, rhs->constval); break;
    case PN_EXPR_SUB: apply_binop(n->constval, n->type, lhs->constval, +, rhs->constval); break;
    case PN_EXPR_MUL: apply_binop(n->constval, n->type, lhs->constval, *, rhs->constval); break;
    case PN_EXPR_DIV: apply_binop(n->constval, n->type, lhs->constval, /, rhs->constval); break;
    case PN_EXPR_MOD: apply_binop_integer(n->constval, n->type, lhs->constval, %, rhs->constval); break;
    case PN_EXPR_LESS: apply_bool_binop(n->constval, n->type, lhs->constval, <, rhs->constval); break;
    case PN_EXPR_LESS_EQ: apply_bool_binop(n->constval, n->type, lhs->constval, <=, rhs->constval); break;
    case PN_EXPR_GREATER: apply_bool_binop(n->constval, n->type, lhs->constval, >, rhs->constval); break;
    case PN_EXPR_GREATER_EQ: apply_bool_binop(n->constval, n->type, lhs->constval, >=, rhs->constval); break;
    default:
        UNREACHABLE;
    }
}

void const_eval_expr_bit_binop(Analyzer* m, SemaNode* n, u8 kind) {
    SemaNode* lhs = n->binop.lhs;
    SemaNode* rhs = n->binop.rhs;

    if (lhs->kind != SN_CONSTVAL || rhs->kind != SN_CONSTVAL) {
        return;
    }

    n->kind = SN_CONSTVAL;
    n->constval.type = n->type;

    switch (kind) {
    case PN_EXPR_BIT_AND: apply_binop_integer(n->constval, n->type, lhs->constval, &, rhs->constval); break;
    case PN_EXPR_BIT_OR: apply_binop_integer(n->constval, n->type, lhs->constval, |, rhs->constval); break;
    case PN_EXPR_XOR: apply_binop_integer(n->constval, n->type, lhs->constval, ^, rhs->constval); break;
    default:
        UNREACHABLE;
    }
}

SemaNode* insert_implicit_cast(Analyzer* an, SemaNode* n, Type to) {
    if (n->type == to || type(n->type) == type(to)) {
        return n;
    }

    // if its a constant val, we can just change reassign the type
    // TODO there might be more conversion to do here,
    // probably separate that out into a different function
    if (n->kind == SN_CONSTVAL) {
        Type undistinct = type_unwrap_distinct(to);

        // TODO ACTUALLY CHECK INTEGER BOUNDS STATICALLY LMAO
        // if (type_is_integer(undistinct)) {
        //     isize minimum = type_integer_min(undistinct);
        //     isize maximum = type_integer_max(undistinct);
        //     if (n->constval.i64 < minimum || n->constval.i64 > maximum) {
        //         string typestr = type_gen_string(to, true);
        //         report_pnode(false, n->pnode, "constant is out of range for '"str_fmt"'", str_arg(typestr));
        //     }
        // }
        n->type = to;
        n->constval.type = to;
        return n;
    }

    // printf("inserting implicit cast\n");

    SemaNode* impl_cast = new_node(an, NULL, SN_IMPLICIT_CAST);
    impl_cast->unop.sub = n;
    impl_cast->type = to;
    return impl_cast;
}


// ==, !=
SemaNode* check_expr_equality(Analyzer* an, u8 kind, EntityTable* scope, PNode* pn, Type expected) {

    SemaNode* lhs = check_expr(an, scope, pn->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(an, scope, pn->binop.rhs, TYPE_UNKNOWN);
    SemaNode* binop = new_node(an, pn, SN_EQ);
    switch (kind) {
    case PN_EXPR_EQ:  binop->kind = SN_EQ; break;
    case PN_EXPR_NEQ: binop->kind = SN_NEQ; break;
    }

    if (!type_is_equalable(lhs->type)) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        report_pnode(true, lhs->pnode, "cannot test equality on '"str_fmt"'", str_arg(lhs_typestr));
    }
    if (!type_is_equalable(rhs->type)) {
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, rhs->pnode, "cannot test equality on '"str_fmt"'", str_arg(rhs_typestr));
    }

    if (!type_equal(lhs->type, rhs->type, false, false)) {
        if (type_can_implicit_cast(lhs->type, rhs->type)) {
            lhs = insert_implicit_cast(an, lhs, rhs->type);
            binop->type = rhs->type;
        } else if (type_can_implicit_cast(rhs->type, lhs->type)) {
            rhs = insert_implicit_cast(an, rhs, lhs->type);
            binop->type = lhs->type;
        }
    } else {
        binop->type = rhs->type;
    }

    binop->binop.lhs = lhs;
    binop->binop.rhs = rhs;

    if (binop->type == TYPE_UNKNOWN) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
    }

    if (lhs->kind == SN_CONSTVAL && rhs->kind == SN_CONSTVAL) {
        apply_bool_binop(binop->constval, binop->type, lhs->constval, ==, rhs->constval);
    }

    binop->type = TYPE_BOOL;

    return binop;
}

// <, >, <=, >=
SemaNode* check_expr_comparison(Analyzer* an, u8 kind, EntityTable* scope, PNode* pn, Type expected) {

    SemaNode* lhs = check_expr(an, scope, pn->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(an, scope, pn->binop.rhs, TYPE_UNKNOWN);
    SemaNode* binop = new_node(an, pn, SN_LESS);
    switch (kind) {
    case PN_EXPR_LESS: binop->kind = SN_LESS; break;
    case PN_EXPR_LESS_EQ: binop->kind = SN_LESS_EQ; break;
    case PN_EXPR_GREATER: binop->kind = SN_GREATER; break;
    case PN_EXPR_GREATER_EQ: binop->kind = SN_GREATER_EQ; break;
    }

    if (!type_is_numeric(lhs->type)) {
        report_pnode(true, lhs->pnode, "expression type is not numeric");
    }
    if (!type_is_numeric(rhs->type)) {
        report_pnode(true, rhs->pnode, "expression type is not numeric");
    }

    if (!type_equal(lhs->type, rhs->type, false, false)) {
        if (type_can_implicit_cast(lhs->type, rhs->type)) {
            lhs = insert_implicit_cast(an, lhs, rhs->type);
            binop->type = rhs->type;
        } else if (type_can_implicit_cast(rhs->type, lhs->type)) {
            rhs = insert_implicit_cast(an, rhs, lhs->type);
            binop->type = lhs->type;
        }
    } else {
        binop->type = rhs->type;
    }

    binop->binop.lhs = lhs;
    binop->binop.rhs = rhs;

    if (binop->type == TYPE_UNKNOWN) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
    }

    if (lhs->kind == SN_CONSTVAL && rhs->kind == SN_CONSTVAL) {
        const_eval_expr_arith_binop(an, binop, kind);
    }

    binop->type = TYPE_BOOL;

    return binop;
}


SemaNode* check_expr_boolean_binop(Analyzer* an, u8 kind, EntityTable* scope, PNode* pn, Type expected) {
    SemaNode* lhs = check_expr(an, scope, pn->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(an, scope, pn->binop.rhs, TYPE_UNKNOWN);
    SemaNode* binop = new_node(an, pn, SN_BOOL_AND);
    if (kind == PN_EXPR_BOOL_OR) {
        binop->kind = SN_BOOL_OR;
    }

    if (type_unwrap_distinct(lhs->type) != TYPE_BOOL) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        report_pnode(true, lhs->pnode, "expected bool, not '"str_fmt"'", str_arg(lhs_typestr));
    }
    
    if (type_unwrap_distinct(rhs->type) != TYPE_BOOL) {
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, rhs->pnode, "expected bool, not '"str_fmt"'", str_arg(rhs_typestr));
    }

    binop->binop.lhs = lhs;
    binop->binop.rhs = rhs;
    binop->type = TYPE_BOOL;
    
    if (lhs->type == rhs->type) {
        binop->type = lhs->type;
    }

    if (lhs->kind == SN_CONSTVAL && rhs->kind == SN_CONSTVAL) {
        switch (kind) {
        case SN_BOOL_AND: 
            binop->constval.bool = lhs->constval.bool & rhs->constval.bool;
            break;
        case SN_BOOL_OR: 
            binop->constval.bool = lhs->constval.bool | rhs->constval.bool;
            break;
        default:
            UNREACHABLE;
        }
        binop->kind = SN_CONSTVAL;
    }

    return binop;
}

// +, -, *, /, %
SemaNode* check_expr_arith_binop(Analyzer* an, u8 kind, EntityTable* scope, PNode* pn, Type expected) {
    
    SemaNode* lhs = check_expr(an, scope, pn->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(an, scope, pn->binop.rhs, TYPE_UNKNOWN);
    SemaNode* binop = new_node(an, pn, SN_ADD);
    binop->type = TYPE_UNKNOWN;
    switch (kind) {
    case PN_EXPR_ADD: binop->kind = SN_ADD; break;
    case PN_EXPR_SUB: binop->kind = SN_SUB; break;
    case PN_EXPR_MUL: binop->kind = SN_MUL; break;
    case PN_EXPR_DIV: binop->kind = SN_DIV; break;
    case PN_EXPR_MOD: binop->kind = SN_MOD; break;
    }

    if (!type_is_numeric(lhs->type)) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        report_pnode(true, lhs->pnode, "expected numeric type, not '"str_fmt"'", str_arg(lhs_typestr));
    }
    if (!type_is_numeric(rhs->type)) {
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, rhs->pnode, "expected numeric type, not '"str_fmt"'", str_arg(rhs_typestr));
    }

    // Type ptr_operation = TYPE_UNKNOWN;
    // if ((type(rhs->type)->kind == TYPE_POINTER) && (type(rhs->type)) {

    // } 

    if (kind == PN_EXPR_MOD && (type_is_float(lhs->type) || type_is_float(rhs->type))) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
    }

    if (!type_equal(lhs->type, rhs->type, false, false)) {
        if (type_can_implicit_cast(lhs->type, rhs->type)) {
            lhs = insert_implicit_cast(an, lhs, rhs->type);
            binop->type = rhs->type;
        } else if (type_can_implicit_cast(rhs->type, lhs->type)) {
            rhs = insert_implicit_cast(an, rhs, lhs->type);
            binop->type = lhs->type;
        }
    } else {
        binop->type = rhs->type;
    }

    binop->binop.lhs = lhs;
    binop->binop.rhs = rhs;

    if (binop->type == TYPE_UNKNOWN) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
    }

    if (lhs->kind == SN_CONSTVAL && rhs->kind == SN_CONSTVAL) {
        const_eval_expr_arith_binop(an, binop, kind);
    }

    return binop;
}


// &, |, ~, ~|
SemaNode* check_expr_bitwise_binop(Analyzer* an, u8 kind, EntityTable* scope, PNode* pn, Type expected) {
    
    SemaNode* lhs = check_expr(an, scope, pn->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(an, scope, pn->binop.rhs, TYPE_UNKNOWN);
    SemaNode* binop = new_node(an, pn, SN_BIT_AND);
    binop->type = TYPE_UNKNOWN;
    switch (kind) {
    case PN_EXPR_BIT_AND: binop->kind = SN_BIT_AND; break;
    case PN_EXPR_BIT_OR: binop->kind = SN_BIT_OR; break;
    case PN_EXPR_XOR: binop->kind = SN_BIT_XOR; break;
    }

    if (!type_is_intlike(lhs->type)) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        report_pnode(true, lhs->pnode, "expected numeric type, not '"str_fmt"'", str_arg(lhs_typestr));
    }
    if (!type_is_intlike(rhs->type)) {
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, rhs->pnode, "expected numeric type, not '"str_fmt"'", str_arg(rhs_typestr));
    }

    if (kind == PN_EXPR_MOD && (type_is_float(lhs->type) || type_is_float(rhs->type))) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
    }

    if (!type_equal(lhs->type, rhs->type, false, false)) {
        if (type_can_implicit_cast(lhs->type, rhs->type)) {
            lhs = insert_implicit_cast(an, lhs, rhs->type);
            binop->type = rhs->type;
        } else if (type_can_implicit_cast(rhs->type, lhs->type)) {
            rhs = insert_implicit_cast(an, rhs, lhs->type);
            binop->type = lhs->type;
        }
    } else {
        binop->type = rhs->type;
    }

    binop->binop.lhs = lhs;
    binop->binop.rhs = rhs;

    if (binop->type == TYPE_UNKNOWN) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
    }

    if (lhs->kind == SN_CONSTVAL && rhs->kind == SN_CONSTVAL) {
        const_eval_expr_bit_binop(an, binop, kind);
    }

    return binop;
}

SemaNode* check_expr_bool_coerce(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    
    SemaNode* sub = check_expr(an, scope, pn->unop.sub, TYPE_UNKNOWN);
    SemaNode* coerce = new_node(an, pn, SN_CAST);
    coerce->type = TYPE_BOOL;

    if (type_can_explicit_cast(sub->type, TYPE_BOOL)) {
        string typestr = type_gen_string(sub->type, true);
        report_pnode(true, sub->pnode, "cannot coerce from '"str_fmt"' to bool", str_arg(typestr));
    }
    coerce->unop.sub = sub;
    if (sub->kind == SN_CONSTVAL) {
        apply_unop_bool(coerce->constval, sub->kind, sub->constval, (bool));
        coerce->kind = SN_CONSTVAL;
    }

    return coerce;
}

// SemaNode* check_expr_arith_unop(Analyzer* an, u8 kind, EntityTable* scope, PNode* pn, Type expected) {

// }

SemaNode* check_expr_addrof(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    SemaNode* addrof = new_node(an, pn, SN_ADDR_OF);

    if (type(expected)->kind == TYPE_POINTER) {
        expected = type(expected)->as_ref.pointee;
    } else {
        expected = TYPE_UNKNOWN;
    }

    SemaNode* subexpr = check_expr(an, scope, pn->unop.sub, expected);

    if (!subexpr->addressable) {
        report_pnode(true, subexpr->pnode, "expression is not addressable");
    }

    Type ptr = type_new_ref(an->m, TYPE_POINTER, subexpr->type, subexpr->mutable);

    addrof->type = ptr;
    addrof->unop.sub = subexpr;
    return addrof;
}

SemaNode* check_expr_ident(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    string ident = pnode_span(pn);
    Entity* ent = etbl_search(scope, ident);
    if (ent == NULL) {
        report_pnode(true, pn, "undefined symbol '"str_fmt"'", str_arg(ident));
    }
    // this entitiy is global, but hasnt been checked yet. lets go do that
    if (ent->check_status == ENT_CHECK_NONE) {
        SemaNode* decl = check_var_decl(an, ent->tbl, ent->decl_pnode);
    } else if (ent->check_status == ENT_CHECK_IN_PROGRESS || 
               ent->check_status == ENT_CHECK_IN_PROGRESS_TYPE_AVAILABLE) {
        if (ent->type == TYPE_TYPEID && ent->decl_pnode->decl.kind == DECLKIND_DEF) {
            SemaNode* cv = new_node(an, pn, SN_CONSTVAL);
            cv->constval.typeid = ingest_type(an, scope, pn);
            cv->type = cv->constval.type = TYPE_TYPEID;
            return cv;
        } else {
            report_pnode(true, pn, "cannot define '"str_fmt"' in terms of itself", str_arg(ident));
        }
    }

    // if storage is comptime, return the value as a SN_CONSTVAL
    // else, return a SN_ENTITY reference to the variable
    if (ent->storage == STORAGE_COMPTIME) {
        SemaNode* cv = new_node(an, pn, SN_CONSTVAL);
        cv->constval = ent->constval;
        cv->type = cv->constval.type;
        return cv;
    } else {
        SemaNode* sn_entity = new_node(an, pn, SN_ENTITY);
        sn_entity->entity = ent;
        sn_entity->type = ent->type;
        sn_entity->mutable = ent->mutable;
        sn_entity->addressable = true;
        if (ent->storage == STORAGE_FUNCTION || ent->storage == STORAGE_EXTERN_FUNCTION) {
            sn_entity->addressable = false;
        }
        return sn_entity;
    }
    UNREACHABLE;
}


SemaNode* check_expr_deref(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    SemaNode* access = new_node(an, pn, SN_DEREF);

    SemaNode* ptr = check_expr(an, scope, pn->unop.sub, expected);
    access->unop.sub = ptr;
    
    if (type(ptr->type)->kind != TYPE_POINTER) {
        string typestr = type_gen_string(ptr->type, true);
        report_pnode(true ,pn->binop.lhs, "cannot dereference type '"str_fmt"'", str_arg(typestr));
    }
    
    access->type = type(ptr->type)->as_ref.pointee;
    access->mutable = type(ptr->type)->as_ref.mutable;

    return access;
}

SemaNode* check_expr_index(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    SemaNode* access = new_node(an, pn, SN_ARRAY_INDEX);

    SemaNode* indexee = check_expr(an, scope, pn->binop.lhs, expected);
    switch (type(indexee->type)->kind) {
    case TYPE_ARRAY:
    case TYPE_ARRAY_LEN_UNKNOWN:
        access->type = type(indexee->type)->as_array.sub;
        access->mutable = indexee->mutable;
        break;
    case TYPE_BOUNDLESS_SLICE:
        access->kind = SN_BOUNDLESS_SLICE_INDEX;
        access->type = type(indexee->type)->as_ref.pointee;
        access->mutable = type(indexee->type)->as_ref.mutable;
        break;
    case TYPE_SLICE:
        access->kind = SN_SLICE_INDEX;
        access->type = type(indexee->type)->as_ref.pointee;
        access->mutable = type(indexee->type)->as_ref.mutable;
        break;
    default:
        string typestr = type_gen_string(indexee->type, true);
        report_pnode(true ,pn->binop.lhs, "cannot index type '"str_fmt"'", str_arg(typestr));
    }

    SemaNode* indexer = check_expr(an, scope, pn->binop.rhs, expected);

    // indexer must be an integer of some kind
    bool is_integer = type_is_integer(type_unwrap_distinct(indexer->type));
    if (!is_integer) {
        report_pnode(true, pn->binop.rhs, "cannot index by non-integer");
    }
    access->binop.lhs = indexee;
    access->binop.rhs = indexer;
    access->addressable = indexer->addressable;

    return access;
}

SemaNode* check_expr_range(Analyzer* an, EntityTable* scope, PNode* pn) {
    
    SemaNode* lhs = check_expr(an, scope, pn->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(an, scope, pn->binop.rhs, TYPE_UNKNOWN);

    if (!type_is_numeric(lhs->type)) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        report_pnode(true, lhs->pnode, "type must be numeric, got '"str_fmt"'", str_arg(lhs_typestr));
    }
    if (!type_is_numeric(rhs->type)) {
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, rhs->pnode, "type must be numeric, got '"str_fmt"'", str_arg(rhs_typestr));
    }

    bool type_resolved = false;
    if (!type_equal(lhs->type, rhs->type, false, false)) {
        if (type_can_implicit_cast(lhs->type, rhs->type)) {
            lhs = insert_implicit_cast(an, lhs, rhs->type);
            type_resolved = true;
        } else if (type_can_implicit_cast(rhs->type, lhs->type)) {
            rhs = insert_implicit_cast(an, rhs, lhs->type);
            type_resolved = true;
        }
    } else {
        type_resolved = true;
    }

    if (!type_resolved) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
    }

    // lhs->type = normalize_untyped(an->m, lhs->type);

    SemaNode* range = new_node(an, pn, pn->base.kind == PN_EXPR_RANGE_LESS ? SN_RANGE_LESS : SN_RANGE_EQ);
    range->binop.lhs = lhs;
    range->binop.rhs = rhs;
    range->type = lhs->type;
    return range;
}

// SemaNode* check_expr_in(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
//     SemaNode* in_range = new_node(an, pn, SN_INVALID);
//     in_range->type = TYPE_BOOL;

//     u8 rhs_kind = pn->binop.rhs->base.kind;
//     if (rhs_kind == PN_EXPR_RANGE_EQ) {
//         in_range->kind = SN_IN_RANGE_EQ;
//     } else if (rhs_kind == PN_EXPR_RANGE_LESS) {
//         in_range->kind = SN_IN_RANGE_LESS;
//     } else {
//         report_pnode(true, pn->binop.rhs, "expected range expression");
//     }

//     SemaNode* value = check_expr(an, scope, pn->binop.lhs, TYPE_UNKNOWN);
//     if (!type_is_numeric(value->type)) {
//         report_pnode(true, pn->binop.lhs, "type must be numeric");

//     }
//     SemaNode* range = check_expr_range(an, scope, pn->binop.rhs);

//     bool type_resolved = false;
//     if (!type_equal(value->type, range->type, false, false)) {
//         if (type_can_implicit_cast(value->type, range->type)) {
//             value = insert_implicit_cast(an, value, range->type);
//             type_resolved = true;
//         } else if (type_can_implicit_cast(range->type, value->type)) {
//             range->binop.lhs = insert_implicit_cast(an, range->binop.lhs, value->type);
//             range->binop.rhs = insert_implicit_cast(an, range->binop.rhs, value->type);
//             type_resolved = true;
//         }
//     } else {
//         type_resolved = true;
//     }

//     if (!type_resolved) {
//         string lhs_typestr = type_gen_string(value->type, true);
//         string rhs_typestr = type_gen_string(range->type, true);
//         report_pnode(true, pn, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
//             str_arg(lhs_typestr), str_arg(rhs_typestr));
//     }

//     in_range->in_range.value = value;
//     in_range->in_range.range = range;

//     return in_range;
// }

SemaNode* check_expr_call(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    SemaNode* call = new_node(an, pn, SN_CALL);
    call->call.len = pn->call.param_list->list.len;
    call->call.at = malloc(sizeof(call->call.at[0]) * call->call.len);
    
    // get callee
    SemaNode* callee = check_expr(an, scope, pn->call.callee, TYPE_UNKNOWN);
    Type callee_type = type_unwrap_distinct(callee->type);
    if (type(callee_type)->kind != TYPE_FUNCTION) {
        string typestr = type_gen_string(callee->type, true);
        report_pnode(true, pn->call.callee, "type must be a function, got '"str_fmt"'", str_arg(typestr));
    }

    usize real_arg_num = pn->call.param_list->list.len;
    usize expected_arg_num = type(callee_type)->as_function.params.len;

    if (real_arg_num != expected_arg_num) {
        report_pnode(true, pn, "expected %zu arguments, got %zu", expected_arg_num, real_arg_num);
    }

    for_n(i, 0, real_arg_num) {
        PNode* param_pnode = pn->call.param_list->list.at[i];
        Type expected_param_type = type(callee_type)->as_function.params.at[i].type;

        SemaNode* value = check_expr(an, scope, param_pnode, expected_param_type);

        if (type_can_implicit_cast(value->type, expected_param_type)) {
            value = insert_implicit_cast(an, value, expected_param_type);
        } else {
            string value_type_str = type_gen_string(value->type, true);
            string param_type_str = type_gen_string(expected_param_type, true);
            report_pnode(true, value->pnode, "cannot coerce from '"str_fmt"' to '"str_fmt"'", 
                str_arg(value_type_str),
                str_arg(param_type_str)
            );
        }

        call->call.at[i] = value;
    }

    call->call.callee = callee;
    call->type = type(callee_type)->as_function.ret_type;

    // UNREACHABLE;
    return call;
}

SemaNode* check_expr_selector(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    SemaNode* expr = new_node(an, pn, SN_SELECTOR);

    SemaNode* selectee = check_expr(an, scope, pn->binop.lhs, TYPE_UNKNOWN);

    Type t = selectee->type;
    expr->mutable = selectee->mutable;
    if (type(t)->kind == TYPE_POINTER) {
        expr->selector.through_pointer = true;
        expr->mutable = type(t)->as_ref.mutable;
        t = type(t)->as_ref.pointee;
    }

    string selector = pnode_span(pn->binop.rhs);

    switch (type(t)->kind) {
    case TYPE_SLICE:
        if (string_eq(selector, constr("len"))) {
            expr->kind = SN_SLICE_SELECTOR_LEN;
            expr->type = TYPE_U64;
        } else if (string_eq(selector, constr("raw"))) {
            expr->kind = SN_SLICE_SELECTOR_RAW;
            TNode* slice_type = type(selectee->type);
            expr->type = type_new_ref(an->m, TYPE_BOUNDLESS_SLICE, slice_type->as_ref.pointee, slice_type->as_ref.mutable);
        } else {
            report_pnode(true, pn->binop.rhs, "slice has no field '"str_fmt"'", str_arg(selector));
        }
        break;
    case TYPE_STRUCT:
        expr->selector.field = UINT64_MAX;
        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField rf = type(t)->as_record.at[i];
            if (string_eq(rf.name, selector)) {
                expr->selector.field = i;
                expr->type = rf.type;
                break;
            }
        }
        if (expr->selector.field == UINT64_MAX) {
            report_pnode(true, pn->binop.rhs, "struct has no field '"str_fmt"'", str_arg(selector));
        }
        break;
    default:
        UNREACHABLE;
    }

    expr->addressable = true;
    expr->selector.selectee = selectee;
    return expr;
}

SemaNode* check_expr(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    if (expected == TYPE_DYN) {
        expected = TYPE_UNKNOWN;
        // TODO("insert dyn boxing");
    }

    SemaNode* expr = NULL;
    switch (pn->base.kind) {
    case PN_IDENT:
        expr = check_expr_ident(an, scope, pn, expected);
        break;
    case PN_EXPR_INTEGER:
        expr = check_expr_integer(an, scope, pn, expected);
        break;
    case PN_EXPR_NULL:
        expr = new_node(an, pn, SN_CONSTVAL);
        expr->constval.type = type_new_ref(an->m, TYPE_POINTER, TYPE_VOID, true);
        expr->type = expr->constval.type;
        expr->constval.i64 = 0;
        break;
    case PN_EXPR_STRING:
        expr = check_expr_string(an, scope, pn, expected);
        break;
    case PN_TYPE_I8 ... PN_TYPE_TYPEID:
    case PN_TYPE_STRUCT:
    case PN_TYPE_UNION:
    case PN_TYPE_ENUM:
    case PN_TYPE_ARRAY:
    case PN_TYPE_DISTINCT:
    case PN_TYPE_POINTER:
    case PN_TYPE_SLICE:
    case PN_TYPE_BOUNDLESS_SLICE:
        Type t = ingest_type(an, scope, pn);
        expr = new_node(an, pn, SN_CONSTVAL);
        expr->constval.type = TYPE_TYPEID;
        expr->type = TYPE_TYPEID;
        expr->constval.typeid = t;
        break;
    case PN_EXPR_ADD:
    case PN_EXPR_SUB:
    case PN_EXPR_MUL:
    case PN_EXPR_DIV:
    case PN_EXPR_MOD:
        expr = check_expr_arith_binop(an, pn->base.kind, scope, pn, expected);
        break;
    case PN_EXPR_BIT_AND:
    case PN_EXPR_BIT_OR:
    case PN_EXPR_XOR:
        expr = check_expr_bitwise_binop(an, pn->base.kind, scope, pn, expected);
        break;
    case PN_EXPR_BOOL_AND:
    case PN_EXPR_BOOL_OR:
        expr = check_expr_boolean_binop(an, pn->base.kind, scope, pn, expected);
        break;
    case PN_EXPR_BOOL_COERCE:
        expr = check_expr_bool_coerce(an, scope, pn, expected);
        break;
    case PN_EXPR_LESS:
    case PN_EXPR_LESS_EQ:
    case PN_EXPR_GREATER:
    case PN_EXPR_GREATER_EQ:
        expr = check_expr_comparison(an, pn->base.kind, scope, pn, expected);
        break;
    case PN_EXPR_EQ:
    case PN_EXPR_NEQ:
        expr = check_expr_equality(an, pn->base.kind, scope, pn, expected);
        break;
    case PN_EXPR_INDEX:
        expr = check_expr_index(an, scope, pn, expected);
        break;
    case PN_EXPR_DEREF:
        expr = check_expr_deref(an, scope, pn, expected);
        break;
    case PN_EXPR_ADDR:
        expr = check_expr_addrof(an, scope, pn, expected);
        break;
    case PN_EXPR_SELECTOR:
        expr = check_expr_selector(an, scope, pn, expected);
        break;
    case PN_EXPR_CALL:
        expr = check_expr_call(an, scope, pn, expected);
        break;
    case PN_DISCARD:
        report_pnode(true, pn, "_ not allowed here");
    case PN_EXPR_RANGE_EQ:
    case PN_EXPR_RANGE_LESS:
            report_pnode(true, pn, "range not allowed here");
    default:
        report_pnode(true, pn, "expected valid expression");
    }
    // string typename = type_gen_string(expr->type, true);
    // report_pnode(false, pn, "type: "str_fmt" value: %d", str_arg(typename), expr->constval.i64);
    return expr;
}

bool check_record_name_already_exists(Type t, string ident, usize current_len) {
    for_n(i, 0, current_len) {
        if (string_eq(type(t)->as_record.at[i].name, ident)) {
            return true;
        }
    }
    return false;
}

Type ingest_type(Analyzer* an, EntityTable* scope, PNode* pn) {
    switch (pn->base.kind) {
    case PN_TYPE_I8:  return TYPE_I8;
    case PN_TYPE_I16: return TYPE_I16;
    case PN_TYPE_I32: return TYPE_I32;
    case PN_TYPE_INT:
    case PN_TYPE_I64: return TYPE_I64;

    case PN_TYPE_U8:  return TYPE_U8;
    case PN_TYPE_U16: return TYPE_U16;
    case PN_TYPE_U32: return TYPE_U32;
    case PN_TYPE_UINT:
    case PN_TYPE_U64: return TYPE_U64;

    case PN_TYPE_F16: return TYPE_F16;
    case PN_TYPE_F32: return TYPE_F32;
    case PN_TYPE_FLOAT:
    case PN_TYPE_F64: return TYPE_F64;

    case PN_TYPE_BOOL:   return TYPE_BOOL;
    case PN_TYPE_DYN:    return TYPE_DYN;
    case PN_TYPE_TYPEID: return TYPE_TYPEID;

    case PN_TYPE_ARRAY:
        SemaNode* length = check_expr(an, scope, pn->array_type.len, TYPE_UNKNOWN);
        if (length->kind != SN_CONSTVAL || !type_is_integer(length->type)) {
            report_pnode(true, pn->array_type.len, "array length must be a constant integer");
        }
        Type sub = ingest_type(an, scope, pn->array_type.sub);
        Type arr = type_new_array(an->m, sub, length->constval.i64);
        return arr;
    case PN_TYPE_DISTINCT:
        sub = ingest_type(an, scope, pn->unop.sub);
        Type distinct = type_new(an->m, TYPE_DISTINCT);
        type(distinct)->as_distinct = sub;
        return distinct;
    case PN_TYPE_POINTER:
        Type pointee = TYPE_VOID;
        if (pn->ref_type.sub) {
            pointee = ingest_type(an, scope, pn->ref_type.sub);
        }
        Type ptr = type_new_ref(an->m, TYPE_POINTER, pointee, pn->ref_type.mutable);
        return ptr;
    case PN_TYPE_BOUNDLESS_SLICE:
        pointee = ingest_type(an, scope, pn->ref_type.sub);
        ptr = type_new_ref(an->m, TYPE_BOUNDLESS_SLICE, pointee, pn->ref_type.mutable);
        return ptr;
    case PN_TYPE_SLICE:
        pointee = ingest_type(an, scope, pn->ref_type.sub);
        ptr = type_new_ref(an->m, TYPE_SLICE, pointee, pn->ref_type.mutable);
        return ptr;
    case PN_IDENT: {
        string ident = pnode_span(pn);
        Entity* ent = etbl_search(scope, ident);
        if (ent == NULL) {
            report_pnode(true, pn, "undefined symbol '"str_fmt"'", str_arg(ident));
        }
        // this entitiy is global, but hasnt been checked yet. lets go do that
        if (ent->check_status == ENT_CHECK_NONE) {
            if (ent->decl_pnode->decl.kind != DECLKIND_DEF) {
                report_pnode(true, pn, "symbol is not a compile-time constant");
            }
            SemaNode* decl = check_var_decl(an, ent->tbl, ent->decl_pnode);
        }
        if (ent->check_status == ENT_CHECK_COMPLETE) {
            SemaNode* snode = check_expr_ident(an, scope, pn, TYPE_UNKNOWN);
            if (snode->type != TYPE_TYPEID) {
                report_pnode(true, pn, "symbol is not a typeid");
            }
            return snode->constval.typeid;
        }

        // create temporary alias node
        Type alias = type_new(an->m, TYPE__TEMP_ALIAS);
        type(alias)->as_temp_alias.ent = ent;
        type(alias)->as_temp_alias.use = pn;
        return alias;
    }
    case PN_TYPE_STRUCT: {
        usize num_fields = 0;
        for_n(i, 0, pn->record_type.fields->list.len) {
            PNode* item = pn->record_type.fields->list.at[i];
            if (item->item.ident->base.kind == PN_LIST) {
                num_fields += item->item.ident->list.len;
            } else {
                num_fields += 1;
            }
        }
        // printf("num fields %d\n", num_fields);
        Type record = type_new_record(an->m, TYPE_STRUCT, num_fields);

        usize field = 0;
        for_n(i, 0, pn->record_type.fields->list.len) {
            PNode* item = pn->record_type.fields->list.at[i];
            if (item->item.ident->base.kind == PN_LIST) {
                Type field_type = ingest_type(an, scope, item->item.type);
                for_n(j, 0, item->item.ident->list.len) {
                    PNode* pnode = item->item.ident->list.at[j];
                    string ident = pnode_span(pnode);
                    if (check_record_name_already_exists(record, ident, field)) {
                        report_pnode(true, pnode, "field name already exists");
                    }
                    type(record)->as_record.at[field].name = ident;
                    type(record)->as_record.at[field].type = field_type;
                    field++;
                }
            } else {
                string ident = pnode_span(item);
                if (check_record_name_already_exists(record, ident, field)) {
                    report_pnode(true, item, "field name already exists");
                }
                Type field_type = ingest_type(an, scope, item->item.type);
                type(record)->as_record.at[field].name = ident;
                type(record)->as_record.at[field].type = field_type;
                field++;
            }
        }

        return record;
    }
    default:
        report_pnode(false, pn, "expected compile-time constant typeid");
        UNREACHABLE;
    }
    return TYPE_UNKNOWN;
}

static void resolve_temp_aliases(Entity* ent, bool is_const_type) {
    for_n(i, _TYPE_SIMPLE_END, tg.handles.len) {
        if (type(i)->kind != TYPE__TEMP_ALIAS) continue;
        if (type(i)->as_temp_alias.ent != ent) continue;
        if (is_const_type) {
            TNode* target = type(ent->constval.typeid);
            tg.handles.at[i] = target;
        } else {
            report_pnode(true, type(i)->as_temp_alias.use, "symbol is not a compile-time constant typeid");
        }
    }
}

static bool can_global_decl_value(SemaNode* n) {
    if (n->kind == SN_CONSTVAL) return true;
    return false;
}

static bool maybe_type(PNode* pn) {
    u8 kind = pn->base.kind;
    return (PN_TYPE_TYPEOF <= kind && kind <= PN_TYPE_TYPEID);
}

SemaNode* check_def_decl(Analyzer* an, EntityTable* scope, PNode* pstmt) {

    assert(pstmt->decl.ident->base.kind == PN_IDENT); // no lists of decls
    string name = pnode_span(pstmt->decl.ident);

    bool is_global_decl = scope == an->m->global;

    Entity* ent = etbl_search(scope, name);
    if (is_global_decl) {
        if (ent->check_status == ENT_CHECK_COMPLETE && ent->decl_pnode == pstmt) {
            return ent->decl;
        }
    }

    if ((is_global_decl && ent->decl_pnode != pstmt) || (scope != an->m->global && ent != NULL)) {
        report_pnode(true, pstmt->decl.ident, "name already declared");
    } else if (!is_global_decl) {
        ent = etbl_put(scope, name);
    }

    ent->check_status = ENT_CHECK_IN_PROGRESS;
    ent->decl_pnode = pstmt;
    ent->type = TYPE_UNKNOWN;
    ent->storage = STORAGE_COMPTIME;

    if (pstmt->decl.type) {
        ent->type = ingest_type(an, scope, pstmt->decl.type);
        ent->check_status = ENT_CHECK_IN_PROGRESS_TYPE_AVAILABLE;
    }

    if (pstmt->decl.value == NULL || pstmt->decl.value->base.kind == PN_UNINIT) {
        report_pnode(true, pstmt, "compile-time variable must have a value");
    }

    if (maybe_type(pstmt->decl.value)) {
        ent->type = TYPE_TYPEID;
    }

    SemaNode* value = check_expr(an, scope, pstmt->decl.value, ent->type);
    if (value->kind != SN_CONSTVAL) {
        report_pnode(true, pstmt->decl.value, "value must be compile-time constant");
    }
    if (ent->type != TYPE_UNKNOWN) {
        if (type_can_implicit_cast(value->type, ent->type)) {
            value = insert_implicit_cast(an, value, ent->type);
            assert(value->kind == SN_CONSTVAL);
        } else {
            string valuetype_str = type_gen_string(value->type, true);
            string decltype_str = type_gen_string(ent->type, true);
            report_pnode(true, value->pnode, "cannot coerce from '"str_fmt"' to '"str_fmt"'", 
                str_arg(valuetype_str),
                str_arg(decltype_str)
            );
        }
    }

    assert((value->type == TYPE_TYPEID) == (value->constval.type == TYPE_TYPEID));


    SemaNode* decl = new_node(an, pstmt, SN_DEF_DECL);
    decl->decl.entity = ent;
    decl->decl.value = value;

    ent->decl = decl;
    ent->constval = value->constval;
    ent->check_status = ENT_CHECK_COMPLETE;
    ent->type = value->type;

    resolve_temp_aliases(ent, value->type == TYPE_TYPEID);

    // UNREACHABLE;

    return decl;
}

SemaNode* check_var_decl(Analyzer* an, EntityTable* scope, PNode* pstmt) {

    if (pstmt->decl.kind == DECLKIND_DEF) {
        return check_def_decl(an, scope, pstmt);
    }

    assert(pstmt->decl.ident->base.kind == PN_IDENT); // no lists of decls yet!
    string name = pnode_span(pstmt->decl.ident);

    bool is_global_decl = scope == an->m->global;

    Entity* ent = etbl_search(scope, name);
    if (is_global_decl) {
        // this is guaranteed to exist    
        assert(ent);

        if (ent->check_status == ENT_CHECK_COMPLETE && ent->decl_pnode == pstmt) {
            return ent->decl;
        }
    }

    if ((is_global_decl && ent->decl_pnode != pstmt) || (scope != an->m->global && ent != NULL)) {
        report_pnode(true, pstmt->decl.ident, "name already declared");
    } else if (!is_global_decl) {
        ent = etbl_put(scope, name);
    }

    ent->check_status = ENT_CHECK_IN_PROGRESS;
    ent->mutable = pstmt->decl.kind == DECLKIND_MUT;
    ent->decl_pnode = pstmt;
    ent->type = TYPE_UNKNOWN;
    if (is_global_decl) {
        ent->storage = STORAGE_GLOBAL;
    } else {
        ent->storage = STORAGE_LOCAL;
    }

    if (pstmt->decl.type) {
        ent->type = ingest_type(an, scope, pstmt->decl.type);
        ent->check_status = ENT_CHECK_IN_PROGRESS_TYPE_AVAILABLE;
    }

    u8 decl_kind = pstmt->decl.kind;
    SemaNode* decl = new_node(an, pstmt, SN_VAR_DECL);
    decl->decl.entity = ent;
    ent->decl = decl;

    // uninitalized value
    if (pstmt->decl.value == NULL) {
        if (ent->type == TYPE_UNKNOWN) {
            report_pnode(true, decl->pnode, "uninitialized variable must have a type");
        }
        if (pstmt->decl.kind == DECLKIND_LET) {
            report_pnode(true, pstmt, "immutable variable must be initialized");
        }
        ent->uninit = (pstmt->decl.value != NULL && pstmt->decl.value->base.kind == PN_UNINIT);
    } else if (pstmt->decl.value != NULL) {
        SemaNode* value = check_expr(an, scope, pstmt->decl.value, ent->type);
        decl->decl.value = value;

        resolve_temp_aliases(ent, false);

        if (ent->type == TYPE_UNKNOWN) {
            ent->type = normalize_untyped(an->m, value->type);
            value = insert_implicit_cast(an, value, ent->type);
        } else if (type_can_implicit_cast(value->type, ent->type)) {
            value = insert_implicit_cast(an, value, ent->type);
        } else {
            string valuetype_str = type_gen_string(value->type, true);
            string decltype_str = type_gen_string(ent->type, true);
            report_pnode(true, value->pnode, "cannot coerce from '"str_fmt"' to '"str_fmt"'", 
                str_arg(valuetype_str),
                str_arg(decltype_str)
            );
        }

        // only certain things are allowed in global decls
        if (is_global_decl && !can_global_decl_value(value)) {
            report_pnode(true, value->pnode, "expression is not constant at load-time");
        }
    } else if (ent->type == TYPE_UNKNOWN) {
        report_pnode(true, decl->pnode, "declaration must have a type, a value, or both");
    }

    ent->check_status = ENT_CHECK_COMPLETE;
    decl->type = TYPE_VOID;
    return decl;
}

SemaNode* check_block(Analyzer* an, EntityTable* scope, PNode* pbody, bool must_return) {
    // VecPtr(SemaNode) stmts = vecptr_new(SemaNode, 8);
    SemaNode* list = new_node(an, pbody, SN_STMT_BLOCK);
    vec_init(&list->list, 8);

    bool seen_return = false;
    bool warned_dead_code = false;
    for_n (i, 0, pbody->list.len) {
        PNode* pstmt = pbody->list.at[i];
        if (seen_return && !warned_dead_code) {
            report_pnode(false, pstmt, "dead code");
            warned_dead_code = true;
        }
        SemaNode* stmt = check_stmt(an, scope, pstmt);
        if (pstmt->base.kind == PN_STMT_RETURN) {
            seen_return = true;
        }
        vec_append(&list->list, stmt);
    }
    if (!seen_return && must_return) {
        PNode* pstmt = pbody;
        if (pbody->list.len != 0) {
            pstmt = pbody->list.at[pbody->list.len - 1];
        }
        report_pnode(true, pstmt, "function with value must return explicitly");
    }
    return list;
}

SemaNode* check_fn_body(Analyzer* an, EntityTable* scope, PNode* pbody, Type ret_type) {
    return check_block(an, scope, pbody, ret_type != TYPE_VOID);
}

Vec_typedef(TypeFnParam);

SemaNode* check_fn_decl(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    PNode* proto = pstmt->fn_decl.proto;
    PNode* body = pstmt->fn_decl.stmts;

    PNode* ident = proto->fnproto.ident;
    string name = pnode_span(ident);

    bool is_global_decl = scope == an->m->global;

    SemaNode* decl = new_node(an, pstmt, SN_FN_DECL);
    Entity* ent = etbl_search(scope, name);
    if (is_global_decl) {
        // this is guaranteed to exist    
        assert(ent);
        if (ent->decl_pnode == pstmt) {
            if (ent->check_status == ENT_CHECK_COMPLETE) {
                return ent->decl;
            }
        } else {
            report_pnode(true, ident, "symbol already declared");
        }
    } else if (ent) {
        report_pnode(true, ident, "symbol already declared");
    }

    if (!ent) {
        ent = etbl_put(scope, name);
    }

    ent->check_status = ENT_CHECK_IN_PROGRESS;
    ent->storage = STORAGE_FUNCTION;

    // new scope for the function scope
    EntityTable* fn_scope = etbl_new(an->m->global); // because you cant capture local variables 

    // scan prototype and create a function type
    Vec(TypeFnParam) params = vec_new(TypeFnParam, 8);
    PNode* param_list = proto->fnproto.param_list;

    // TODO extract into general param_list ingest
    for_n (i, 0, param_list->list.len) {
        PNode* items = param_list->list.at[i];
        
        Type t = ingest_type(an, fn_scope, items->item.type);

        if (items->item.ident->base.kind == PN_IDENT) {
            string name = pnode_span(items->item.ident);
            if (etbl_search(fn_scope, name)) {
                report_pnode(true, items->item.ident, "symbol already declared");
            }
            // warn if the parameter name is the same as a local name
            if (etbl_search(scope, name)) {
                report_pnode(false, items->item.ident->list.at[i], "parameter does not capture local; consider renaming");
            }
            TypeFnParam p = { .name = name, .type = t };
            vec_append(&params, p);
            
            Entity* p_ent = etbl_put(fn_scope, name);
            p_ent->storage = STORAGE_PARAMETER;
            p_ent->type = t;
            p_ent->check_status = ENT_CHECK_COMPLETE;
        } else {
            for_n(i, 0, items->item.ident->list.len) {
                string name = pnode_span(items->item.ident->list.at[i]);
                if (etbl_search(fn_scope, name)) {
                    report_pnode(true, items->item.ident->list.at[i], "symbol already declared");
                }
                // warn if the parameter name is the same as a local name
                if (etbl_search(scope, name)) {
                    report_pnode(false, items->item.ident->list.at[i], "parameter does not capture local; consider renaming");
                }
                TypeFnParam p = { .name = name, .type = t };
                vec_append(&params, p);

                Entity* p_ent = etbl_put(fn_scope, name);
                p_ent->storage = STORAGE_PARAMETER;
                p_ent->type = t;
                p_ent->check_status = ENT_CHECK_COMPLETE;
            }
        }
    }

    Type ret_type;
    if (proto->fnproto.ret_type) {
        ret_type = ingest_type(an, fn_scope, proto->fnproto.ret_type);
    } else {
        ret_type = TYPE_VOID;
    }

    Type t = type_new(an->m, TYPE_FUNCTION);
    type(t)->as_function.params.at = params.at;
    type(t)->as_function.params.len = params.len;
    type(t)->as_function.ret_type = ret_type;

    decl->fn_decl.entity = ent;
    decl->fn_decl.scope = fn_scope;

    ent->check_status = ENT_CHECK_COMPLETE;
    ent->decl = decl;
    ent->type = t;

    an->current_fn = ent;
    an->return_type = ret_type;

    decl->fn_decl.body = check_fn_body(an, fn_scope, pstmt->fn_decl.stmts, ret_type);
    
    // ent->check_status = ENT_CHECK_COMPLETE;
    
    return decl;
}

SemaNode* check_stmt_return(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    SemaNode* ret_stmt = new_node(an, pstmt, SN_STMT_RETURN);
    ret_stmt->return_stmt.value = NULL;
    if (an->return_type == TYPE_VOID) {
        if (pstmt->expr_stmt.expr != NULL) {
            report_pnode(true, pstmt, "void function cannot return a value");
        }
    } else {
        if (pstmt->expr_stmt.expr == NULL) {
            string ret_type_str = type_gen_string(an->return_type, true);
            report_pnode(true, pstmt, "function must return '"str_fmt"'", str_arg(ret_type_str));
        }
        SemaNode* ret_expr = check_expr(an, scope, pstmt->expr_stmt.expr, an->return_type);
        if (type_can_implicit_cast(ret_expr->type, an->return_type)) {
            ret_expr = insert_implicit_cast(an, ret_expr, an->return_type);
        } else {
            string ret_type_str = type_gen_string(an->return_type, true);
            string value_type_str = type_gen_string(ret_expr->type, true);
            report_pnode(true, ret_expr->pnode, "cannot coerce from '"str_fmt"' to '"str_fmt"'", 
                str_arg(value_type_str),
                str_arg(ret_type_str)
            );
        }
        ret_stmt->return_stmt.value = ret_expr;
    }
    return ret_stmt;
}

SemaNode* check_stmt_assign(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    SemaNode* assign_stmt = new_node(an, pstmt, SN_STMT_ASSIGN);

    SemaNode* lhs = check_expr(an, scope, pstmt->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(an, scope, pstmt->binop.rhs, lhs->type);
    assign_stmt->assign.lhs = lhs;
    assign_stmt->assign.rhs = rhs;

    if (!assign_stmt->assign.lhs->mutable) {
        report_pnode(true, lhs->pnode, "cannot assign to immutable expression");
    }

    if (type_can_implicit_cast(rhs->type, lhs->type)) {
        lhs = insert_implicit_cast(an, rhs, lhs->type);
    } else {
        string lhs_type_str = type_gen_string(lhs->type, true);
        string rhs_type_str = type_gen_string(rhs->type, true);
        report_pnode(true, rhs->pnode, "cannot coerce from '"str_fmt"' to '"str_fmt"'", 
            str_arg(rhs_type_str),
            str_arg(lhs_type_str)
        );
    }

    return assign_stmt;
}

SemaNode* check_stmt_op_assign(Analyzer* an, EntityTable* scope, PNode* pstmt) {

    SemaNode* op;
    switch (pstmt->base.kind) {
    case PN_STMT_ASSIGN_ADD:
    case PN_STMT_ASSIGN_SUB:
    case PN_STMT_ASSIGN_MUL:
    case PN_STMT_ASSIGN_DIV:
    case PN_STMT_ASSIGN_MOD:
        op = check_expr_arith_binop(an, pstmt->base.kind - PN_STMT_ASSIGN_ADD + PN_EXPR_ADD, scope, pstmt, TYPE_UNKNOWN);
        break;
    case PN_STMT_ASSIGN_BIT_AND:
    case PN_STMT_ASSIGN_BIT_OR:
    case PN_STMT_ASSIGN_XOR:
    op = check_expr_bitwise_binop(an, pstmt->base.kind - PN_STMT_ASSIGN_ADD + PN_EXPR_ADD, scope, pstmt, TYPE_UNKNOWN);
    break;
    default:
        UNREACHABLE;
    }

    SemaNode* assign_stmt = new_node(an, pstmt, SN_STMT_ASSIGN);

    SemaNode* lhs = check_expr(an, scope, pstmt->binop.lhs, TYPE_UNKNOWN);
    assign_stmt->assign.lhs = lhs;
    assign_stmt->assign.rhs = op;

    if (!assign_stmt->assign.lhs->mutable) {
        report_pnode(true, lhs->pnode, "cannot assign to immutable expression");
    }

    if (type_can_implicit_cast(op->type, lhs->type)) {
        lhs = insert_implicit_cast(an, op, lhs->type);
    } else {
        string lhs_type_str = type_gen_string(lhs->type, true);
        string rhs_type_str = type_gen_string(op->type, true);
        report_pnode(true, op->pnode, "cannot coerce from '"str_fmt"' to '"str_fmt"'", 
            str_arg(rhs_type_str),
            str_arg(lhs_type_str)
        );
    }

    return assign_stmt;
}

SemaNode* check_stmt_if(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    SemaNode* if_stmt = new_node(an, pstmt, SN_STMT_IF);
    
    SemaNode* cond = check_expr(an, scope, pstmt->ternary.cond, TYPE_BOOL);
    if (type_can_implicit_cast(cond->type, TYPE_BOOL)) {
        cond = insert_implicit_cast(an, cond, TYPE_BOOL);
    } else {
        string cond_type_str = type_gen_string(cond->type, true);
        report_pnode(true, cond->pnode, "condition must be 'bool', not '"str_fmt"'", 
            str_arg(cond_type_str)
        );
    }
    if_stmt->if_stmt.cond = cond;
    
    EntityTable* if_true_scope = etbl_new(scope);
    if_stmt->if_stmt.if_true = check_stmt(an, if_true_scope, pstmt->ternary.if_true);
    if (pstmt->ternary.if_false) {
        EntityTable* if_false_scope = etbl_new(scope);
        if_stmt->if_stmt.if_false = check_stmt(an, if_false_scope, pstmt->ternary.if_false);
    }
    return if_stmt;
}

SemaNode* check_stmt_while(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    SemaNode* while_loop = new_node(an, pstmt, SN_STMT_WHILE);
    
    SemaNode* cond = check_expr(an, scope, pstmt->binop.lhs, TYPE_BOOL);
    if (type_can_implicit_cast(cond->type, TYPE_BOOL)) {
        cond = insert_implicit_cast(an, cond, TYPE_BOOL);
    } else {
        string cond_type_str = type_gen_string(cond->type, true);
        report_pnode(true, cond->pnode, "condition must be 'bool', not '"str_fmt"'", 
            str_arg(cond_type_str)
        );
    }
    while_loop->while_loop.cond = cond;
    
    EntityTable* body_scope = etbl_new(scope);
    while_loop->while_loop.body = check_stmt(an, body_scope, pstmt->binop.rhs);

    return while_loop;
}

SemaNode* check_stmt_for_in(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    SemaNode* for_loop = new_node(an, pstmt, SN_STMT_FOR_IN);
    string iter_var_ident = pnode_span(pstmt->for_in.ident);

    if (etbl_search(scope, iter_var_ident)) {
        report_pnode(true, pstmt->for_in.ident, "name already declared"); 
    }

    EntityTable* body_scope = etbl_new(scope);
    Entity* iter_var = etbl_put(body_scope, iter_var_ident);
    iter_var->check_status = ENT_CHECK_IN_PROGRESS;
    iter_var->mutable = false;

    u8 iter_kind = pstmt->for_in.range->base.kind;
    if (iter_kind == PN_EXPR_RANGE_EQ || iter_kind == PN_EXPR_RANGE_LESS) {
        if (pstmt->for_in.type) {
            iter_var->type = ingest_type(an, scope, pstmt->for_in.type);
            iter_var->check_status = ENT_CHECK_IN_PROGRESS_TYPE_AVAILABLE;
        }
    
        SemaNode* range = check_expr_range(an, body_scope, pstmt->for_in.range);
    
        iter_var->check_status = ENT_CHECK_COMPLETE;
    
        if (iter_var->type == TYPE_UNKNOWN) {
            iter_var->type = range->type;
        } else {
            if (type_can_implicit_cast(range->type, iter_var->type)) {
                range->binop.lhs = insert_implicit_cast(an, range->binop.lhs, iter_var->type);
                range->binop.rhs = insert_implicit_cast(an, range->binop.rhs, iter_var->type);
            } else {
                string lhs_typestr = type_gen_string(iter_var->type, true);
                string rhs_typestr = type_gen_string(range->type, true);
                report_pnode(true, pstmt, "type mismatch: '"str_fmt"' and '"str_fmt"'", 
                    str_arg(lhs_typestr), str_arg(rhs_typestr));
            }
        }
    
        for_loop->for_range.range = range;
        for_loop->for_range.iter_var = iter_var;
        for_loop->for_range.body = check_stmt(an, body_scope, pstmt->for_in.block);    
    } else {
        UNREACHABLE;
    }

    return for_loop;
}

SemaNode* check_stmt_for_cstyle(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    SemaNode* for_loop = new_node(an, pstmt, SN_STMT_FOR_CSTYLE);

    EntityTable* body_scope = etbl_new(scope);

    for_loop->for_cstyle.init = check_stmt(an, body_scope, pstmt->for_cstyle.init);
    SemaNode* cond = check_expr(an, body_scope, pstmt->for_cstyle.cond, TYPE_BOOL);
    if (type_can_implicit_cast(cond->type, TYPE_BOOL)) {
        cond = insert_implicit_cast(an, cond, TYPE_BOOL);
    } else {
        string cond_type_str = type_gen_string(cond->type, true);
        report_pnode(true, cond->pnode, "condition must be 'bool', not '"str_fmt"'", 
            str_arg(cond_type_str)
        );
    }
    for_loop->for_cstyle.cond = cond;
    for_loop->for_cstyle.post = check_stmt(an, body_scope, pstmt->for_cstyle.post);
    for_loop->for_cstyle.body = check_stmt(an, body_scope, pstmt->for_cstyle.block);

    return for_loop;
}

SemaNode* check_stmt(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    switch (pstmt->base.kind) {
    case PN_LIST:
        return check_block(an, scope, pstmt, false);
    case PN_STMT_DECL:
        return check_var_decl(an, scope, pstmt);
    case PN_STMT_FN_DECL:
        return check_fn_decl(an, scope, pstmt);
    case PN_STMT_RETURN:
        return check_stmt_return(an, scope, pstmt);
    case PN_STMT_ASSIGN:
        return check_stmt_assign(an, scope, pstmt);
    case PN_STMT_ASSIGN_ADD:
    case PN_STMT_ASSIGN_SUB:
    case PN_STMT_ASSIGN_MUL:
    case PN_STMT_ASSIGN_DIV:
    case PN_STMT_ASSIGN_MOD:
    case PN_STMT_ASSIGN_BIT_AND:
    case PN_STMT_ASSIGN_BIT_OR:
    case PN_STMT_ASSIGN_XOR:
        return check_stmt_op_assign(an, scope, pstmt);
    case PN_STMT_IF:
        return check_stmt_if(an, scope, pstmt);
    case PN_STMT_WHILE:
        return check_stmt_while(an, scope, pstmt);
    case PN_STMT_FOR_IN:
        return check_stmt_for_in(an, scope, pstmt);
    case PN_STMT_FOR_CSTYLE:
        return check_stmt_for_cstyle(an, scope, pstmt);
    case PN_DO:
        return check_stmt(an, scope, pstmt->unop.sub);
    case PN_EXPR_CALL:
        return check_expr_call(an, scope, pstmt, TYPE_UNKNOWN);
    default:
        report_pnode(true, pstmt, "expected statement %d", pstmt->base.kind);
        return NULL;
    }
}

Module* sema_check_module(PNode* top) {
    Module* mod = malloc(sizeof(Module));
    *mod = (Module){};
    assert(top->base.kind == PN_LIST);
    da_init(&mod->decls, top->list.len);
    mod->global = etbl_new(NULL);

    PNode* mod_decl = top->list.at[0];
    mod->name = pnode_span(mod_decl->module_decl.ident);

    Analyzer an;
    an.m = mod;
    an.current_fn = NULL;
    an.return_type = TYPE_UNKNOWN;
    an.defer_stack = vecptr_new(SemaNode, 4);

    // pre-collect entitites
    for_n (i, 1, top->list.len) {
        PNode* tls = top->list.at[i];
        switch (tls->base.kind) {
        case PN_STMT_DECL: {
            PNode* ident = tls->decl.ident;
            string ident_name = pnode_span(ident);
            Entity* ent = etbl_put(mod->global, ident_name);
            ent->check_status = ENT_CHECK_NONE;
            ent->decl_pnode = tls;
            ent->tbl = mod->global;
            } break;
        case PN_STMT_FN_DECL: {
            PNode* prototype = tls->fn_decl.proto;
            PNode* ident = prototype->fnproto.ident;
            string ident_name = pnode_span(ident);
            Entity* ent = etbl_put(mod->global, ident_name);
            ent->check_status = ENT_CHECK_NONE;
            ent->decl_pnode = tls;
            ent->tbl = mod->global;
            } break;
        case PN_STMT_EXTERN_DECL: {
            PNode* decl = tls->unop.sub;
            switch (decl->base.kind) {
            case PN_FNPROTO:
                PNode* ident = decl->fnproto.ident;
                string ident_name = pnode_span(ident);
                Entity* ent = etbl_put(mod->global, ident_name);
                ent->check_status = ENT_CHECK_NONE;
                ent->storage = STORAGE_EXTERN_FUNCTION;
                ent->tbl = mod->global;
                break;
            default:
                UNREACHABLE;
            }
            } break;
        default:
            UNREACHABLE;
        }
    }

    // check stmts
    for_n (i, 1, top->list.len) {
        PNode* tls = top->list.at[i];
        SemaNode* decl = check_stmt(&an, mod->global, tls);
        da_append(&mod->decls, decl);
    }

    return mod;
}