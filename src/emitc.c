#include "emitc.h"
#include "strbuilder.h"

/* very rudimentary C emission

    all global names get mangled as {module}_{entity}

*/

static usize indent_level = 0;
static StringBuilder* sb;

// static StringBuilder _mangled_sb;

// static string gen_mangled(Module* m, string name) {
//     if (_mangled_sb.buffer == 0) sb_init(&_mangled_sb);
//     sb_clear(&_mangled_sb);
//     sb_append(&_mangled_sb, m->name);
//     sb_append_char(&_mangled_sb, '_');
//     sb_append(&_mangled_sb, name);
//     return sb_export(&_mangled_sb);
// }

// static string gen_mangled_anon(Module* m, void* rand) {
//     if (_mangled_sb.buffer == 0) sb_init(&_mangled_sb);
//     sb_clear(&_mangled_sb);
//     sb_append_char(&_mangled_sb, '_');
//     sb_append(&_mangled_sb, m->name);
//     sb_append_char(&_mangled_sb, '_');
//     sb_printf(&_mangled_sb, "%zu", rand);
//     return sb_export(&_mangled_sb);
// }

static void emit_hex_fast(u64 id) {
    static const char charset[] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
    };
    static char buffer[16] = {};
    // looks like a lot, but the processor should actually
    // be able to run this pretty fast. none of these depend
    // on each other and the processor should be able to work
    // on these more or less at the same time
    // way better than working with printf and va_list and shit
    buffer[15] = charset[((id >> (0 *4)) & 0xFull)];
    buffer[14] = charset[((id >> (1 *4)) & 0xFull)];
    buffer[13] = charset[((id >> (2 *4)) & 0xFull)];
    buffer[12] = charset[((id >> (3 *4)) & 0xFull)];
    buffer[11] = charset[((id >> (4 *4)) & 0xFull)];
    buffer[10] = charset[((id >> (5 *4)) & 0xFull)];
    buffer[9 ] = charset[((id >> (6 *4)) & 0xFull)];
    buffer[8 ] = charset[((id >> (7 *4)) & 0xFull)];
    buffer[7 ] = charset[((id >> (8 *4)) & 0xFull)];
    buffer[6 ] = charset[((id >> (9 *4)) & 0xFull)];
    buffer[5 ] = charset[((id >> (10*4)) & 0xFull)];
    buffer[4 ] = charset[((id >> (11*4)) & 0xFull)];
    buffer[3 ] = charset[((id >> (12*4)) & 0xFull)];
    buffer[2 ] = charset[((id >> (13*4)) & 0xFull)];
    buffer[1 ] = charset[((id >> (14*4)) & 0xFull)];
    buffer[0 ] = charset[((id >> (15*4)) & 0xFull)];
    u64 trailing_zeros = __builtin_clzll(id) / 4;
    sb_append(sb, (string){.raw = buffer + trailing_zeros, .len = 16 - trailing_zeros});
}

static void emit_mangled(Module* m, string name) {
    sb_append(sb, m->name);
    sb_append_char(sb, '_');
    sb_append(sb, name);
}

static void emit_mangled_anon(Module* m, void* rand) {
    sb_append(sb, m->name);
    sb_append_char(sb, '_');
    emit_hex_fast((u64)rand);
}

static void emit_mangled_smth(Module* m, const char* kind, void* rand) {
    // sb_append(sb, m->name);
    // sb_append_char(sb, '_');
    sb_append_c(sb, kind);
    sb_append_char(sb, '_');
    emit_hex_fast((u64)rand);
}

