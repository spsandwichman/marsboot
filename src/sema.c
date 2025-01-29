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
    EntityTable* etbl = malloc(sizeof(*etbl));
    memset(etbl, 0, sizeof(*etbl));
    etbl->parent = parent;
    da_init(etbl, 32);
    return etbl;
}

Entity* etbl_search(EntityTable* etbl, string name) {
    if (etbl == NULL) return NULL;
    foreach (Entity* e, *etbl) {
        if (string_eq(e->name, name)) {
            return e;
        }
    }
    return etbl_search(etbl->parent, name);
}

Entity* etbl_put(EntityTable* etbl, string name) {
    Entity* ent = malloc(sizeof(*ent));
    memset(ent, 0, sizeof(*ent));
    ent->name = name;
    da_append(etbl, ent);
    return ent;
}

SemaNode* new_node(Module* m, PNode* pn, u8 kind) {
    SemaNode* node = malloc(sizeof(*node));
    memset(node, 0, sizeof(*node));
    node->kind = kind;
    node->pnode = pn;
    return node;
}

static u64 eval_integer(PNode* pn, char* raw, usize len) {
    if (len == 1) {
        return raw[0] - '0';
    }
    isize value = 0;
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
            cval = c - 'a';
        } else if ('A' <= c && c <= 'F') {
            cval = c - 'A';
        }
        if (cval >= base) {
            report_pnode(true, pn, "'%c' is not a valid base-%d digit", c, base);
        }
        // TODO add overflow check here
        value = value * base + cval;
    }
    return value;
}

SemaNode* check_expr_integer(Module* m, EntityTable* etbl, PNode* pn) {
    // get integer value
    i64 value = eval_integer(pn, pn->base.raw, pn->base.len);
    SemaNode* integer = new_node(m, pn, SN_CONSTVAL);
    integer->constval = (ConstVal){
        .i64 = value,
        .type = TYPE_UNTYPED_INT,
    };
    integer->type = TYPE_UNTYPED_INT;
    return integer;
}

