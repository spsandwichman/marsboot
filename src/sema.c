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

SemaNode* check_expr_integer(Analyzer* an, EntityTable* scope, PNode* pn) {
    // get integer value
    i64 value = eval_integer(pn, pn->base.raw, pn->base.len);
    SemaNode* integer = new_node(an->m, pn, SN_CONSTVAL);
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

SemaNode* insert_implicit_cast(Analyzer* an, SemaNode* n, Type to) {
    // if its a constant val, we can just change reassign the type
    // TODO there might be more conversion to do here,
    // probably separate that out into a different function
    if (n->kind == SN_CONSTVAL) {
        n->type = to;
        n->constval.type = to;
        return n;
    }

    SemaNode* impl_cast = new_node(an, NULL, SN_IMPLICIT_CAST);
    impl_cast->cast.sub = n;
    impl_cast->cast.to = to;
    return impl_cast;
}

// +, -, *, /, %
SemaNode* check_expr_arith_binop(Module* m, u8 kind, EntityTable* scope, PNode* pn, Type expected) {
    
    SemaNode* lhs = check_expr(m, scope, pn->binop.lhs, TYPE_UNKNOWN);
    SemaNode* rhs = check_expr(m, scope, pn->binop.rhs, TYPE_UNKNOWN);
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

    if (kind == PN_EXPR_MOD && (type_is_float(lhs->type) || type_is_float(rhs->type))) {
        string lhs_typestr = type_gen_string(lhs->type, true);
        string rhs_typestr = type_gen_string(rhs->type, true);
        report_pnode(true, pn, "% undefined for '"str_fmt"' and '"str_fmt"'", 
            str_arg(lhs_typestr), str_arg(rhs_typestr));
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

SemaNode* check_expr_ident(Module* m, EntityTable* scope, PNode* pn) {
    string ident = pnode_span(pn);
    Entity* ent = etbl_search(scope, ident);
    if (ent == NULL) {
        report_pnode(true, pn, "undefined symbol '"str_fmt"'", str_arg(ident));
    }
    // this entitiy is global, but hasnt been checked yet. lets go do that
    if (ent->check_status == ENT_CHECK_NONE) {
        SemaNode* decl = check_var_decl(m, ent->tbl, ent->decl_pnode);
    } else if (ent->check_status == ENT_CHECK_IN_PROGRESS || 
               ent->check_status == ENT_CHECK_IN_PROGRESS_TYPE_AVAILABLE) {
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

SemaNode* check_expr(Analyzer* an, EntityTable* scope, PNode* pn, Type expected) {
    if (expected == TYPE_DYN) {
        TODO("insert dyn boxing");
    }

    SemaNode* expr = NULL;
    switch (pn->base.kind) {
    case PN_IDENT:
        expr = check_expr_ident(an, scope, pn);
        break;
    case PN_EXPR_INTEGER:
        expr = check_expr_integer(an, scope, pn);
        break;
    case PN_TYPE_I8 ... PN_TYPE_TYPEID:
    case PN_TYPE_STRUCT:
    case PN_TYPE_UNION:
    case PN_TYPE_ENUM:
    case PN_TYPE_ARRAY:
    case PN_TYPE_DISTINCT:
    case PN_TYPE_POINTER:
    case PN_TYPE_BOUNDLESS_SLICE:
        Type t = ingest_type(an, scope, pn);
        expr = new_node(an, pn, SN_CONSTVAL);
        expr->type = TYPE_TYPEID;
        expr->constval.typeid = t;
        break;
    case PN_EXPR_ADD:
    case PN_EXPR_SUB:
    case PN_EXPR_MUL:
    case PN_EXPR_DIV:
        expr = check_expr_arith_binop(an, pn->base.kind, scope, pn, expected);
        break;
    default:
        report_pnode(true, pn, "expected valid expression");
    }
    // string typename = type_gen_string(expr->type, true);
    // report_pnode(false, pn, "type: "str_fmt" value: %d", str_arg(typename), expr->constval.i64);
    return expr;
}

static Type ingest_type_internal(Analyzer* an, EntityTable* scope, PNode* pn) {
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
        Type sub = ingest_type_internal(an, scope, pn->array_type.sub);
        Type arr = type_new(an->m, TYPE_ARRAY);
        type(arr)->as_array.len = length->constval.i64;
        type(arr)->as_array.sub = sub;
        return arr;
    case PN_TYPE_POINTER:
        Type pointee = ingest_type_internal(an, scope, pn->ref_type.sub);
        Type ptr = type_new_ref(an->m, TYPE_POINTER, pointee, pn->ref_type.mutable);
        return ptr;
    case PN_TYPE_BOUNDLESS_SLICE:
        pointee = ingest_type_internal(an, scope, pn->ref_type.sub);
        ptr = type_new_ref(an->m, TYPE_BOUNDLESS_SLICE, pointee, pn->ref_type.mutable);
        return ptr;
    case PN_TYPE_SLICE:
        pointee = ingest_type_internal(an, scope, pn->ref_type.sub);
        ptr = type_new_ref(an->m, TYPE_SLICE, pointee, pn->ref_type.mutable);
        return ptr;

    case PN_IDENT:
        SemaNode* type = check_expr_ident(an, scope, pn);
        if (type->type != TYPE_TYPEID || type->kind != SN_CONSTVAL) {
            report_pnode(true, pn, "symbol is not a compile-time constant typeid");
        }
        return type->constval.typeid;
    default:
        UNREACHABLE;
    }
    // TODO("ingest complex type");
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

Type ingest_type(Analyzer* an, EntityTable* scope, PNode* pn) {
    Type t = ingest_type_internal(an, scope, pn);
    return t;
}

static bool can_global_decl_value(SemaNode* n) {
    if (n->kind == SN_CONSTVAL) return true;
    return false;
}

SemaNode* check_var_decl(Analyzer* an, EntityTable* scope, PNode* pstmt) {
        
    assert(pstmt->decl.ident->base.kind == PN_IDENT); // no lists of decls yet!
    string name = pnode_span(pstmt->decl.ident);

    bool is_global_decl = scope == an->m->global;

    Entity* ent = etbl_search(scope, name);
    if (is_global_decl) {
        // this is guaranteed to exist    
        assert(ent);

        if (ent->check_status == ENT_CHECK_DONE && ent->decl_pnode == pstmt) {
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
        ent->type = ingest_type(an, scope, pstmt->decl.type);
        ent->check_status = ENT_CHECK_IN_PROGRESS_TYPE_AVAILABLE;
    }

    u8 decl_kind = pstmt->decl.kind;
    SemaNode* decl = new_node(an, pstmt, SN_VAR_DECL);
    decl->decl.entity = ent;
    ent->decl = decl;

    // uninitalized value
    if (pstmt->decl.value == NULL || pstmt->decl.value->base.kind == PN_UNINIT) {
        if (ent->type == TYPE_UNKNOWN) {
            report_pnode(true, decl->pnode, "uninitialized variable must have a type");
        }
        if (pstmt->decl.kind != DECLKIND_MUT) {
            report_pnode(true, pstmt, "immutable variable must be initialized");
        }
        ent->uninit = pstmt->decl.value != NULL && pstmt->decl.value->base.kind == PN_UNINIT;
    } else if (pstmt->decl.value != NULL) {
        SemaNode* value = check_expr(an, scope, pstmt->decl.value, ent->type);
        decl->decl.value = value;

        if (ent->storage == STORAGE_COMPTIME && value->kind == SN_CONSTVAL && value->type == TYPE_TYPEID) {
            
            // this is actually a type declaration!!
            ent->type = TYPE_TYPEID;
            Type alias = type_create_alias(an->m, value->constval.typeid);
            type_attach_name(alias, name);
            value->constval.typeid = alias;
        }

        if (ent->type == TYPE_UNKNOWN) {
            ent->type = normalize_untyped(an->m, value->type);
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
        // if it is constant, we need to store the constant in the entity as well
        if (value->kind == SN_CONSTVAL) {
            ent->constval = value->constval;
        }
        if (ent->storage == STORAGE_COMPTIME && value->kind != SN_CONSTVAL) {
            report_pnode(true, value->pnode, "def expression must be a compile-time constant");
        }
    } else if (ent->type == TYPE_UNKNOWN) {
        report_pnode(true, decl->pnode, "declaration must have a type, a value, or both");
    }

    ent->check_status = ENT_CHECK_DONE;
    decl->type = TYPE_VOID;
    return decl;
}

SemaNode* check_block(Analyzer* an, EntityTable* scope, PNode* pbody, bool must_return) {
    VecPtr(SemaNode) stmts = vecptr_new(SemaNode, 8);

    bool seen_return = false;
    for_n (i, 0, pbody->list.len) {
        PNode* pstmt = pbody->list.at[i];
        SemaNode* stmt = check_stmt(an, scope, pstmt);
        if (pstmt->base.kind == PN_STMT_RETURN) {
            seen_return = true;
        }
        vec_append(&stmts, stmt);
    }
}

SemaNode* check_fn_body(Analyzer* an, EntityTable* scope, PNode* pbody, Type ret_type) {
    return check_block(an, scope, pbody, true);
}

Vec_typedef(TypeFnParam);

SemaNode* check_fn_decl(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    PNode* proto = pstmt->fn_decl.proto;
    PNode* body = pstmt->fn_decl.stmts;

    PNode* ident = proto->fnproto.ident;
    string name = pnode_span(ident);

    bool is_global_decl = scope == an->m->global;


    SemaNode* decl = new_node(an->m, pstmt, SN_FN_DECL);
    Entity* ent = etbl_search(scope, name);
    if (is_global_decl) {
        // this is guaranteed to exist    
        assert(ent);
        if (ent->decl_pnode == pstmt) {
            if (ent->check_status == ENT_CHECK_DONE) {
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
        
        Type t = ingest_type(an->m, fn_scope, items->item.type);

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
            p_ent->check_status = ENT_CHECK_DONE;
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
                p_ent->check_status = ENT_CHECK_DONE;
            }
        }
    }

    Type ret_type;
    if (proto->fnproto.ret_type) {
        ret_type = ingest_type(an->m, fn_scope, proto->fnproto.ret_type);
    } else {
        ret_type = TYPE_VOID;
    }

    Type t = type_new(an->m, TYPE_FUNCTION);
    type(t)->as_function.params.at = params.at;
    type(t)->as_function.params.len = params.len;
    type(t)->as_function.ret_type = ret_type;

    decl->fn_decl.entity = ent;
    decl->fn_decl.scope = fn_scope;

    ent->check_status = ENT_CHECK_IN_PROGRESS_TYPE_AVAILABLE;
    ent->decl = decl;
    ent->type = t;

    decl->fn_decl.body = check_fn_body(an->m, fn_scope, pstmt->fn_decl.stmts, ret_type);
    
    ent->check_status = ENT_CHECK_DONE;
    
    return decl;
}

SemaNode* check_stmt(Analyzer* an, EntityTable* scope, PNode* pstmt) {
    switch (pstmt->base.kind) {
    case PN_STMT_DECL:
        return check_var_decl(an, scope, pstmt);
    case PN_STMT_FN_DECL:
        return check_fn_decl(an, scope, pstmt);
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

    Analyzer an;
    an.m = mod;
    an.current_fn = NULL;
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
            PNode* body = tls->fn_decl.stmts;
            PNode* ident = prototype->fnproto.ident;
            string ident_name = pnode_span(ident);
            Entity* ent = etbl_put(mod->global, ident_name);
            ent->check_status = ENT_CHECK_NONE;
            ent->decl_pnode = tls;
            ent->tbl = mod->global;
            } break;
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