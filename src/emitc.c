#include "emitc.h"
#include "strbuilder.h"

/* C emission.

    all names get mangled as {module}_{entity}
    enum variants get mangled as {module}_{type}_{variant}
    enum types are defined under the `enum` namespace, 
    but are globally `typedef`d to their underlying type. for example:
        module foo;
        def Bar = enum u8 {
            A, B, C
        };
    gets emitted as:
        enum foo_Bar {
            foo_Bar_A = 0, foo_Bar_B = 1, foo_Bar_C = 2
        };
        typedef u8 foo_Bar;

    anonymous entities get mangled as _{module}_{kind}_{rand}
        where {rand} is some sort of random but unique value,
        often a pointer or internal handle of some kind,
        and {kind} is 'var', 'fn', 'type'


    ARRAYS: arrays get put into wrapper structs

*/

static usize indent_level = 0;
static StringBuilder* sb;

static StringBuilder _mangled_sb;

static string gen_mangled(Module* m, string name) {
    if (_mangled_sb.buffer == 0) sb_init(&_mangled_sb);
    sb_clear(&_mangled_sb);
    sb_append(&_mangled_sb, m->name);
    sb_append_char(&_mangled_sb, '_');
    sb_append(&_mangled_sb, name);
    return sb_export(&_mangled_sb);
}

static string gen_mangled_variant(Module* m, Type t, string name) {
    if (_mangled_sb.buffer == 0) sb_init(&_mangled_sb);
    sb_clear(&_mangled_sb);
    sb_append(&_mangled_sb, m->name);
    sb_append_char(&_mangled_sb, '_');
    sb_append(&_mangled_sb, name);
    return sb_export(&_mangled_sb);
}

static string gen_mangled_anon(Module* m, char* kind, void* rand) {
    if (_mangled_sb.buffer == 0) sb_init(&_mangled_sb);
    sb_clear(&_mangled_sb);
    sb_append_char(&_mangled_sb, '_');
    sb_append(&_mangled_sb, m->name);
    sb_append_char(&_mangled_sb, '_');
    sb_append_c(&_mangled_sb, kind);
    sb_append_char(&_mangled_sb, '_');
    sb_printf(&_mangled_sb, "%zu", rand);
    return sb_export(&_mangled_sb);
}

static void emit_mangled(Module* m, string name) {
    sb_append(sb, m->name);
    sb_append_char(sb, '_');
    sb_append(sb, name);
}

static void emit_mangled_variant(Module* m, Type t, string name) {
    sb_append(sb, m->name);
    sb_append_char(sb, '_');
    sb_append(sb, name);
}

static void emit_mangled_anon(Module* m, char* kind, void* rand) {
    sb_append_char(sb, '_');
    sb_append(sb, m->name);
    sb_append_char(sb, '_');
    sb_append_c(sb, kind);
    sb_append_char(sb, '_');
    sb_printf(sb, "%zu", rand);
}

// included in every module's header file.
static const char* const builtin_prelude = 
    "#ifndef NO_MARS_BUILTINS\n"
    "    #define NO_MARS_BUILTINS\n"
    "    #include <stdint.h>\n"
    "    #include <stdbool.h>\n"
    "    typedef int8_t   i8;\n"
    "    typedef int16_t  i16;\n"
    "    typedef int32_t  i32;\n"
    "    typedef int64_t  i64;\n"
    "    typedef uint8_t  u8;\n"
    "    typedef uint16_t u16;\n"
    "    typedef uint32_t u32;\n"
    "    typedef uint64_t u64;\n"
    "    typedef _Float16 f16;\n" 
    "    // TODO f16 causes problems on some platforms/compilers,\n"
    "    //      we should only emit it if it's used\n"
    "    typedef float    f32;\n"
    "    typedef double   f64;\n"
    "    typedef uint64_t typeid;\n"
    "    typedef struct dyn {\n"
    "        void* _raw;\n"
    "        typeid _id;\n"
    "    } dyn;\n"
    "    #define null (void*)0\n"
    "#endif // NO_MARS_BUILTINS\n\n"
;

