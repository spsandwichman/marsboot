#include "parse.h"

// dont care about performance or efficenty, only hackability just fuckin 
// cobble shit together so i can write the GOOD compiler in mars

static struct Parser {
    Token* tb;
    usize len;
    usize cursor;

    // stats
    usize node_count;
    usize mem_allocated;
} p;

static Token* current() {
    return &p.tb[p.cursor];
}

static void advance() {
    p.cursor = min(p.cursor + 1, p.len - 1);
}

static Token* peek(isize n) {
    return &p.tb[p.cursor + n];
}

static bool match(u8 token_kind) {
    return current()->kind == token_kind;
}

static void report_token(bool error, char* message, ...) {
    va_list args;
    va_start(args, message);

    SourceFile* f = ctx_find_from_substring(current()->raw, current()->len);
    assert(f != NULL);

    string highlight = {current()->raw, current()->len};

    emit_report(error, f->text, f->path, highlight, message, args);
    va_end(args);
}

static void expect(u8 token_kind) {
    if (!match(token_kind)) {
        report_token(true, "expected '%s', got '%s'", token_kind_name[token_kind], token_kind_name[current()->kind]);
    }
}


static void span_copy(PNode* p, PNode* from) {
    p->base.raw = from->base.raw;
    p->base.len = from->base.len;
}
static void span_begin(PNode* p) {
    p->base.raw = current()->raw;
    p->base.len = current()->len;
}

static void span_extend(PNode* node, isize offset) {
    node->base.len = p.tb[p.cursor + offset].raw + p.tb[p.cursor + offset].len - node->base.raw;
}

static string span(PNode* p) {
    return (string){.raw = p->base.raw, .len = p->base.len};
}

#define new_node(T, kind) new_node_with_size(sizeof(((PNode*)0)->T), kind)

static PNode* new_node_with_size(usize size, u8 kind) {
    size = sizeof(PNodeBase) + size;
    PNode* pn = malloc(size);
    memset(pn, 0, size);
    p.mem_allocated += size;
    p.node_count++;
    pn->base.kind = kind;
    span_begin(pn);
    return pn;
}

typedef struct PNodeList {
    PNode** at;
    u32 len;
    u32 cap;

    struct {
        char* raw;
        u32 len;
    } span;
} PNodeList;

PNodeList list_new(u32 cap) {
    PNodeList list;
    da_init(&list, cap);
    list.span.raw = current()->raw;
    list.span.len = current()->len;
    return list;
}

static PNode* list_solidify(PNodeList l) {
    PNode* block = new_node(list, PN_LIST);
    // da_shrink(&l);
    block->list.at = l.at;
    block->list.len = l.len;
    block->base.raw = l.span.raw;
    block->base.len = l.span.len;
    return block;
}

PNode* parse_module_decl() {
    expect(TOK_KEYWORD_MODULE);
    PNode* decl = new_node(module_decl, PN_STMT_MODULE_DECL);
    advance();
    decl->module_decl.ident = parse_ident();
    expect(TOK_SEMICOLON);
    advance();
    span_extend(decl, -1);
    return decl;
}

PNode* parse_import_decl() {
    expect(TOK_KEYWORD_IMPORT);
    PNode* decl = new_node(module_decl, PN_STMT_IMPORT_DECL);
    advance();
    decl->import_decl.ident = parse_ident();
    decl->import_decl.ident = parse_string();
    expect(TOK_SEMICOLON);
    advance();
    return decl;
}


// ('let' | 'mut') list<ident> [':' expr] ['=' expr]
PNode* parse_var_decl(bool is_extern) {
    PNode* decl = new_node(decl, PN_STMT_DECL);
    switch (current()->kind) {
    case TOK_KEYWORD_LET: decl->decl.kind = DECLKIND_LET; break;
    case TOK_KEYWORD_MUT: decl->decl.kind = DECLKIND_MUT; break;
    case TOK_KEYWORD_DEF: 
        decl->decl.kind = DECLKIND_DEF;
        if (is_extern) {
            report_token(true, "cannot declare a compile-time constant as extern");
        }
        break;
    default:
        report_token(true, "expected let, mut, or def");
    }
    advance();

    PNode* ident = parse_ident();
    // turn ident into a identifier list if there's more than one
    if (match(TOK_COMMA)) {
        advance();
        if (match(TOK_IDENTIFIER)) {
            PNodeList ident_list = list_new(4);
            ident_list.span.raw = ident->base.raw;
            ident_list.span.len = ident->base.len;
            da_append(&ident_list, ident);
            while (match(TOK_IDENTIFIER)) {
                ident = parse_ident();
                da_append(&ident_list, ident);
                if (match(TOK_COMMA)) {
                    advance();
                    continue;
                } else {
                    ident = list_solidify(ident_list);
                    break;
                }
            }
            if (ident->base.kind != PN_LIST) {
                ident = list_solidify(ident_list);
            }
        }
    }
    decl->decl.ident = ident;


    if (match(TOK_COLON)) {
        advance();
        decl->decl.type = parse_expr();
    } else if (is_extern) {
        report_token(true, "extern declaration requires type");
    }

    if (match(TOK_EQUAL)) {
        if (is_extern) {
            report_token(true, "extern declaration cannot have a value");
        }
        advance();
        decl->decl.value = parse_expr();
    } else if (decl->decl.kind == DECLKIND_DEF) {
        report_token(true, "def declaration must have a value");
    }
    
    return decl;
}