bool type_is_intlike(Type t) {
    return type_is_integer(t) 
        || type(t)->kind == TYPE_POINTER 
        || type(t)->kind == TYPE_BOUNDLESS_SLICE;
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

// +, -, *, /, %
void const_eval_expr_arith_binop(Module* m, SemaNode* n, u8 kind) {
    SemaNode* lhs = n->binop.lhs;
    SemaNode* rhs = n->binop.rhs;

    n->kind = SN_CONSTVAL;
    n->constval.type = n->type;

    switch (kind) {
    case PN_EXPR_ADD: apply_binop(n->constval, n->type, lhs->constval, +, rhs->constval); break;
    case PN_EXPR_SUB: apply_binop(n->constval, n->type, lhs->constval, +, rhs->constval); break;
    case PN_EXPR_MUL: apply_binop(n->constval, n->type, lhs->constval, *, rhs->constval); break;
    case PN_EXPR_DIV: apply_binop(n->constval, n->type, lhs->constval, /, rhs->constval); break;
    case PN_EXPR_MOD: apply_binop_integer(n->constval, n->type, lhs->constval, %, rhs->constval); break;
    default:
        UNREACHABLE;
    }
}

SemaNode* insert_implicit_cast(Module* m, SemaNode* n, Type to) {
    // if its a constant val, we can just change reassign the type
    // TODO there might be more conversion to do here,
    // probably separate that out into a different function
    if (n->kind == SN_CONSTVAL) {
        n->type = to;
        n->constval.type = to;
        return n;
    }

    SemaNode* impl_cast = new_node(m, NULL, SN_IMPLICIT_CAST);
    impl_cast->cast.sub = n;
    impl_cast->cast.to = to;
    return impl_cast;
}

// +, -, *, /, %
SemaNode* check_expr_arith_binop(Module* m, u8 kind, EntityTable* etbl, PNode* pn, Type expected) {
    
    SemaNode* lhs = check_expr(m, etbl, pn->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(m, etbl, pn->binop.rhs, TYPE_UNKNOWN);
    SemaNode* binop = new_node(m, pn, SN_BINOP);
    binop->type = TYPE_UNKNOWN;
    binop->binop.lhs = lhs;
    binop->binop.rhs = rhs;
    
    if (!type_is_numeric(lhs->type)) {
        report_pnode(true, lhs->pnode, "expression type is not numeric");
    }
    if (!type_is_numeric(rhs->type)) {
        report_pnode(true, rhs->pnode, "expression type is not numeric");
    }

    if (!type_compare(lhs->type, rhs->type, false, false)) {
        if (type_can_implicit_cast(lhs->type, rhs->type)) {
            lhs = insert_implicit_cast(m, lhs, rhs->type);
            binop->type = rhs->type;
        } else if (type_can_implicit_cast(rhs->type, lhs->type)) {
            rhs = insert_implicit_cast(m, rhs, lhs->type);
            binop->type = lhs->type;
        }
    } else {
        binop->type = rhs->type;
    }

    if (binop->type == TYPE_UNKNOWN) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "operation undefined for '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
    }

    if (lhs->kind == SN_CONSTVAL && rhs->kind == SN_CONSTVAL) {
        const_eval_expr_arith_binop(m, binop, kind);
    }

    return binop;
}

SemaNode* check_expr_ident(Module* m, EntityTable* etbl, PNode* pn) {
    string ident = pnode_span(pn);
    Entity* ent = etbl_search(etbl, ident);
    if (ent == NULL) {
        report_pnode(true, pn, "undefined symbol '"str_fmt"'", str_arg(ident));
    }
    // this entitiy is global, but hasnt been checked yet. lets go do that
    if (ent->check_status == CHK_NONE) {
        SemaNode* decl = check_var_decl(m, ent->tbl, ent->decl_pnode);
    } else if (ent->check_status == CHK_IN_PROGRESS || 
               ent->check_status == CHK_IN_PROGRESS_TYPE_AVAILABLE) {
        report_pnode(true, pn, "cannot define '"str_fmt"' in terms of itself", str_arg(ident));
    }

    // if storage is comptime, return the value as a SN_CONSTVAL
    // else, return a SN_ENTITY reference to the variable
    if (ent->storage == STORAGE_COMPTIME) {
        SemaNode* cv = new_node(m, pn, SN_CONSTVAL);
        cv->constval = ent->constval;
        cv->type = cv->constval.type;
        return cv;
    } else {
        SemaNode* sn_entity = new_node(m, pn, SN_ENTITY);
        sn_entity->entity = ent;
        sn_entity->type = ent->type;
        return sn_entity;
    }
    UNREACHABLE;
}

SemaNode* check_expr(Module* m, EntityTable* etbl, PNode* pn, Type expected) {
    if (expected == TYPE_DYN) {
        TODO("insert dyn boxing");
    }

    SemaNode* expr = NULL;
    switch (pn->base.kind) {
    case PN_IDENT:
        expr = check_expr_ident(m, etbl, pn);
        break;
    case PN_EXPR_INTEGER:
        expr = check_expr_integer(m, etbl, pn);
        break;
    case PN_TYPE_I8 ... PN_TYPE_TYPEID:
    case PN_TYPE_STRUCT:
    case PN_TYPE_UNION:
    case PN_TYPE_ENUM:
    case PN_TYPE_ARRAY:
    case PN_TYPE_DISTINCT:
    case PN_TYPE_POINTER:
    case PN_TYPE_BOUNDLESS_SLICE:
        Type t = ingest_type(m, etbl, pn);
        TODO("");
    case PN_EXPR_ADD:
    case PN_EXPR_SUB:
    case PN_EXPR_MUL:
    case PN_EXPR_DIV:
        expr = check_expr_arith_binop(m, pn->base.kind, etbl, pn, expected);
        break;
    default:
        report_pnode(true, pn, "expected valid expression");
    }
    // string typename = type_gen_string(expr->type, true);
    // report_pnode(false, pn, "type: "str_fmt" value: %d", str_arg(typename), expr->constval.i64);
    return expr;
}

static Type ingest_type_internal(Module* m, EntityTable* etbl, PNode* pn) {
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
    }
    TODO("ingest complex type");
    return TYPE_UNKNOWN;
}

static Type normalize_untyped(Module* m, Type t) {

    switch (t) {
    case TYPE_UNTYPED_FLOAT: 
        return TYPE_F64;
    case TYPE_UNTYPED_INT: 
        return TYPE_I64;
    }
    return t;
}

Type ingest_type(Module* m, EntityTable* etbl, PNode* pn) {
    Type t = ingest_type_internal(m, etbl, pn);
    return t;
}

static bool can_global_decl_value(SemaNode* n) {
    if (n->kind == SN_CONSTVAL) return true;
    return false;
}