static void c_emit_type(Type t, bool force_full_type) {
    if (!force_full_type && type_has_name(t)) {
        sb_append(sb, type_get_name(t));
        return;
    }

    if (force_full_type && type(t)->kind < _TYPE_SIMPLE_END) {
        t = type(t)->kind;
    }

    switch(type(t)->kind) {
    case TYPE_STRUCT:
        sb_append_c(sb, "struct ");
        if (force_full_type && type_has_name(t)) {
            sb_append(sb, type_get_name(t));
            sb_append_c(sb, " ");
        }
        sb_append_c(sb, "{\n");
        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField* field = &type(t)->as_record.at[i];
            sb_append_c(sb, "    ");
            c_emit_declaration(true, field->name, field->type, false, false);
            sb_append_c(sb, ";\n");
        }
        sb_append_c(sb, "}");
        break;
    case TYPE_UNION:
        sb_append_c(sb, "union {\n");
        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField* field = &type(t)->as_record.at[i];
            sb_append_c(sb, "    ");
            c_emit_declaration(false, field->name, field->type, false, false);
            sb_append_c(sb, ";\n");
        }
        sb_append_c(sb, "}");
        break;
    default:
        if (type_has_name(t)) {
            sb_append(sb, type_get_name(t));
            return;
        }
        string typestr = type_gen_string(t, false);
        printf("cannot emit type "str_fmt" (%d)\n", str_arg(typestr), t);
        UNREACHABLE;
    }

}

static bool is_standalone_type(Type t, bool force_full_type) {
    if (type(t)->kind < _TYPE_SIMPLE_END || (!force_full_type && type_has_name(t))) {
        return true;
    }

    switch (type(t)->kind) {
    case TYPE_STRUCT:
    case TYPE_UNION:
    case TYPE_ENUM:
    case TYPE_SLICE:
        return true;
    }
    return false;
}

da_typedef(Type);

// is_ret_type is used to declare the return type of a function, it prevents
// the emitter from putting parens around declarators and fails on arrays.
static void c_emit_declarator(bool mutable, string ident, Type base, da(Type)* declarators, bool is_ret_type) {
    if (declarators->len == 0) {
        if (!mutable) {
            sb_append_c(sb, "const ");
        }
        sb_append(sb, ident);
        return;
        return;
    }
    Type declarator = declarators->at[--declarators->len];

    switch (type(declarator)->kind) {
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE:

        if (!type(declarator)->as_ref.mutable) {
            sb_append_c(sb, "const ");
        }
        sb_append_c(sb, is_ret_type ? "*" : "(*");
        c_emit_declarator(mutable, ident, base, declarators, is_ret_type);
        sb_append_c(sb, is_ret_type ? "" : ")");
        break;
    case TYPE_ARRAY:
        assert(!is_ret_type);
        c_emit_declarator(mutable, ident, base, declarators, is_ret_type);
        sb_printf(sb, "[%zu]", type(declarator)->as_array.len);
        break;
    default:
        UNREACHABLE;
    }
}

// force_full_type is used for type declarations and stuff
// is_ret_type is used to declare the return type of a function, it prevents
// the emitter from putting parens around declarators and fails on arrays.
void c_emit_declaration(bool mutable, string ident, Type t, bool force_full_type, bool is_ret_type) {
    static da(Type) declarators;
    if (declarators.at == NULL) { // there should really be a macro for checking uninitialized lol
        da_init(&declarators, 4);
    }
    
    // find base type
    Type base = t;
    while (!is_standalone_type(base, force_full_type)) {
        da_append(&declarators, base);
        switch (type(base)->kind) {
        case TYPE_POINTER:
        case TYPE_BOUNDLESS_SLICE:
            base = type(base)->as_ref.pointee;
            break;
        case TYPE_ARRAY:
            assert(!is_ret_type);
            base = type(base)->as_array.sub;
            break;
        default:
            break;
        }
    }

    // emit base type
    c_emit_type(base, force_full_type);
    sb_append_c(sb, " ");

    // emit declarators
    c_emit_declarator(mutable, ident, base, &declarators, is_ret_type);

    da_clear(&declarators);
}