PNode* parse_item_list(u8 terminator) {
    PNodeList items = list_new(8);
    while (!match(terminator)) {
        // ident [',' ident] ':' expr
        PNode* item = new_node(item, PN_ITEM);
        PNodeList identifiers = list_new(1);
        if (!match(TOK_IDENTIFIER) && !match(TOK_IDENTIFIER_DISCARD)) {
            expect(TOK_IDENTIFIER);
        }
        while (match(TOK_IDENTIFIER) || match(TOK_IDENTIFIER_DISCARD)) {
            PNode* ident = parse_ident();
            da_append(&identifiers, ident);
            if (match(TOK_COMMA)) {
                advance();
                continue;
            } else {
                break;
            }
        }
        expect(TOK_COLON);
        advance();
        if (identifiers.len == 1) {
            item->item.ident = identifiers.at[0];
            da_destroy(&identifiers);
        } else {
            item->item.ident = list_solidify(identifiers);
        }
        item->item.type = parse_expr();
        da_append(&items, item);
        if (match(TOK_COMMA)) {
            advance();
            continue;
        } else {
            break;
        }
    }
    return list_solidify(items);
}

PNode* parse_fn_prototype() {
    PNode* proto = new_node(fnproto, PN_FNPROTO);
    advance();
    if (match(TOK_IDENTIFIER)) {
        proto->fnproto.ident = parse_ident();
    }
    expect(TOK_OPEN_PAREN);
    advance();
    proto->fnproto.param_list = parse_item_list(TOK_CLOSE_PAREN);
    expect(TOK_CLOSE_PAREN);
    advance();
    if (match(TOK_ARROW_RIGHT)) {
        advance();
        if (match(TOK_OPEN_PAREN)) {
            advance();
            proto->fnproto.returns = parse_item_list(TOK_CLOSE_PAREN);
            expect(TOK_CLOSE_PAREN);
            advance();
        } else {
            proto->fnproto.returns = parse_expr();
        }
    }
    return proto;
}

PNode* parse_stmt_group() {
    expect(TOK_OPEN_BRACE);

    PNodeList stmts = list_new(32);
    advance();
    while (!match(TOK_CLOSE_BRACE)) {
        PNode* stmt = parse_stmt();
        da_append(&stmts, stmt);
    }
    expect(TOK_CLOSE_BRACE);
    advance();
    return list_solidify(stmts);
}

PNode* parse_do_group() {
    if (match(TOK_KEYWORD_DO)) {
        PNode* do_clause = new_node(unop, PN_DO);
        advance();
        do_clause->unop.sub = parse_stmt();
        span_extend(do_clause, -1);
        return do_clause;
    }
    return parse_stmt_group();
}

// cont is either TOK_KEYWORD_IF or TOK_KEYWORD_WHEN
PNode* parse_if_stmt(u8 cont) {
    bool is_when = cont == TOK_KEYWORD_WHEN;
    PNode* tern = new_node(ternary, is_when ? PN_STMT_WHEN : PN_STMT_IF);
    advance();
    tern->ternary.cond = parse_expr();
    tern->ternary.if_true = parse_do_group();
    if (match(TOK_KEYWORD_ELSE)) {
        advance();
        if (match(cont)) {
            tern->ternary.if_false = parse_if_stmt(cont);
        } else {
            tern->ternary.if_false = parse_do_group();
        }
    }
    span_extend(tern, -1);
    return tern;
}

PNode* parse_case_block() {
    PNode* cb = new_node(switch_case, PN_CASE_BLOCK);
    expect(TOK_KEYWORD_CASE);
    advance();
    PNodeList matches = list_new(1);
    while (!match(TOK_COLON)) {
        PNode* condition = parse_expr();
        da_append(&matches, condition);
        if (match(TOK_COMMA)) {
            advance();
            continue;
        } else {
            break;
        }
    }
    cb->switch_case.matches = list_solidify(matches);
    expect(TOK_COLON);
    advance();

    PNodeList stmts = list_new(8);
    while (!match(TOK_KEYWORD_CASE) && !match(TOK_CLOSE_BRACE)) {
        PNode* stmt = parse_stmt();
        da_append(&stmts, stmt);
    }
    cb->switch_case.sub = list_solidify(stmts);
    return cb;
}