static void emit_typename(Module* m, TNode* t) {
    while (t->kind == TYPE_DISTINCT) {
        t = type(t->as_distinct);
    }
    switch (t->kind) {
    case TYPE_VOID:   sb_append_c(sb, "void"); break;
    case TYPE_BOOL:   sb_append_c(sb, "_Bool"); break;
    case TYPE_DYN:    sb_append_c(sb, "Dyn"); break;
    case TYPE_TYPEID: sb_append_c(sb, "uint64_t"); break;
    case TYPE_I8:  sb_append_c(sb, "int8_t"); break;
    case TYPE_I16: sb_append_c(sb, "int16_t"); break;
    case TYPE_I32: sb_append_c(sb, "int32_t"); break;
    case TYPE_I64: sb_append_c(sb, "int64_t"); break;
    case TYPE_U8:  sb_append_c(sb, "uint8_t"); break;
    case TYPE_U16: sb_append_c(sb, "uint16_t"); break;
    case TYPE_U32: sb_append_c(sb, "uint32_t"); break;
    case TYPE_U64: sb_append_c(sb, "uint64_t"); break;
    case TYPE_F16: sb_append_c(sb, "_Float16"); break;
    case TYPE_F32: sb_append_c(sb, "float"); break;
    case TYPE_F64: sb_append_c(sb, "double"); break;
    case TYPE_STRUCT: emit_mangled_smth(m, "Struct", t); break;
    case TYPE_UNION: emit_mangled_smth(m, "Union", t); break;
    case TYPE_FUNCTION: emit_mangled_smth(m, "Fn", t); break;
    case TYPE_SLICE: sb_append_c(sb, "Slice");
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE: sb_append_c(sb, "Ptr"); break;
    case TYPE_ENUM: emit_typename(m, type(t->as_enum.underlying)); break;
    default:
        UNREACHABLE;
    }
}

enum {
    TYPE_UNEMITTED,
    TYPE_IN_PROGRESS,
    TYPE_PREDECLARED,
    TYPE_EMITTED,
};

void emit_typedef(Module* m, TNode* t) {
    while (t->kind == TYPE_DISTINCT) {
        t = type(t->as_distinct);
    }
    
    if (t->kind < _TYPE_SIMPLE_END || t->flags == TYPE_EMITTED || t->flags == TYPE_PREDECLARED) {
        return;
    }

    switch (t->kind) {
    case TYPE_SLICE:
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE:
        return;
    }

    if (t->flags == TYPE_IN_PROGRESS) {
        // return some sort of predecl
        
        switch (t->kind) {
        case TYPE_STRUCT:
        case TYPE_ARRAY:
            sb_append_c(sb, "typedef struct ");
            emit_typename(m, t);
            sb_append_c(sb, " ");
            emit_typename(m, t);
            sb_append_c(sb, ";\n");
            break;
        case TYPE_UNION:
            sb_append_c(sb, "typedef union ");
            emit_typename(m, t);
            sb_append_c(sb, " ");
            emit_typename(m, t);
            sb_append_c(sb, ";\n");
            break;
        }
        t->flags = TYPE_PREDECLARED;
        return;
    }

    t->flags = TYPE_IN_PROGRESS;

    // make sure everything this type needs is available
    switch (t->kind) {
    case TYPE_UNION:
    case TYPE_STRUCT:
        for_n(i, 0, t->as_record.len) {
            TypeRecordField f = t->as_record.at[i];
            emit_typedef(m, type(f.type));
        }
        break;
    case TYPE_FUNCTION:
        emit_typedef(m, type(t->as_function.ret_type));
        for_n(i, 0, t->as_function.params.len) {
            TypeFnParam f = t->as_function.params.at[i];
            emit_typedef(m, type(f.type));
        }
        break;
    case TYPE_ARRAY:
        emit_typedef(m, type(t->as_array.sub));
        break;
    case TYPE_SLICE:
        emit_typedef(m, type(t->as_array.sub));
        break;
        break;
    default:
        UNREACHABLE;
    }

    sb_append_c(sb, "typedef ");
    switch (t->kind) {
    case TYPE_STRUCT:
        sb_append_c(sb, "struct ");
        emit_typename(m, t);
        sb_append_c(sb, " {\n");
        for_n(i, 0, t->as_record.len) {
            TypeRecordField f = t->as_record.at[i];
            sb_append_c(sb, "    ");
            emit_typename(m, type(f.type));
            sb_append_c(sb, " ");
            sb_append(sb, f.name);
            sb_append_c(sb, ";\n");
        }
        sb_append_c(sb, "}");
        break;
    case TYPE_UNION:
        sb_append_c(sb, "union ");
        emit_typename(m, t);
        sb_append_c(sb, " {\n");
        for_n(i, 0, t->as_record.len) {
            TypeRecordField f = t->as_record.at[i];
            sb_append_c(sb, "    ");
            emit_typename(m, type(f.type));
            sb_append_c(sb, " ");
            sb_append(sb, f.name);
            sb_append_c(sb, ";\n");
        }
        sb_append_c(sb, "}");
        break;
    case TYPE_ARRAY:
        sb_append_c(sb, "struct ");
        emit_typename(m, t);
        sb_append_c(sb, " { ");
        emit_typename(m, type(t->as_array.sub));
        sb_printf(sb, " _[%llu]; } ", t->as_array.len);
        emit_typename(m, t);
        break;
    case TYPE_SLICE:
        sb_append_c(sb, "struct ");
        emit_typename(m, t);
        sb_append_c(sb, " { ");
        emit_typename(m, type(t->as_array.sub));
        sb_printf(sb, "* data; uint64_t len; } ", t->as_array.len);
        emit_typename(m, t);
        break;
    case TYPE_FUNCTION:
        emit_typename(m, type(t->as_function.ret_type));
        sb_append_c(sb, " ");
        emit_typename(m, t);
        sb_append_c(sb, "(");
        for_n(i, 0, t->as_function.params.len) {
            if (i != 0) sb_append_c(sb, ", ");
            TypeFnParam f = t->as_function.params.at[i];
            emit_typename(m, type(f.type));
        }
        sb_append_c(sb, ")");
        break;
    default:
        UNREACHABLE;
    }
    sb_append_c(sb, ";\n");

    t->flags = TYPE_EMITTED;
}