void c_emit_expr_id(SemaNode* expr) {
    sb_printf(sb, "expr%llu", expr);
}

// is_ret_type is used to declare the return type of a function, it prevents
// the emitter from putting parens around declarators and fails on arrays.
static void c_emit_temp_declarator(bool mutable, SemaNode* expr, Type base, da(Type)* declarators) {
    if (declarators->len == 0) {
        if (!mutable) {
            sb_append_c(sb, "const ");
        }
        c_emit_expr_id(expr);

        return;
    }
    Type declarator = declarators->at[--declarators->len];

    switch (type(declarator)->kind) {
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE:

        if (!type(declarator)->as_ref.mutable) {
            sb_append_c(sb, "const ");
        }
        sb_append_c(sb, "(*");
        c_emit_temp_declarator(mutable, expr, base, declarators);
        sb_append_c(sb, ")");
        break;
    case TYPE_ARRAY:
        c_emit_temp_declarator(mutable, expr, base, declarators);
        sb_printf(sb, "[%zu]", type(declarator)->as_array.len);
        break;
    default:
        UNREACHABLE;
    }
}

// force_full_type is used for type declarations and stuff
// is_ret_type is used to declare the return type of a function, it prevents
// the emitter from putting parens around declarators and fails on arrays.
void c_emit_temp_declaration(bool mutable, SemaNode* expr, Type t, bool force_full_type) {
    static da(Type) declarators;
    if (declarators.at == NULL) { // there should really be a macro for checking uninitialized lol
        da_init(&declarators, 4);
    }
    
    // find base type
    Type base = t;
    while (!is_standalone_type(base, force_full_type)) {
        da_append(&declarators, base);
        switch (type(base)->kind) {
        case TYPE_POINTER:
        case TYPE_BOUNDLESS_SLICE:
            base = type(base)->as_ref.pointee;
            break;
        case TYPE_ARRAY:
            base = type(base)->as_array.sub;
            break;
        default:
            break;
        }
    }

    // emit base type
    c_emit_type(base, force_full_type);
    sb_append_c(sb, " ");

    // emit declarators
    c_emit_temp_declarator(mutable, expr, base, &declarators);

    da_clear(&declarators);
}


void c_emit_function_prototype(string ident, Type fn_type) {
    
    Type ret_type = type(fn_type)->as_function.ret_type;
    c_emit_declaration(true, ident, ret_type, false, true);
    sb_append_c(sb, "(");

    if (type(fn_type)->as_function.params.len != 0) {
        TypeFnParam param = type(fn_type)->as_function.params.at[0];
        c_emit_declaration(true, param.name, param.type, false, false);
    }

    for_n (i, 1, type(fn_type)->as_function.params.len) {
        sb_append_c(sb, ", ");
        TypeFnParam param = type(fn_type)->as_function.params.at[i];
        c_emit_declaration(true, param.name, param.type, false, false);
    }

    sb_append_c(sb, ")");
}

static void indent() {
    for_n(i, 0, indent_level) {
        sb_append_c(sb, "    ");
    }
}

void c_emit_expr(SemaNode* expr) {
    switch (expr->kind) {
    case SN_CONSTVAL:
        indent();
        c_emit_temp_declaration(true, expr, expr->type, false);
        sb_append_c(sb, " = ");
        c_emit_constval(expr);
        sb_append_c(sb, ";\n");
        break;
    case SN_ENTITY:
        indent();
        c_emit_temp_declaration(true, expr, expr->type, false);
        sb_append_c(sb, " = ");
        sb_append(sb, expr->entity->name);
        sb_append_c(sb, ";\n");
        break;
    case SN_BINOP:
        c_emit_expr(expr->binop.lhs);
        c_emit_expr(expr->binop.rhs);
        indent();
        c_emit_temp_declaration(true, expr, expr->type, false);
        sb_append_c(sb, " = ");

        c_emit_expr_id(expr->binop.lhs);
        switch (expr->pnode->base.kind) {
        case PN_EXPR_ADD: sb_append_c(sb, " + "); break;
        case PN_EXPR_SUB: sb_append_c(sb, " - "); break;
        case PN_EXPR_MUL: sb_append_c(sb, " * "); break;
        case PN_EXPR_DIV: sb_append_c(sb, " / "); break;
        case PN_EXPR_MOD: sb_append_c(sb, " % "); break;
        default:
            UNREACHABLE;
        }
        c_emit_expr_id(expr->binop.rhs);
        sb_append_c(sb, ";\n");
        break;
    default:
        printf("attempt emit expr kind %d\n", expr->kind);
        UNREACHABLE;
    }
}