PNode* parse_switch_stmt(bool is_which) {
    PNode* stmt = new_node(switch_stmt, is_which ? PN_STMT_WHICH : PN_STMT_SWITCH);
    advance();
    stmt->switch_stmt.cond = parse_expr();
    expect(TOK_OPEN_BRACE);
    advance();


    PNodeList cases = list_new(8);
    while (!match(TOK_CLOSE_BRACE)) {
        PNode* switch_case = parse_case_block();
        da_append(&cases, switch_case);
    }

    advance();

    stmt->switch_stmt.cases = list_solidify(cases);
    return stmt;
}

static u8 assign_kind[_TOK_COUNT] = {
    [TOK_EQUAL] = PN_STMT_ASSIGN,
    [TOK_ADD_EQUAL] = PN_STMT_ASSIGN_ADD,
    [TOK_SUB_EQUAL] = PN_STMT_ASSIGN_SUB,
    [TOK_MUL_EQUAL] = PN_STMT_ASSIGN_MUL,
    [TOK_DIV_EQUAL] = PN_STMT_ASSIGN_DIV,
    [TOK_MOD_EQUAL] = PN_STMT_ASSIGN_MOD,
    [TOK_MOD_MOD_EQUAL] = PN_STMT_ASSIGN_REM,
    [TOK_AND_EQUAL] = PN_STMT_ASSIGN_BIT_AND,
    [TOK_OR_EQUAL] = PN_STMT_ASSIGN_BIT_OR,
    [TOK_NOR_EQUAL] = PN_STMT_ASSIGN_NOR,
    [TOK_XOR_EQUAL] = PN_STMT_ASSIGN_XOR,
    [TOK_LSHIFT_EQUAL] = PN_STMT_ASSIGN_LSHIFT,
    [TOK_RSHIFT_EQUAL] = PN_STMT_ASSIGN_RSHIFT,
};

// NOTE: does NOT consume semicolons
PNode* parse_simple_stmt() {
    PNode* stmt;
    switch (current()->kind) {
    case TOK_KEYWORD_LET:
    case TOK_KEYWORD_MUT:
    case TOK_KEYWORD_DEF:
        stmt = parse_var_decl(false);
        break;
    default:
        // assignment or expr
        PNode* lhs = parse_expr();
        if (assign_kind[current()->kind] == 0) {
            // expr stmt
            stmt = lhs;
        } else {
            // assignment
            stmt = new_node(binop, assign_kind[current()->kind]);
            stmt->binop.lhs = lhs;
            advance();
            stmt->binop.rhs = parse_expr();
        }
    }
    return stmt;
}

PNode* parse_for_stmt() {
    expect(TOK_KEYWORD_FOR);
    advance();
    Token* begin = current();
    PNode* left = parse_simple_stmt();
    if (current()->kind == TOK_KEYWORD_IN) {
        // parse ranged for loop
        PNode* fl = new_node(for_ranged, PN_STMT_FOR_RANGED);
        // copy span
        fl->base.raw = begin->raw;
        fl->base.len = begin->len;

        fl->for_ranged.decl = left;
        expect(TOK_KEYWORD_IN);
        advance();
        fl->for_ranged.range = parse_expr();
        fl->for_ranged.block = parse_do_group();
        return fl;
    } else {
        // parse cstyle loop
        PNode* fl = new_node(for_cstyle, PN_STMT_FOR_CSTYLE);
        // copy span
        fl->base.raw = begin->raw;
        fl->base.len = begin->len;

        fl->for_cstyle.init = left;
        expect(TOK_SEMICOLON);
        advance();
        if (!match(TOK_SEMICOLON)) {
            fl->for_cstyle.cond = parse_expr();
        }
        expect(TOK_SEMICOLON);
        advance();
        if (!match(TOK_KEYWORD_DO) && !match(TOK_OPEN_BRACE)) {
            fl->for_cstyle.post = parse_simple_stmt();
        }
        fl->for_cstyle.block = parse_do_group();
        return fl;
    }
}

PNode* parse_extern_decl() {
    PNode* stmt = new_node(unop, PN_STMT_EXTERN_DECL);
    advance();

    switch (current()->kind) {
    case TOK_KEYWORD_FN:
        stmt->unop.sub = parse_fn_prototype();
        break;
    case TOK_KEYWORD_LET:
    case TOK_KEYWORD_MUT:
        stmt->unop.sub = parse_var_decl(true);
        break;
    default:
        report_token(true, "expected function or variable declaration");
    }
    expect(TOK_SEMICOLON);
    advance();
    return stmt;
}

