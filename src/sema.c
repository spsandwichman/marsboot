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

EntityTable* etbl_new(EntityTable* parent, Type t) {
    EntityTable* etbl = malloc(sizeof(*etbl));
    memset(etbl, 0, sizeof(*etbl));
    etbl->parent = parent;
    da_init(etbl, 32);
    return etbl;
}

Entity* etbl_search(EntityTable* etbl, string name) {
    if (etbl != NULL) return NULL;
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

SemaExpr* check_expr(Module* m, EntityTable* etbl, PNode* pn, Type expected) {
    
}

static Type ingest_type_internal(Module* m, EntityTable* etbl, PNode* pn) {
    switch (pn->base.kind) {
    case PN_TYPE_I8:  return TYPE_I8;
    case PN_TYPE_I16: return TYPE_I16;
    case PN_TYPE_I32: return TYPE_I32;
    case PN_TYPE_I64: return TYPE_I64;

    case PN_TYPE_U8:  return TYPE_U8;
    case PN_TYPE_U16: return TYPE_U16;
    case PN_TYPE_U32: return TYPE_U32;
    case PN_TYPE_U64: return TYPE_U64;

    case PN_TYPE_F16: return TYPE_F16;
    case PN_TYPE_F32: return TYPE_F32;
    case PN_TYPE_F64: return TYPE_F64;

    case PN_TYPE_BOOL:   return TYPE_BOOL;
    case PN_TYPE_DYN:    return TYPE_DYN;
    case PN_TYPE_TYPEID: return TYPE_TYPEID;
    }
    TODO("ingest complex type");
    return TYPE_UNKNOWN;
}

Type ingest_type(Module* m, EntityTable* etbl, PNode* pn) {
    Type t = ingest_type_internal(m, etbl, pn);
    type_condense();
    return t;
}

SemaStmt* new_stmt(Module* m, u8 kind) {
    SemaStmt* stmt = malloc(sizeof(*stmt));
    memset(stmt, 0, sizeof(*stmt));
    stmt->kind = kind;
    return stmt;
}

SemaStmt* check_decl(Module* m, EntityTable* etbl, PNode* pstmt) {
    assert(pstmt->decl.ident->base.kind == PN_IDENT); // no lists of decls yet!
    string name = pnode_span(pstmt->decl.ident);

    if (etbl_search(etbl, name)) {
        report_pnode(true, pstmt->decl.ident, "name already declared");
    }
    Entity* ent = etbl_put(etbl, name);

    Type ent_type = TYPE_UNKNOWN;
    if (pstmt->decl.type) {
        ent_type = ingest_type(m, etbl, pstmt->decl.type);
    }

    u8 decl_kind = pstmt->decl.kind;
    SemaStmt* stmt = new_stmt(m, SSTMT_DECL);

    stmt->decl.entity = ent;
    stmt->decl.value = check_expr(m, etbl, pstmt->decl.value, ent_type);

    return stmt;
}

SemaStmt* check_stmt(Module* m, EntityTable* etbl, PNode* pstmt) {
    switch (pstmt->base.kind) {
    case PN_STMT_DECL:
        return check_decl(m, etbl, pstmt);
    default:
        TODO("lmao XD");
    }
}

Module sema_check_module(PNode* top) {
    Module mod = {
        .name = constr("placeholder"),
    };
    assert(top->base.kind == PN_LIST);
    da_init(&mod.decls, top->list.len);
}