void c_emit_stmt(SemaNode* stmt) {
    
    switch (stmt->kind) {
    case SN_STMT_BLOCK:
        indent();
        sb_append_c(sb, "{\n");
        indent_level++;
        for_vec(SemaNode** substmt, &stmt->list) {
            c_emit_stmt(*substmt);
        }
        indent_level--;
        indent();
        sb_append_c(sb, "}\n");
        break;
    case SN_STMT_RETURN:
        if (stmt->return_stmt.value) {
            c_emit_expr(stmt->return_stmt.value);
        }
        indent();
        sb_append_c(sb, "return ");
        if (stmt->return_stmt.value) {
            c_emit_expr_id(stmt->return_stmt.value);
        }
        sb_append_c(sb, ";\n");
        break;
    default:
        UNREACHABLE;
    }
}

void c_emit_constval(SemaNode* n) {
    switch (n->constval.type) {
    case TYPE_I8:  sb_printf(sb, "%lld", (i8)n->constval.i8); break;
    case TYPE_I16: sb_printf(sb, "%lld", (i16)n->constval.i16); break;
    case TYPE_I32: sb_printf(sb, "%lld", (i32)n->constval.i32); break;
    case TYPE_UNTYPED_INT:
    case TYPE_I64: sb_printf(sb, "%lld", (i64)n->constval.i64); break;
    default:
        UNREACHABLE;
    }
}

void c_emit_simple_expr_zero(Type t) {
    t = type_unwrap_distinct(t);

    if (type_is_numeric(t)) {
        sb_append_c(sb, "0");
        return;
    }

    switch (type(t)->kind) {
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE:
        sb_append_c(sb, "null");
        break;
    case TYPE_UNION:
    case TYPE_STRUCT:
    case TYPE_ARRAY:
    case TYPE_SLICE:
    case TYPE_DYN:
        sb_append_c(sb, "{}");
        break;
    default:
        UNREACHABLE;
    }
}

void c_emit_simple_expr(SemaNode* n) {
    switch (n->kind) {
    case SN_CONSTVAL:
        c_emit_constval(n);
        break;
    default:
        UNREACHABLE;
    }
}

// prepare a module for c emission
void c_prepare(Module* m) {
    // mangle type names
    for_n(t, _TYPE_SIMPLE_END, tg.handles.len) {
        if (type(t)->kind == TYPE_ARRAY) {
            TODO("ARRAYS IN C ARE COMPLETELY FUCKED LMFAO");
        }

        if (type_has_name(t)) {
            string mangled = gen_mangled(m, type_get_name(t));
            type_attach_name(t, mangled);
        } else {
            switch (type(t)->kind) {
            case TYPE_ARRAY:
            case TYPE_POINTER:
            case TYPE_BOUNDLESS_SLICE:
                continue;
            }
            string mangled = gen_mangled_anon(m, "type", type(t));
            type_attach_name(t, mangled);
        }
    }

    // mangle entity names
    for_n(i, 0, m->global->len) {
        Entity* ent = m->global->at[i];
        ent->name = gen_mangled(m, ent->name);
    }
}

static char to_upper(char c) {
    if ('a' <= c && c <= 'z') return c - 'a' + 'A';
    return c;
}

static void c_emit_header_define_symbol(string name) {
    // "foobar" -> "FOOBAR_H"
    for_n(i, 0, name.len) {
        sb_append_char(sb, to_upper(name.raw[i]));
    }
    sb_append_c(sb, "_H");
}