PNode* parse_stmt() {
    PNode* stmt;
    switch (current()->kind) {
    case TOK_OPEN_BRACE:
        stmt = parse_stmt_group();
        break;
    case TOK_KEYWORD_EXTERN:
        stmt = parse_extern_decl();
        break;
    case TOK_SEMICOLON:
        stmt = new_node(_, PN_STMT_EMPTY);
        advance();
        break;
    case TOK_KEYWORD_FN:
        stmt = new_node(fn_decl, PN_STMT_FN_DECL);
        stmt->fn_decl.proto = parse_fn_prototype();
        stmt->fn_decl.stmts = parse_stmt_group(true);
        span_extend(stmt, -1);
        break;
    case TOK_KEYWORD_RETURN:
        stmt = new_node(expr_stmt, PN_STMT_RETURN);
        advance();
        if (match(TOK_SEMICOLON)) {
            break;
        }
        PNode* ret = parse_expr();
        if (match(TOK_COMMA)) {
            advance();
            // return is a sequence!
            PNodeList ret_list = list_new(4);
            da_append(&ret_list, ret);
            while (!match(TOK_SEMICOLON)) {
                PNode* expr = parse_expr();
                da_append(&ret_list, expr);

                if (match(TOK_COMMA)) {
                    advance();
                    continue;
                } else {
                    break;
                }
            }
            stmt->expr_stmt.expr = list_solidify(ret_list);
            expect(TOK_SEMICOLON);
            advance();
            break;
        }
        expect(TOK_SEMICOLON);
        advance();
        stmt->expr_stmt.expr = ret;
        span_extend(stmt, -1);
        break;
    case TOK_KEYWORD_IF:
    case TOK_KEYWORD_WHEN:
        stmt = parse_if_stmt(current()->kind);
        break;
    case TOK_KEYWORD_SWITCH:
    case TOK_KEYWORD_WHICH:
        stmt = parse_switch_stmt(match(TOK_KEYWORD_WHICH));
        break;
    case TOK_KEYWORD_WHILE:
        stmt = new_node(binop, PN_STMT_WHILE);
        advance();
        stmt->binop.lhs = parse_expr();
        stmt->binop.rhs = parse_do_group();
        span_extend(stmt, -1);
        break;
    case TOK_KEYWORD_FOR:
        stmt = parse_for_stmt();
        break;
    case TOK_KEYWORD_DEFER:
        stmt = new_node(unop, PN_STMT_DEFER);
        advance();
        stmt->unop.sub = parse_stmt();
        span_extend(stmt, -1);
        break;
    case TOK_KEYWORD_FALLTHROUGH:
        stmt = new_node(fallthrough, PN_STMT_FALLTHROUGH);
        advance();
        if (!match(TOK_SEMICOLON)) {
            stmt->fallthrough.first = parse_expr();
            if (match(TOK_COMMA)) {
                advance();
                if (!match(TOK_SEMICOLON)) {
                    stmt->fallthrough.second = parse_expr();
                }
            }
        }
        expect(TOK_SEMICOLON);
        advance();
        span_extend(stmt, -1);
        break;
    case TOK_KEYWORD_CONTINUE:
    case TOK_KEYWORD_BREAK:
        stmt = new_node(cflow, match(TOK_KEYWORD_BREAK) ? PN_STMT_BREAK : PN_STMT_CONTINUE);
        advance();
        if (match(TOK_IDENTIFIER)) {
            stmt->cflow.label = parse_ident();
        }
        expect(TOK_SEMICOLON);
        advance();
        span_extend(stmt, -1);
        break;
    case TOK_IDENTIFIER:
        if (peek(1)->kind == TOK_COLON) {
            // label!
            stmt = new_node(label, PN_STMT_LABEL);
            stmt->label.ident = parse_ident();
            advance();
            stmt->label.stmt = parse_stmt();
            span_extend(stmt, -1);
            break;
        }
        // else, fallthrough to simple_stmt parsing
    default:
        stmt = parse_simple_stmt();
        expect(TOK_SEMICOLON);
        advance();
        // report_token(true, "expected a statement");
    }

    return stmt;
}

PNode* parse_string() {
    expect(TOK_LITERAL_STRING);
    PNode* str = new_node(_, PN_EXPR_STRING);
    advance();
    return str;
}

PNode* parse_ident() {
    if (!match(TOK_IDENTIFIER) && !match(TOK_IDENTIFIER_DISCARD)) {
        expect(TOK_IDENTIFIER);
    }
    PNode* ident = new_node(_, match(TOK_IDENTIFIER) ? PN_IDENT : PN_DISCARD);
    advance();
    return ident;
}