// thank you dasha
void c_emit_constval(SemaNode* n) {
    switch (n->constval.type) {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_UNTYPED_INT:
    case TYPE_I64: sb_printf(sb, "%lli", (i64)n->constval.i64); break;
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64: sb_printf(sb, "%llu", (i64)n->constval.i64); break;
    default:
        UNREACHABLE;
    }
}

void c_emit_constval_zero(Type t) {
    t = type_unwrap_distinct(t);

    if (type_is_numeric(t)) {
        sb_append_c(sb, "0");
        return;
    }

    switch (type(t)->kind) {
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE:
        sb_append_c(sb, "((void*)0)");
        break;
    case TYPE_UNION:
    case TYPE_STRUCT:
    case TYPE_ARRAY:
    case TYPE_SLICE:
    case TYPE_DYN:
        sb_append_c(sb, "{0}");
        break;
    default:
        UNREACHABLE;
    }
}

string c_gen(Module* m) {
    StringBuilder strbuilder;
    sb = &strbuilder;
    sb_init(sb);

    sb_append_c(sb, "#include <stdint.h>\n\n");
    
    // builtins
    sb_append_c(sb, 
        "typedef void* Ptr;\n"
        "typedef struct Slice { Ptr data; uint64_t len; } Slice;\n"
        "typedef struct Dyn { Ptr data; uint64_t id; } Dyn;\n"
    );
    // emit typegraph
    for_n (t, _TYPE_SIMPLE_END, tg.handles.len) {
        emit_typedef(m, type(t));
    }

    sb_append_c(sb, "\n");

    // emit extern variable decls
    for_n (i, 0, m->global->len) {
        Entity* e = m->global->at[i];
        if (e->storage != STORAGE_GLOBAL) {
            continue;
        }
        sb_append_c(sb, "extern ");
        emit_typename(m, type(e->type));
        sb_append_c(sb, " ");
        emit_mangled(m, e->name);
        sb_append_c(sb, ";\n");
    }

    sb_append_c(sb, "\n");

    // emit extern fn decls
    for_n (i, 0, m->global->len) {
        Entity* e = m->global->at[i];
        if (e->storage != STORAGE_FUNCTION) {
            continue;
        }
        emit_typename(m, type(type(e->type)->as_function.ret_type));
        sb_append_c(sb, " ");
        emit_mangled(m, e->name);
        sb_append_c(sb, "(");
        for_n(i, 0, type(e->type)->as_function.params.len) {
            if (i != 0) sb_append_c(sb, ", ");
            TypeFnParam f = type(e->type)->as_function.params.at[i];
            emit_typename(m, type(f.type));
            sb_append_c(sb, " ");
            sb_append(sb, f.name);
        }
        sb_append_c(sb, ")");
        sb_append_c(sb, ";\n");
    }

    sb_append_c(sb, "\n");

    // emit actual variable decls
    for_n (i, 0, m->global->len) {
        Entity* e = m->global->at[i];
        if (e->storage != STORAGE_GLOBAL) {
            continue;
        }
        emit_typename(m, type(e->type));
        sb_append_c(sb, " ");
        emit_mangled(m, e->name);
        sb_append_c(sb, " = ");
        c_emit_constval_zero(e->type);
        sb_append_c(sb, ";\n");
    }

    string s = string_alloc(sb->len);
    sb_write(sb, s.raw);
    return s;
}