static void c_header_internal(Module* m) {
    sb_append_c(sb, "#ifndef ");
    c_emit_header_define_symbol(m->name);
    sb_append_c(sb, "\n#define ");
    c_emit_header_define_symbol(m->name);
    sb_append_c(sb, "\n\n");

    // builtin types
    sb_append_c(sb, builtin_prelude);

    // emit def defines
    for_n(i, 0, m->decls.len) {
        SemaNode* decl = m->decls.at[i];

        if (decl->kind != SN_VAR_DECL) continue;
        Entity* ent = decl->decl.entity; 
        if (ent->storage != STORAGE_COMPTIME) continue;
        if (ent->type == TYPE_TYPEID) continue;

        sb_append_c(sb, "#define ");
        sb_append(sb, ent->name);
        sb_append_c(sb, " ((");
        c_emit_declaration(true, constr(""), ent->type, false, false);
        sb_append_c(sb, ")(");
        c_emit_simple_expr(decl->decl.value);
        sb_append_c(sb, "))\n");
    }

    // emit predecl typedefs
    for_n(t, _TYPE_SIMPLE_END, tg.handles.len) {
        if (!type_has_name(t)) continue;
        if (type(t)->kind == TYPE_FUNCTION) {
            continue;
        }
        string typename = type_get_name(t);
        sb_append_c(sb, "typedef ");
        c_emit_declaration(true, typename, t, true, false);
        sb_append_c(sb, ";\n");
    }
    sb_append_c(sb, "\n");

    // emit function forward decls
    for_n(i, 0, m->global->len) {
        Entity* ent = m->global->at[i];
        SemaNode* decl = ent->decl;

        switch (ent->decl->kind) {
        case SN_FN_DECL:
            assert(ent->storage == STORAGE_FUNCTION);
            c_emit_function_prototype(ent->name, ent->type);
            sb_append_c(sb, ";\n");
        }
    }
    sb_append_c(sb, "\n");

    // emit variable extern decls
    for_n(i, 0, m->global->len) {
        Entity* ent = m->global->at[i];
        SemaNode* decl = ent->decl;
        switch (ent->decl->kind) {
        case SN_VAR_DECL:
            if (ent->storage == STORAGE_COMPTIME) continue;
            sb_append_c(sb, "extern ");
            c_emit_declaration(ent->mutable, ent->name, ent->type, false, false);
            sb_append_c(sb, ";\n");
        }
    }
    
    sb_append_c(sb, "\n");

    sb_append_c(sb, "#endif // ");
    c_emit_header_define_symbol(m->name);
    sb_append_c(sb, "\n");
}

string c_header(Module* mod) {

    StringBuilder strbuilder;
    sb = &strbuilder;

    sb_init(sb);
    c_header_internal(mod);

    string s = string_alloc(sb->len);
    sb_write(sb, s.raw);
    return s;
}

static void c_body_internal(Module* m) {
    sb_append_c(sb, "#include \"");
    sb_append(sb, m->name);
    sb_append_c(sb, ".h\"\n\n");

    // emit global var decls
    for_n(i, 0, m->global->len) {
        Entity* ent = m->global->at[i];
        SemaNode* decl = ent->decl;

        switch (ent->decl->kind) {
        case SN_VAR_DECL: {
            if (ent->storage == STORAGE_COMPTIME) continue;

            c_emit_declaration(ent->mutable, ent->name, ent->type, false, false);
            if (!ent->uninit) {
                sb_append_c(sb, " = ");
                if (decl->decl.value != NULL) {
                    c_emit_simple_expr(decl->decl.value);
                } else {
                    c_emit_simple_expr_zero(ent->type);
                }
            }
            sb_append_c(sb, ";\n");
            } break;
        case SN_FN_DECL: {
            assert(ent->storage == STORAGE_FUNCTION);
            c_emit_function_prototype(ent->name, ent->type);
            sb_append_c(sb, " ");
            c_emit_stmt(decl->fn_decl.body);
            sb_append_c(sb, "\n");
            } break;
        }
    }
}

string c_body(Module* mod) {
    // type_condense();

    StringBuilder strbuilder;
    sb = &strbuilder;

    sb_init(sb);
    c_body_internal(mod);

    string s = string_alloc(sb->len);
    sb_write(sb, s.raw);
    return s;
}