// expr | '[' expr ']' '=' expr | '.' ident '=' expr
PNode* parse_compound_item() {
    PNode* item;
    switch (current()->kind) {
    case TOK_OPEN_BRACKET: // [ expr ] = expr
        item = new_node(binop, PN_COMP_ITEM_INDEX);
        advance();
        item->binop.lhs = parse_expr();
        expect(TOK_CLOSE_BRACKET);
        advance();
        expect(TOK_EQUAL);
        advance();
        item->binop.rhs = parse_expr();
        return item;
    case TOK_PERIOD: // . ident = expr
        item = new_node(binop, PN_COMP_ITEM_INDEX);
        advance();
        item->binop.lhs = parse_ident();
        expect(TOK_EQUAL);
        advance();
        item->binop.rhs = parse_expr();
        return item;
    default:
        return parse_expr();
    }
}

PNode* parse_enum_variant() {
    PNode* lhs = parse_expr();
    if (match(TOK_EQUAL)) {
        PNode* variant = new_node(binop, PN_ENUM_VARIANT_VALUED);
        variant->binop.lhs = lhs;
        span_copy(variant, lhs);
        advance();
        variant->binop.rhs = parse_expr();
        span_extend(variant, -1);
        return variant;
    }
    return lhs;
}

static u8 tok2pn_atom[_TOK_COUNT] = {
    [TOK_IDENTIFIER_DISCARD] = PN_DISCARD,
    [TOK_IDENTIFIER] = PN_IDENT,
    [TOK_LITERAL_INT] = PN_EXPR_INTEGER,
    [TOK_LITERAL_CHAR] = PN_EXPR_CHAR,
    [TOK_LITERAL_STRING] = PN_EXPR_STRING,
    [TOK_KEYWORD_TRUE] = PN_EXPR_TRUE,
    [TOK_KEYWORD_FALSE] = PN_EXPR_FALSE,
    [TOK_KEYWORD_NULL] = PN_EXPR_NULL,
    [TOK_KEYWORD_I8] = PN_TYPE_I8,
    [TOK_KEYWORD_I16] = PN_TYPE_I16,
    [TOK_KEYWORD_I32] = PN_TYPE_I32,
    [TOK_KEYWORD_I64] = PN_TYPE_I64,
    [TOK_KEYWORD_INT] = PN_TYPE_INT,
    [TOK_KEYWORD_U8] = PN_TYPE_U8,
    [TOK_KEYWORD_U16] = PN_TYPE_U16,
    [TOK_KEYWORD_U32] = PN_TYPE_U32,
    [TOK_KEYWORD_U64] = PN_TYPE_U64,
    [TOK_KEYWORD_UINT] = PN_TYPE_UINT,
    [TOK_KEYWORD_F16] = PN_TYPE_F16,
    [TOK_KEYWORD_F32] = PN_TYPE_F32,
    [TOK_KEYWORD_F64] = PN_TYPE_F64,
    [TOK_KEYWORD_FLOAT] = PN_TYPE_FLOAT,
    [TOK_KEYWORD_BOOL] = PN_TYPE_BOOL,
    [TOK_KEYWORD_DYN] = PN_TYPE_DYN,
    [TOK_KEYWORD_TYPEID] = PN_TYPE_TYPEID,
};

