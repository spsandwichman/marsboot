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

SemaNode* check_expr_integer(Module* m, EntityTable* etbl, PNode* pn, Type expected) {
    // get integer value
    i64 value = eval_integer(pn, pn->base.raw, pn->base.len);

    SemaNode* integer = new_node(m, pn, SN_CONSTVAL);

    if (expected == TYPE_UNKNOWN) {
        expected = TYPE_UNTYPED_INT;
    }

    integer->constval = (ConstVal){
        .i64 = value,
        .type = expected,
    };
    integer->type = expected;

    return integer;
}

SemaNode* check_expr(Module* m, EntityTable* etbl, PNode* pn, Type expected) {
    switch (pn->base.kind) {
    case PN_EXPR_INTEGER:
        return check_expr_integer(m, etbl, pn, expected);
    case PN_EXPR_ADD:
        // return check_expr_binary_op(m, etbl, pn, expected);
    default:
        report_pnode(true, pn, "expected valid expression");
    }
    TODO("");
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
    static Type default_string_type = 0;
    if (default_string_type == 0) {
        default_string_type = type_new_ref(m, TYPE_SLICE, TYPE_U8, false);
    }

    switch (t) {
    case TYPE_UNTYPED_FLOAT: return TYPE_F64;
    case TYPE_UNTYPED_INT: return TYPE_I64;
    case TYPE_UNTYPED_STRING: return default_string_type;
    }
    return t;
}

Type ingest_type(Module* m, EntityTable* etbl, PNode* pn) {
    Type t = ingest_type_internal(m, etbl, pn);
    type_condense();
    return t;
}

SemaNode* check_var_decl(Module* m, EntityTable* etbl, PNode* pstmt) {
    assert(pstmt->decl.ident->base.kind == PN_IDENT); // no lists of decls yet!
    string name = pnode_span(pstmt->decl.ident);

    Entity* ent = etbl_search(etbl, name);
    if ((etbl == m->global && ent->decl_pnode != pstmt) || (etbl != m->global && ent != NULL)) {
        report_pnode(true, pstmt->decl.ident, "name already declared");
    } else {
        ent = etbl_put(etbl, name);
    }
    ent->check_status = CHK_IN_PROGRESS;
    ent->mutable = pstmt->decl.kind == DECLKIND_MUT;
    ent->decl_pnode = pstmt;
    ent->type = TYPE_UNKNOWN;

    if (pstmt->decl.type) {
        ent->type = ingest_type(m, etbl, pstmt->decl.type);
        ent->check_status = CHK_IN_PROGRESS_TYPE_AVAILABLE;
    }

    u8 decl_kind = pstmt->decl.kind;
    SemaNode* stmt = new_node(m, pstmt, SN_DECL);

    stmt->decl.entity = ent;
    stmt->decl.value = check_expr(m, etbl, pstmt->decl.value, ent->type);
    ent->type = stmt->decl.value->type;
    ent->check_status = CHK_DONE;
    stmt->type = TYPE_VOID;

    return stmt;
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

Module sema_check_module(PNode* top) {
    Module mod;
    assert(top->base.kind == PN_LIST);
    da_init(&mod.decls, top->list.len);
    mod.global = etbl_new(NULL);

    PNode* mod_decl = top->list.at[0];
    mod.name = pnode_span(mod_decl->module_decl.ident);
    printf("starting check at head module '"str_fmt"'\n", str_arg(mod.name));

    // pre-collect entitites
    for_n (i, 1, top->list.len) {
        PNode* tls = top->list.at[i];
        switch (tls->base.kind) {
        case PN_STMT_DECL:
            PNode* ident = tls->decl.ident;
            string ident_name = pnode_span(ident);
            Entity* ent = etbl_put(mod.global, ident_name);
            ent->check_status = CHK_NONE;
            ent->decl_pnode = tls;
            break;
        default:
            UNREACHABLE;
        }
    }

    // check stmts
    for_n (i, 1, top->list.len) {
        PNode* tls = top->list.at[i];
        check_stmt(&mod, mod.global, tls);
    }

    return mod;
}