SemaNode* check_var_decl(Module* m, EntityTable* etbl, PNode* pstmt) {
    assert(pstmt->decl.ident->base.kind == PN_IDENT); // no lists of decls yet!
    string name = pnode_span(pstmt->decl.ident);

    bool is_global_decl = etbl == m->global;

    if (is_global_decl) {
        // this is guaranteed to exist
        Entity* ent = etbl_search(etbl, name);
        assert(ent);

        if (ent->check_status == CHK_DONE && ent->decl_pnode == pstmt) {
            return ent->decl;
        }
    }

    Entity* ent = etbl_search(etbl, name);
    if ((is_global_decl && ent->decl_pnode != pstmt) || (etbl != m->global && ent != NULL)) {
        report_pnode(true, pstmt->decl.ident, "name already declared");
    } else if (!is_global_decl) {
        ent = etbl_put(etbl, name);
    }

    ent->check_status = CHK_IN_PROGRESS;
    ent->mutable = pstmt->decl.kind == DECLKIND_MUT;
    ent->decl_pnode = pstmt;
    ent->type = TYPE_UNKNOWN;
    switch (pstmt->decl.kind) {
    case DECLKIND_DEF: ent->storage = STORAGE_COMPTIME; break;
    case DECLKIND_LET:
    case DECLKIND_MUT:
        if (is_global_decl) {
            ent->storage = STORAGE_GLOBAL;
        } else {
            ent->storage = STORAGE_LOCAL;
        }
        break;
    }

    if (pstmt->decl.type) {
        ent->type = ingest_type(m, etbl, pstmt->decl.type);
        ent->check_status = CHK_IN_PROGRESS_TYPE_AVAILABLE;
    }

    u8 decl_kind = pstmt->decl.kind;
    SemaNode* decl = new_node(m, pstmt, SN_DECL);
    decl->decl.entity = ent;
    ent->decl = decl;

    SemaNode* value = check_expr(m, etbl, pstmt->decl.value, ent->type);
    decl->decl.value = value;

    if (ent->type == TYPE_UNKNOWN) {
        ent->type = value->type;
        if (ent->storage != STORAGE_COMPTIME) {
            ent->type = normalize_untyped(m, ent->type);
        }
    } else if (type_can_implicit_cast(value->type, ent->type)) {
        value = insert_implicit_cast(m, value, ent->type);
    } else {
        string valuetype_str = type_gen_string(value->type, true);
        string decltype_str = type_gen_string(ent->type, true);
        report_pnode(true, value->pnode, "cannot implicitly cast from '"str_fmt"' to '"str_fmt"'", 
            str_arg(valuetype_str),
            str_arg(decltype_str)
        );
    }

    // only certain things are allowed in global decls
    if (is_global_decl && !can_global_decl_value(value)) {
        report_pnode(true, value->pnode, "expression is not constant at load-time");
    }
    // if it is constant, we need to store the constant in the entity as well
    if (value->kind == SN_CONSTVAL) {
        ent->constval = value->constval;
    }
    if (ent->storage == STORAGE_COMPTIME && value->kind != SN_CONSTVAL) {
        report_pnode(true, value->pnode, "def expression must be a compile-time constant");
    }
    
    ent->check_status = CHK_DONE;
    decl->type = TYPE_VOID;
    return decl;
}

SemaNode* check_stmt(Module* m, EntityTable* etbl, PNode* pstmt) {
    switch (pstmt->base.kind) {
    case PN_STMT_DECL:
        return check_var_decl(m, etbl, pstmt);
    default:
        report_pnode(true, pstmt, "expected statement");
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
    // printf("starting check at head module '"str_fmt"'\n", str_arg(mod->name));

    // pre-collect entitites
    for_n (i, 1, top->list.len) {
        PNode* tls = top->list.at[i];
        switch (tls->base.kind) {
        case PN_STMT_DECL:
            PNode* ident = tls->decl.ident;
            string ident_name = pnode_span(ident);
            Entity* ent = etbl_put(mod->global, ident_name);
            ent->check_status = CHK_NONE;
            ent->decl_pnode = tls;
            ent->tbl = mod->global;
            break;
        default:
            UNREACHABLE;
        }
    }

    // check stmts
    for_n (i, 1, top->list.len) {
        PNode* tls = top->list.at[i];
        SemaNode* decl = check_stmt(mod, mod->global, tls);
        da_append(&mod->decls, decl);
    }

    return mod;
}