// allow_none allows this to return none if there was no 
// expression found, instead of erroring
// useful for ^let/^mut parsing
PNode* parse_atom_terminal(bool allow_none) {
    u8 kind = tok2pn_atom[current()->kind];
    if (kind != 0) {
        PNode* simple = new_node(_, kind);
        advance();
        return simple;
    }
    PNode* term = NULL;
    switch (current()->kind) {
    case TOK_KEYWORD_ENUM:
        term = new_node(enum_type, PN_TYPE_ENUM);
        advance();
        term->enum_type.type = parse_expr();
        expect(TOK_OPEN_BRACE);
        PNodeList items = list_new(8);
        advance();
        while (!match(TOK_CLOSE_BRACE)) {
            PNode* item = parse_enum_variant();
            da_append(&items, item);
            if (match(TOK_COMMA)) {
                advance();
                continue;
            } else {
                break;
            }
        }
        expect(TOK_CLOSE_BRACE);
        span_extend(term,0);
        advance();
        break;
    case TOK_KEYWORD_STRUCT:
    case TOK_KEYWORD_UNION:
        term = new_node(record_type, match(TOK_KEYWORD_UNION) ? PN_TYPE_UNION : PN_TYPE_STRUCT);
        advance();
        expect(TOK_OPEN_BRACE);
        advance();
        term->record_type.fields = parse_item_list(TOK_CLOSE_BRACE);
        expect(TOK_CLOSE_BRACE);
        advance();
        break;
    case TOK_KEYWORD_FN:
        term = parse_fn_prototype();
        if (match(TOK_OPEN_BRACE)) {
            PNode* fn = new_node(fn_decl, PN_EXPR_ANON_FN);
            span_copy(fn, term);
            fn->fn_decl.proto = term;
            fn->fn_decl.stmts = parse_stmt_group();
            span_extend(fn, -1);
            term = fn;
        }
        break;
    case TOK_OPEN_PAREN:
        advance();
        term = parse_expr();
        expect(TOK_CLOSE_PAREN);
        advance();
        break;
    case TOK_OPEN_BRACKET:
        if (peek(1)->kind == TOK_CLOSE_BRACKET) {
            term = new_node(ref_type, PN_TYPE_SLICE);
            advance();
            advance();
            if (match(TOK_KEYWORD_MUT)) {
                term->ref_type.mutable = true;
            } else if (match(TOK_KEYWORD_LET)) {
                term->ref_type.mutable = false;
            } else {
                report_token(true, "expected mut or let");
            }
            advance();
            term->ref_type.sub = parse_atom_terminal(false);
        } else {
            term = new_node(array_type, PN_TYPE_ARRAY);
            advance();
            term->array_type.len = parse_expr();
            expect(TOK_CLOSE_BRACKET);
            advance();
            term->ref_type.sub = parse_atom_terminal(false);
        }
        break;
    case TOK_CARET:
        term = new_node(ref_type, PN_TYPE_POINTER);
        advance();
        if (match(TOK_KEYWORD_MUT)) {
            term->ref_type.mutable = true;
        } else if (match(TOK_KEYWORD_LET)) {
            term->ref_type.mutable = false;
        } else {
            report_token(true, "expected mut or let");
        }
        advance();
        // allow for subtype to be nothing
        term->ref_type.sub = parse_atom_terminal(true);
        break;
    case TOK_KEYWORD_DISTINCT:
        term = new_node(unop, PN_TYPE_DISTINCT);
        advance();
        term->unop.sub = parse_atom_terminal(false);
        break;
    case TOK_PERIOD:
        if (peek(1)->kind == TOK_OPEN_BRACE) {
            term = new_node(compound, PN_EXPR_COMPOUND);
            term->compound.type = NULL;
            advance();
            advance();
            PNodeList items = list_new(8);
            while (!match(TOK_CLOSE_BRACE)) {
                PNode* item = parse_compound_item();
                da_append(&items, item);
                if (match(TOK_COMMA)) {
                    advance();
                    continue;
                } else {
                    break;
                }
            }
            expect(TOK_CLOSE_BRACE);
            span_extend(term,0);
            term->compound.expr_list = list_solidify(items);
            advance();
        } else {
            term = new_node(unop, PN_EXPR_IMPLICIT_SELECTOR);
            advance();
            term->unop.sub = parse_ident();
        }
        break;
    default:
        if (allow_none) return NULL;
        report_token(true, "expected expression");
    }
    span_extend(term, -1);
    return term;
}

PNode* parse_atom() {
    PNode* left = parse_atom_terminal(false);

    PNode* atom = left;
    while (true) {
        switch (current()->kind) {
        case TOK_OPEN_PAREN:
            atom = new_node(call, PN_EXPR_CALL);
            span_copy(atom, left);
            atom->call.callee = left;
            advance();
            PNodeList params = list_new(4);
            while (!match(TOK_CLOSE_PAREN)) {
                PNode* expr = parse_expr();
                da_append(&params, expr);
                if (match(TOK_COMMA)) {
                    advance();
                    continue;
                } else {
                    break;
                }
            }
            atom->call.param_list = list_solidify(params);
            expect(TOK_CLOSE_PAREN);
            advance();
            span_extend(atom, -1);
            break;
        case TOK_COLON_COLON:
            atom = new_node(binop, PN_EXPR_NAMESPACE);
            atom->binop.lhs = left;
            span_copy(atom, left);
            advance();
            atom->binop.rhs = parse_ident();
            span_extend(atom, -1);
            break;
        case TOK_PERIOD:
            if (peek(1)->kind == TOK_OPEN_BRACE) {
                atom = new_node(compound, PN_EXPR_COMPOUND);
                atom->compound.type = left;
                advance();
                advance();
                PNodeList items = list_new(8);
                while (!match(TOK_CLOSE_BRACE)) {
                    PNode* item = parse_compound_item();
                    da_append(&items, item);
                    if (match(TOK_COMMA)) {
                        advance();
                        continue;
                    } else {
                        break;
                    }
                }
                expect(TOK_CLOSE_BRACE);
                span_extend(atom,0);
                atom->compound.expr_list = list_solidify(items);
                advance();
                break;
            }
            atom = new_node(binop, PN_EXPR_SELECTOR);
            atom->binop.lhs = left;
            span_copy(atom, left);
            advance();
            atom->binop.rhs = parse_ident();
            span_extend(atom, -1);
            break;
        case TOK_CARET:
            atom = new_node(unop, PN_EXPR_DEREF);
            atom->unop.sub = left;
            span_copy(atom, left);
            advance();
            span_extend(atom, -1);
            break;
        case TOK_OPEN_BRACKET:
            advance();
            PNode* left_indexer = NULL;
            if (!match(TOK_COLON)) {
                left_indexer = parse_expr();
            }
            if (match(TOK_COLON)) {
                atom = new_node(slice, PN_EXPR_SLICE);
                atom->slice.left_bound = left_indexer;
                advance();
                if (!match(TOK_CLOSE_BRACKET)) {
                    atom->slice.right_bound = parse_expr();
                }
                span_extend(atom, 0);
                expect(TOK_CLOSE_BRACKET);
                advance();
            } else {
                // regular indexed access
                atom = new_node(binop, PN_EXPR_INDEX);
                span_copy(atom, left);
                atom->binop.lhs = left;
                atom->binop.rhs = left_indexer;
                span_extend(atom, 0);
                expect(TOK_CLOSE_BRACKET);
                advance();
            }
            break;
        default:
            return atom;
        }
        left = atom;
    }

    return left;
}

PNode* parse_unary() {
    u8 kind = 0;
    switch (current()->kind) {
    case TOK_KEYWORD_CAST:
    case TOK_KEYWORD_BITCAST:
        PNode* cast = new_node(binop, 
            match(TOK_KEYWORD_CAST) ? PN_EXPR_CAST : PN_EXPR_BITCAST
        );
        advance();
        expect(TOK_OPEN_PAREN);
        advance();
        cast->binop.lhs = parse_expr();
        expect(TOK_CLOSE_PAREN);
        advance();
        cast->binop.rhs = parse_unary();
        span_extend(cast, -1);
        return cast;

    // this should be turned into a table, dont care rn tho
    case TOK_AND: kind = PN_EXPR_ADDR; break;
    case TOK_KEYWORD_OFFSETOF: kind = PN_EXPR_OFFSETOF; break;
    case TOK_EXCLAM: kind = PN_EXPR_BOOL_NOT; break;
    case TOK_QUESTION: kind = PN_EXPR_BOOL_COERCE; break;
    case TOK_TILDE: kind = PN_EXPR_BIT_NOT; break;
    case TOK_SUB: kind = PN_EXPR_NEG; break;
    case TOK_ADD: kind = PN_EXPR_POS; break; // does nothing lol
    case TOK_KEYWORD_ALIGNOF: kind = PN_EXPR_ALIGNOF; break;
    case TOK_KEYWORD_SIZEOF: kind = PN_EXPR_SIZEOF; break;
    case TOK_KEYWORD_TYPEOF: kind = PN_TYPE_TYPEOF; break;
    default:
        return parse_atom();
    }
    PNode* unary = new_node(unop, 0);
    advance();
    unary->base.kind = kind;
    unary->unop.sub = parse_unary();
    span_extend(unary, -1);
    return unary;
}

static isize bin_precedence(u8 kind) {
    switch (kind) {
    case TOK_LSHIFT:
    case TOK_RSHIFT:
        return 10;
    case TOK_AND:
        return 9;
    case TOK_OR:
    case TOK_TILDE:
    case TOK_NOR:
        return 8;
    case TOK_MUL:
    case TOK_DIV:
    case TOK_MOD:
    case TOK_MOD_MOD:
        return 7;
    case TOK_ADD:
    case TOK_SUB:
        return 6;
    case TOK_EQUAL_EQUAL:
    case TOK_NOT_EQUAL:
    case TOK_LESS_THAN:
    case TOK_LESS_EQUAL:
    case TOK_GREATER_THAN:
    case TOK_GREATER_EQUAL:
    case TOK_RANGE_EQ:
    case TOK_RANGE_LESS:
        return 5;
    case TOK_KEYWORD_IN:
        return 4;
    case TOK_AND_AND:
        return 3;
    case TOK_OR_OR:
        return 2;
    }
    return 0;
}

static u8 bin_kind[_TOK_COUNT] = {
    [TOK_ADD] = PN_EXPR_ADD,
    [TOK_SUB] = PN_EXPR_SUB,
    [TOK_MUL] = PN_EXPR_MUL,
    [TOK_DIV] = PN_EXPR_DIV,
    [TOK_MOD] = PN_EXPR_MOD,
    [TOK_MOD_MOD] = PN_EXPR_REM,
    [TOK_AND] = PN_EXPR_BIT_AND,
    [TOK_OR] = PN_EXPR_BIT_OR,
    [TOK_NOR] = PN_EXPR_NOR,
    [TOK_TILDE] = PN_EXPR_XOR,
    [TOK_LSHIFT] = PN_EXPR_LSHIFT,
    [TOK_RSHIFT] = PN_EXPR_RSHIFT,
    [TOK_AND_AND] = PN_EXPR_BOOL_AND,
    [TOK_OR_OR] = PN_EXPR_BOOL_OR,
    [TOK_EQUAL] = PN_EXPR_EQ,
    [TOK_NOT_EQUAL] = PN_EXPR_NEQ,
    [TOK_LESS_THAN] = PN_EXPR_LESS,
    [TOK_LESS_EQUAL] = PN_EXPR_LESS_EQ,
    [TOK_GREATER_THAN] = PN_EXPR_GREATER,
    [TOK_GREATER_EQUAL] = PN_EXPR_GREATER_EQ,
    [TOK_RANGE_EQ] = PN_EXPR_RANGE_EQ,
    [TOK_RANGE_LESS] = PN_EXPR_RANGE_LESS,
    [TOK_KEYWORD_IN] = PN_EXPR_IN,
};

PNode* parse_case_expr() {
    PNode* cb = new_node(switch_case, PN_CASE_BLOCK);
    expect(TOK_KEYWORD_CASE);
    advance();
    PNodeList matches = list_new(1);
    while (!match(TOK_COLON)) {
        PNode* condition = parse_expr();
        da_append(&matches, condition);
        if (match(TOK_COMMA)) {
            advance();
            continue;
        } else {
            break;
        }
    }
    cb->switch_case.matches = list_solidify(matches);
    expect(TOK_COLON);
    advance();
    cb->switch_case.sub = parse_expr();
    return cb;
}

PNode* parse_switch_expr(bool is_which) {
    PNode* stmt = new_node(switch_stmt, is_which ? PN_EXPR_WHICH : PN_EXPR_SWITCH);
    advance();
    stmt->switch_stmt.cond = parse_expr();
    expect(TOK_OPEN_BRACE);
    advance();


    PNodeList cases = list_new(8);
    while (!match(TOK_CLOSE_BRACE)) {
        PNode* switch_case = parse_case_expr();
        da_append(&cases, switch_case);
        if (match(TOK_COMMA)) {
            advance();
            continue;
        } else {
            break;
        }
    }
    expect(TOK_CLOSE_BRACE);

    advance();

    stmt->switch_stmt.cases = list_solidify(cases);
    return stmt;
}

PNode* parse_binary(isize precedence) {
    switch (current()->kind) {
    case TOK_KEYWORD_SWITCH:
    case TOK_KEYWORD_WHICH:
        return parse_switch_expr(current()->kind == TOK_KEYWORD_WHICH);
    case TOK_KEYWORD_IF:
    case TOK_KEYWORD_WHEN:
        PNode* tern = new_node(ternary, current()->kind == TOK_KEYWORD_IF ? PN_EXPR_IF : PN_EXPR_WHEN);
        advance();
        tern->ternary.cond = parse_expr();
        expect(TOK_KEYWORD_DO);
        advance();
        tern->ternary.if_true = parse_expr();
        expect(TOK_KEYWORD_ELSE);
        advance();
        tern->ternary.if_false = parse_expr();
        span_extend(tern, -1);
        return tern;
    }

    PNode* lhs = parse_unary();

    while (precedence < bin_precedence(current()->kind)) {
        isize prec = bin_precedence(current()->kind);
        PNode* bin = new_node(binop, bin_kind[current()->kind]);
        advance();
        span_copy(bin, lhs);
        bin->binop.lhs = lhs;
        bin->binop.rhs = parse_binary(precedence);
        span_extend(bin, -1);
        lhs = bin;
    }
    return lhs;
}

PNode* parse_expr() {
    return parse_binary(1);
}


// return NULL if expected_module_name is not the same (we're in a diff module)
// expected_module_name is NULL if we dont actually have an expected name

// TODO this check should be made at an earlier stage, so we dont have to lex the
// whole damn file just to discard it based on the first statement
// again, hackability over performance or efficiency
PNode* parse_file(TokenBuf tb, string expected_module_name) {
    p.cursor = 0;
    p.tb = tb.at;
    p.len = tb.len;

    PNode* mod_decl = parse_module_decl();
    if (expected_module_name.raw != NULL && !string_eq(expected_module_name, span(mod_decl->module_decl.ident))) {
        return NULL;
    }

    PNodeList list = list_new(32);
    da_append(&list, mod_decl);

    while (!match(TOK_EOF) && match(TOK_KEYWORD_IMPORT)) {
        PNode* import = parse_import_decl();
        da_append(&list, import);
    }

    while (!match(TOK_EOF)) {
        PNode* import = parse_stmt();
        da_append(&list, import);
    }

    printf("parse %u nodes (%u B)\n", p.node_count, p.mem_allocated);

    return list_solidify(list);
}