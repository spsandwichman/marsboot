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
    if (id < 10) {
        sb_append_char(sb, id + '0');
        return;
    }

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

void emit_typeid(Type t) {
    sb_append_c(sb, "0x");
    emit_hex_fast(type_gen_typeid(t));
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

static void emit_mangled_smth(const char* kind, void* rand) {
    // sb_append(sb, m->name);
    // sb_append_char(sb, '_');
    sb_append_c(sb, kind);
    sb_append_char(sb, '_');
    emit_hex_fast((u64)rand);
}

static void emit_typename(TNode* t) {
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
    case TYPE_UNTYPED_INT:
    case TYPE_I64: sb_append_c(sb, "int64_t"); break;
    case TYPE_U8:  sb_append_c(sb, "uint8_t"); break;
    case TYPE_U16: sb_append_c(sb, "uint16_t"); break;
    case TYPE_U32: sb_append_c(sb, "uint32_t"); break;
    case TYPE_U64: sb_append_c(sb, "uint64_t"); break;
    case TYPE_F16: sb_append_c(sb, "_Float16"); break;
    case TYPE_F32: sb_append_c(sb, "float"); break;
    case TYPE_F64: sb_append_c(sb, "double"); break;
    case TYPE_STRUCT: emit_mangled_smth("Struct", t); break;
    case TYPE_UNION: emit_mangled_smth("Union", t); break;
    case TYPE_FUNCTION: emit_mangled_smth("Fun", t); break;
    case TYPE_SLICE: sb_append_c(sb, "Slice"); break;
    case TYPE_ARRAY: emit_mangled_smth("Array", t); break;
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE: sb_append_c(sb, "Ptr"); break;
    case TYPE_ENUM: emit_typename(type(t->as_enum.underlying)); break;
    default:
        printf("-- %d\n", t->kind);
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
    case TYPE_ENUM:
        return;
    }

    if (t->flags == TYPE_IN_PROGRESS) {
        // return some sort of predecl
        
        switch (t->kind) {
        case TYPE_STRUCT:
        case TYPE_ARRAY:
            sb_append_c(sb, "typedef struct ");
            emit_typename(t);
            sb_append_c(sb, " ");
            emit_typename(t);
            sb_append_c(sb, ";\n");
            break;
        case TYPE_UNION:
            sb_append_c(sb, "typedef union ");
            emit_typename(t);
            sb_append_c(sb, " ");
            emit_typename(t);
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
    default:
        UNREACHABLE;
    }

    sb_append_c(sb, "typedef ");
    switch (t->kind) {
    case TYPE_STRUCT:
        sb_append_c(sb, "struct ");
        emit_typename(t);
        sb_append_c(sb, " {\n");
        for_n(i, 0, t->as_record.len) {
            TypeRecordField f = t->as_record.at[i];
            sb_append_c(sb, "  ");
            emit_typename(type(f.type));
            sb_append_c(sb, " ");
            sb_append(sb, f.name);
            sb_append_c(sb, ";\n");
        }
        sb_append_c(sb, "} ");
        emit_typename(t);
        break;
    case TYPE_UNION:
        sb_append_c(sb, "union ");
        emit_typename(t);
        sb_append_c(sb, " {\n");
        for_n(i, 0, t->as_record.len) {
            TypeRecordField f = t->as_record.at[i];
            sb_append_c(sb, "    ");
            emit_typename(type(f.type));
            sb_append_c(sb, " ");
            sb_append(sb, f.name);
            sb_append_c(sb, ";\n");
        }
        sb_append_c(sb, "} ");
        emit_typename(t);
        break;
    case TYPE_ARRAY:
        sb_append_c(sb, "struct ");
        emit_typename(t);
        sb_append_c(sb, " { ");
        emit_typename(type(t->as_array.sub));
        sb_printf(sb, " _[%llu]; } ", t->as_array.len);
        emit_typename(t);
        break;
    case TYPE_SLICE:
        sb_append_c(sb, "struct ");
        emit_typename(t);
        sb_append_c(sb, " { ");
        emit_typename(type(t->as_array.sub));
        sb_printf(sb, "* data; uint64_t len; } ", t->as_array.len);
        emit_typename(t);
        break;
    case TYPE_FUNCTION:
        emit_typename(type(t->as_function.ret_type));
        sb_append_c(sb, " (*");
        emit_typename(t);
        sb_append_c(sb, ")(");
        for_n(i, 0, t->as_function.params.len) {
            if (i != 0) sb_append_c(sb, ", ");
            TypeFnParam f = t->as_function.params.at[i];
            emit_typename(type(f.type));
        }
        sb_append_c(sb, ")");
        break;
    default:
        UNREACHABLE;
    }
    sb_append_c(sb, ";\n");

    t->flags = TYPE_EMITTED;
}

static void emit_byte_hex(u8 byte) {
    static const char digits[] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    sb_append_char(sb, digits[byte >> 4]);
    sb_append_char(sb, digits[byte & 0b00001111]);
}

// thank you dasha :p
void c_emit_constval(ConstVal cv) {
    sb_append_c(sb, "(");
    emit_typename(type(cv.type));
    sb_append_c(sb, ")");

    if (type(cv.type)->kind == TYPE_ENUM) {
        cv.type = type(cv.type)->as_enum.underlying;
    }

    if (cv.is_zero) {
        if ((type(cv.type)->kind == TYPE_ARRAY && type(cv.type)->as_array.len == 0) ||
            (type(cv.type)->kind == TYPE_STRUCT && type(cv.type)->as_record.len == 0)) {
                sb_append_c(sb, "{}");
        } else {
            sb_append_c(sb, "{0}");
        }
        return;
    }
    switch (type(cv.type)->kind) {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_UNTYPED_INT:
    case TYPE_I64:
        sb_printf(sb, "%llill", cv.i64);
        break;
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
    case TYPE_POINTER:
    case TYPE_BOUNDLESS_SLICE:
        sb_printf(sb, "%llullu", cv.i64);
        break;
    case TYPE_BOOL:
        sb_append_char(sb, cv.bool ? '1' : '0');
        break;
    case TYPE_F16: sb_printf(sb, "%.*lf", (f64)cv.f16, DECIMAL_DIG); break;
    case TYPE_F32: sb_printf(sb, "%.*lf", (f64)cv.f32, DECIMAL_DIG); break;
    case TYPE_F64: sb_printf(sb, "%.*lf", (f64)cv.f64, DECIMAL_DIG); break;
    case TYPE_TYPEID:
        emit_typeid(cv.typeid);
        break;
    case TYPE_STRUCT:
    case TYPE_ARRAY:
        sb_append_c(sb, "{");
        for_n (i, 0, cv.compound.len) {
            if (i != 0) sb_append_c(sb, ", ");
            c_emit_constval(cv.compound.at[i]);
        }
        sb_append_c(sb, "}");
        break;
    default:
        UNREACHABLE;
    }
}

// god fucking dammit
void c_emit_constval_zero(Type t) {
    sb_append_c(sb, "{0}");
}

void emit_indent() {
    for_n(_, 0, indent_level) {
        sb_append_c(sb, "  ");
    }
}

void emit_expr_id(SemaNode* expr) {
    sb_append_c(sb, "v");
    emit_hex_fast((u64)expr);
}

static void decl_begin(SemaNode* expr) {
    emit_indent();
    emit_typename(type(expr->type));
    sb_append_c(sb, " ");
    emit_expr_id(expr);
    sb_append_c(sb, " = ");
}

static void ptr_decl_begin(SemaNode* expr) {
    emit_indent();
    sb_append_c(sb, "Ptr ");
    emit_expr_id(expr);
    sb_append_c(sb, " = &");
}

void c_calculate_expr(Module* m, SemaNode* expr);

void c_calculate_expr_ptr(Module* m, SemaNode* expr) {
    switch (expr->kind) {
    case SN_ENTITY:
        ptr_decl_begin(expr);

        Entity* e = expr->entity;
        switch (e->storage) {
        case STORAGE_PARAMETER:
            sb_append_c(sb, "p_");
            sb_append(sb, e->name);
            break;
        case STORAGE_LOCAL:
            sb_append_c(sb, "l_");
            sb_append(sb, e->name);
            break;
        case STORAGE_EXTERN:
        case STORAGE_EXTERN_FUNCTION:
            sb_append(sb, e->name);
            break;
        default:
            emit_mangled(m, e->name);
        }
        break;
    case SN_DEREF:
        c_calculate_expr(m, expr->unop.sub);
        emit_indent();
        sb_append_c(sb, "Ptr ");
        emit_expr_id(expr);
        sb_append_c(sb, " = ");
        emit_expr_id(expr->unop.sub); 
        break;
    case SN_ARRAY_INDEX:
        c_calculate_expr(m, expr->binop.rhs);
        c_calculate_expr_ptr(m, expr->binop.lhs);

        ptr_decl_begin(expr);

        sb_append_c(sb, "((");
        emit_typename(type(expr->type));
        sb_append_c(sb, "*)");
        emit_expr_id(expr->binop.lhs);        
        sb_append_c(sb, ")[");
        emit_expr_id(expr->binop.rhs);        
        sb_append_c(sb, "]");
        break;
    case SN_SLICE_INDEX:
        c_calculate_expr(m, expr->binop.rhs);
        c_calculate_expr(m, expr->binop.lhs);

        ptr_decl_begin(expr);

        sb_append_c(sb, "((");
        emit_typename(type(expr->type));
        sb_append_c(sb, "*)");
        emit_expr_id(expr->binop.lhs);        
        sb_append_c(sb, ".raw");
        sb_append_c(sb, ")[");
        emit_expr_id(expr->binop.rhs);        
        sb_append_c(sb, "]");
        break;
    case SN_SELECTOR:
        Type aggregate = expr->selector.selectee->type;
        if (expr->selector.through_pointer) {
            aggregate = type(aggregate)->as_ref.pointee;
            c_calculate_expr(m, expr->selector.selectee);
        } else {
            c_calculate_expr_ptr(m, expr->selector.selectee);
        }
        ptr_decl_begin(expr);

        sb_append_c(sb, "((");
        emit_typename(type(aggregate));
        sb_append_c(sb, "*)");
        emit_expr_id(expr->selector.selectee);
        sb_append_c(sb, ")->");
        sb_append(sb, type(aggregate)->as_record.at[expr->selector.field].name);
        break;
    default:
        UNREACHABLE;
    }
    sb_append_c(sb, ";\n");
}

// run this before using an expression's value.
// this will emit the c stmts needed to calculate
// this expression's value.
void c_calculate_expr(Module* m, SemaNode* expr) {
    switch (expr->kind) {
    case SN_ADD: case SN_SUB: case SN_MUL: case SN_DIV: case SN_MOD:
    case SN_LESS: case SN_LESS_EQ: case SN_GREATER: case SN_GREATER_EQ:
    case SN_EQ: case SN_NEQ:
    case SN_BIT_AND: case SN_BIT_OR: case SN_BIT_XOR: 
        c_calculate_expr(m, expr->binop.lhs);
        c_calculate_expr(m, expr->binop.rhs);

        decl_begin(expr);
        sb_append_c(sb, "(");
        emit_typename(type(expr->type));
        sb_append_c(sb, ")(");
        emit_expr_id(expr->binop.lhs);
        switch (expr->kind) {
        case SN_EQ:  sb_append_c(sb, " == "); break;
        case SN_NEQ: sb_append_c(sb, " != "); break;
        case SN_ADD: sb_append_c(sb, " + "); break;
        case SN_SUB: sb_append_c(sb, " - "); break;
        case SN_MUL: sb_append_c(sb, " * "); break;
        case SN_DIV: sb_append_c(sb, " / "); break;
        case SN_MOD: sb_append_c(sb, " % "); break;
        case SN_LESS:       sb_append_c(sb, " < "); break;
        case SN_LESS_EQ:    sb_append_c(sb, " <= "); break;
        case SN_GREATER:    sb_append_c(sb, " > "); break;
        case SN_GREATER_EQ: sb_append_c(sb, " >= "); break;
        case SN_BIT_AND: sb_append_c(sb, " & "); break;
        case SN_BIT_OR:  sb_append_c(sb, " | "); break;
        case SN_BIT_XOR: sb_append_c(sb, " ^ "); break;
        default:
            UNREACHABLE;
        }
        emit_expr_id(expr->binop.rhs);
        sb_append_c(sb, ")");
        break;
    case SN_NEG:
    case SN_BOOL_NOT:
        c_calculate_expr(m, expr->unop.sub);

        decl_begin(expr);
        sb_append_c(sb, "(");
        emit_typename(type(expr->type));
        sb_append_c(sb, ")(");
        switch (expr->kind) {
        case SN_NEG:      sb_append_c(sb, "-"); break;
        case SN_BOOL_NOT: sb_append_c(sb, "!"); break;
        default:
            UNREACHABLE;
        }
        sb_append_c(sb, ")");
        emit_expr_id(expr->unop.sub);
        break;
    case SN_BOOL_OR:
    case SN_BOOL_AND:
        /* a && b

        bool _1 = a;
        if (_1) {
            _1 = b;
        }
        */
        c_calculate_expr(m, expr->binop.lhs);
        decl_begin(expr);
        emit_expr_id(expr->binop.lhs);
        sb_append_c(sb, ";\n");

        emit_indent();
        sb_append_c(sb, "if (");
        if (expr->kind == SN_BOOL_OR) {
            sb_append_c(sb, "!");
        }
        emit_expr_id(expr->binop.lhs);
        sb_append_c(sb, ") {\n");
        indent_level++;
        c_calculate_expr(m, expr->binop.rhs);

        emit_indent();
        emit_expr_id(expr);
        sb_append_c(sb, " = ");
        emit_expr_id(expr->binop.rhs);
        sb_append_c(sb, ";\n");
        indent_level--;
        emit_indent();
        sb_append_c(sb, "}\n");

        break;
    case SN_SLICE_SELECTOR_RAW:
    case SN_SLICE_SELECTOR_LEN:
        c_calculate_expr(m, expr->selector.selectee);
        decl_begin(expr);
        if (expr->selector.through_pointer) {
            sb_append_c(sb, "((Slice*)");
            emit_expr_id(expr->selector.selectee);
            sb_append_c(sb, ")");
        } else {
            emit_expr_id(expr->selector.selectee);
        }
        sb_append_c(sb, expr->selector.through_pointer ? "->" : ".");
        sb_append_c(sb, expr->kind == SN_SLICE_SELECTOR_LEN ? "len" : "raw");
        break;
    case SN_SELECTOR:
        Type aggregate = expr->selector.selectee->type;
        if (expr->selector.through_pointer) {
            aggregate = type(aggregate)->as_ref.pointee;
            c_calculate_expr(m, expr->selector.selectee);
        } else {
            c_calculate_expr_ptr(m, expr->selector.selectee);
        }
        decl_begin(expr);

        sb_append_c(sb, "((");
        emit_typename(type(aggregate));
        sb_append_c(sb, "*)");
        emit_expr_id(expr->selector.selectee);
        sb_append_c(sb, ")->");
        sb_append(sb, type(aggregate)->as_record.at[expr->selector.field].name);
        break;
    case SN_CALL:
        for_n(i, 0, expr->call.len) {
            c_calculate_expr(m, expr->call.at[i]);
        }

        c_calculate_expr(m, expr->call.callee);
        if (expr->type != TYPE_VOID) {
            decl_begin(expr);
        } else {
            emit_indent();
        }
        emit_expr_id(expr->call.callee);
        sb_append_c(sb, "(");
        for_n (i, 0, expr->call.len) {
            if (i != 0) sb_append_c(sb, ", ");
            emit_expr_id(expr->call.at[i]);
        }
        sb_append_c(sb, ")");

        break;
    case SN_IMPLICIT_CAST:
    case SN_CAST:
        c_calculate_expr(m, expr->unop.sub);

        decl_begin(expr);
        Type from = (expr->unop.sub->type);
        Type to = (expr->type);

        // dyn boxing
        if (to == TYPE_DYN && from != TYPE_DYN) {
            isize from_size = type_calculate_size(from);
            switch (from_size) {
            case 1: sb_append_c(sb, "{(void*)*(uint8_t*)&"); break;
            case 2: sb_append_c(sb, "{(void*)*(uint16_t*)&"); break;
            case 4: sb_append_c(sb, "{(void*)*(uint32_t*)&"); break;
            case 8: sb_append_c(sb, "{(void*)*(uint64_t*)&"); break;
            default: sb_append_c(sb, "{(void*)&"); break; // box by reference
            // TODO box by value with all sizes less than 8
            }

            emit_expr_id(expr->unop.sub);
            sb_append_c(sb, ", ");
            emit_typeid(from);
            sb_append_c(sb, "}");
        } else
        // dyn unboxing
        if (to != TYPE_DYN && from == TYPE_DYN) {
            isize to_size = type_calculate_size(to);

            sb_append_c(sb, "*(");
            emit_typename(type(to));
            sb_append_c(sb, "*)");

            switch (to_size) {
            case 1: case 2: case 4: case 8: 
                sb_append_c(sb, "&"); break;
            default: 
                break; // unbox by reference
            // TODO unbox by value with all sizes less than 8
            }

            emit_expr_id(expr->unop.sub);
            sb_append_c(sb, ".raw");
        } else {
            bool require_ptr_cast = type(to)->kind == TYPE_STRUCT;
            if (require_ptr_cast) {
                sb_append_c(sb, "*(");
                emit_typename(type(expr->type));
                sb_append_c(sb, "*)&");
                emit_expr_id(expr->unop.sub);
            } else {
                sb_append_c(sb, "(");
                emit_typename(type(expr->type));
                sb_append_c(sb, ")");
                emit_expr_id(expr->unop.sub);
            }
        }
        break;
    case SN_BITCAST:
        c_calculate_expr(m, expr->unop.sub);

        // generate union
        emit_indent();
        sb_append_c(sb, "union { ");
        emit_typename(type(expr->type));
        sb_append_c(sb, " to; ");
        emit_typename(type(expr->unop.sub->type));
        sb_append_c(sb, " from; } ");
        emit_expr_id(expr);
        sb_append_c(sb, "_union;\n");

        // assign to union
        emit_indent();
        emit_expr_id(expr);
        sb_append_c(sb, "_union.from = ");
        emit_expr_id(expr->unop.sub);
        sb_append_c(sb, ";\n");

        // get from union
        decl_begin(expr);
        emit_expr_id(expr);
        sb_append_c(sb, "_union.to");
        break;
    case SN_ADDR_OF:
        c_calculate_expr_ptr(m, expr->unop.sub);
        
        decl_begin(expr);
        emit_expr_id(expr->unop.sub);
        break;
    case SN_DEREF:
        c_calculate_expr(m, expr->unop.sub);

        decl_begin(expr);
        sb_append_c(sb, "*(");
        emit_typename(type(expr->type));
        sb_append_c(sb, "*)");
        emit_expr_id(expr->binop.lhs);
        break;
    case SN_ARRAY_INDEX:
        c_calculate_expr(m, expr->binop.rhs);
        c_calculate_expr_ptr(m, expr->binop.lhs);

        decl_begin(expr);
        sb_append_c(sb, "((");
        emit_typename(type(expr->type));
        sb_append_c(sb, "*)");
        emit_expr_id(expr->binop.lhs);        
        sb_append_c(sb, ")[");
        emit_expr_id(expr->binop.rhs);        
        sb_append_c(sb, "]");
        break;
    case SN_BOUNDLESS_SLICE_INDEX:
        c_calculate_expr(m, expr->binop.rhs);
        c_calculate_expr(m, expr->binop.lhs);

        decl_begin(expr);
        sb_append_c(sb, "((");
        emit_typename(type(expr->type));
        sb_append_c(sb, "*)");
        emit_expr_id(expr->binop.lhs);        
        sb_append_c(sb, ")[");
        emit_expr_id(expr->binop.rhs);        
        sb_append_c(sb, "]");
        break;
    case SN_SLICE_INDEX:
        c_calculate_expr(m, expr->binop.rhs);
        c_calculate_expr(m, expr->binop.lhs);

        decl_begin(expr);
        sb_append_c(sb, "((");
        emit_typename(type(expr->type));
        sb_append_c(sb, "*)");
        emit_expr_id(expr->binop.lhs);        
        sb_append_c(sb, ".raw");
        sb_append_c(sb, ")[");
        emit_expr_id(expr->binop.rhs);        
        sb_append_c(sb, "]");
        break;
    case SN_ENTITY:
        decl_begin(expr);
        Entity* e = expr->entity;
        switch (e->storage) {
        case STORAGE_PARAMETER:
            sb_append_c(sb, "p_");
            sb_append(sb, e->name);
            break;
        case STORAGE_LOCAL:
            sb_append_c(sb, "l_");
            sb_append(sb, e->name);
            break;
        case STORAGE_EXTERN:
        case STORAGE_EXTERN_FUNCTION:
            sb_append(sb, e->name);
            break;
        default:
            emit_mangled(m, e->name);
        }
        break;
    case SN_CONSTVAL:
        decl_begin(expr);
        c_emit_constval(expr->constval);
        break;
    case SN_COMPOUND:
        for_n(i, 0, expr->compound.len) {
            c_calculate_expr(m, expr->compound.at[i]);
        }

        decl_begin(expr);
        sb_append_c(sb, "{");
        if (type(expr->type)->kind == TYPE_ARRAY) {
            sb_append_c(sb, "._={");
        }
        for_n(i, 0, expr->compound.len) {
            if (i != 0) sb_append_c(sb, ", ");
            emit_expr_id(expr->compound.at[i]);
        }
        if (type(expr->type)->kind == TYPE_ARRAY) {
            sb_append_c(sb, "}");
        }
        sb_append_c(sb, "}");
        break;
    case SN_SLICE_ARRAY:
        c_calculate_expr(m, expr->slice.sub);
        if (expr->slice.lower_bound) {
            c_calculate_expr(m, expr->slice.lower_bound);
        }
        if (expr->slice.upper_bound) {
            c_calculate_expr(m, expr->slice.upper_bound);
        }
        decl_begin(expr);
        sb_append_c(sb, "{((Ptr)&");
        emit_expr_id(expr->slice.sub);
        sb_append_c(sb, ")");
        if (expr->slice.lower_bound) {
            sb_append_c(sb, " + ");
            sb_printf(sb, "%llu * ", type_calculate_size(type(expr->slice.sub->type)->as_array.sub));
            emit_expr_id(expr->slice.lower_bound);
        }
        sb_append_c(sb, ", ");
        if (expr->slice.upper_bound) {
            emit_expr_id(expr->slice.upper_bound);
        } else {
            sb_printf(sb, "%llu", type(expr->slice.sub->type)->as_array.len);
        }

        sb_append_c(sb, "}");
        break;
    default:
        UNREACHABLE;
    }
    sb_append_c(sb, ";\n");
}

void c_emit_stmt(Module* m, SemaNode* stmt) {
    switch (stmt->kind) {
    case SN_DEF_DECL:
        break;
    case SN_CALL:
        c_calculate_expr(m, stmt);
        break;
    case SN_STMT_IF:
        c_calculate_expr(m, stmt->if_stmt.cond);
        emit_indent();
        sb_append_c(sb, "if (");
        emit_expr_id(stmt->if_stmt.cond);
        sb_append_c(sb, ")\n");
        if (stmt->if_stmt.if_true) {
            if (stmt->if_stmt.if_true->kind != SN_STMT_BLOCK) {
                emit_indent();
                sb_append_c(sb, "{\n");
                indent_level++;
            }
            c_emit_stmt(m, stmt->if_stmt.if_true);
            if (stmt->if_stmt.if_true->kind != SN_STMT_BLOCK) {
                indent_level--;
                emit_indent();
                sb_append_c(sb, "}\n");
            }
        }
        if (stmt->if_stmt.if_false) {
            emit_indent();
            sb_append_c(sb, "else {\n");
            indent_level++;
            c_emit_stmt(m, stmt->if_stmt.if_false);
            indent_level--;
            emit_indent();
            sb_append_c(sb, "}\n");
        }
        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_break:\n");
        break;
    case SN_STMT_WHILE:
        emit_indent();
        sb_append_c(sb, "while (1) {\n");
        indent_level++;
        
        c_calculate_expr(m, stmt->while_loop.cond);
        emit_indent();
        sb_append_c(sb, "if (!");
        emit_expr_id(stmt->while_loop.cond);
        sb_append_c(sb, ") break;\n");
        
        c_emit_stmt(m, stmt->while_loop.body);
        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_continue:\n");
        indent_level--;
        emit_indent();
        sb_append_c(sb, "}\n");
        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_break:\n");
        break;
    case SN_STMT_FOR_IN:
        emit_indent();
        sb_append_c(sb, "{\n");
        indent_level++;
        SemaNode* initial = stmt->for_range.range->binop.lhs;
        SemaNode* final = stmt->for_range.range->binop.rhs;

        c_calculate_expr(m, initial);

        Entity* iter_var = stmt->for_range.iter_var;
        
        // declare iteration variable
        emit_indent();
        emit_typename(type(iter_var->type));
        sb_append_c(sb, " ");
        sb_append_c(sb, "l_");
        sb_append(sb, iter_var->name);
        sb_append_c(sb, " = ");
        emit_expr_id(initial);
        sb_append_c(sb, ";\n");

        
        emit_indent();
        sb_append_c(sb, "while (1) {\n");
        indent_level++;

        // emit test condition
        emit_indent();
        emit_typename(type(iter_var->type));
        sb_append_c(sb, " v");
        emit_hex_fast((u64)iter_var);
        sb_append_c(sb, " = ");
        sb_append_c(sb, "l_");
        sb_append(sb, iter_var->name);
        sb_append_c(sb, ";\n");

        c_calculate_expr(m, final);

        emit_indent();
        sb_append_c(sb, "if (!(v");
        emit_hex_fast((u64)iter_var);
        sb_append_c(sb, stmt->for_range.range->kind == SN_RANGE_EQ ? " <= " : " < ");
        emit_expr_id(final);
        sb_append_c(sb, ")) break;\n");
        
        c_emit_stmt(m, stmt->for_range.body);
        
        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_continue:\n");

        emit_indent();
        sb_append_c(sb, "++");
        sb_append_c(sb, "l_");
        sb_append(sb, iter_var->name);
        sb_append_c(sb, ";\n");

        indent_level--;
        emit_indent();
        sb_append_c(sb, "}\n");
        indent_level--;
        emit_indent();
        sb_append_c(sb, "}\n");
        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_break:\n");
        break;
    case SN_STMT_FOR_CSTYLE:
        emit_indent();
        sb_append_c(sb, "{\n");
        indent_level++;
        
        c_emit_stmt(m, stmt->for_cstyle.init);
        
        emit_indent();
        sb_append_c(sb, "while (1) {\n");
        indent_level++;

        c_calculate_expr(m, stmt->for_cstyle.cond);

        emit_indent();
        sb_append_c(sb, "if (!");
        emit_expr_id(stmt->for_cstyle.cond);
        sb_append_c(sb, ") break;\n");

        c_emit_stmt(m, stmt->for_cstyle.body);

        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_continue:\n");
        c_emit_stmt(m, stmt->for_cstyle.post);
        
        indent_level--;
        emit_indent();
        sb_append_c(sb, "}\n");
        indent_level--;
        emit_indent();
        sb_append_c(sb, "}\n");
        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_break:\n");
        break;
    case SN_STMT_BREAK:
        emit_indent();
        sb_append_c(sb, "goto ");
        emit_expr_id(stmt->disrupt_cflow_stmt.label);
        sb_append_c(sb, "_break;\n");
        break;
    case SN_STMT_SWITCH:
        c_calculate_expr(m, stmt->switch_stmt.cond);
        emit_indent();
        sb_append_c(sb, "switch (");
        emit_expr_id(stmt->switch_stmt.cond);
        if (stmt->switch_stmt.cond->type == TYPE_DYN) {
            sb_append_c(sb, ".id");
        }
        sb_append_c(sb, ") {\n");

        for_n(i, 0, stmt->switch_stmt.len) {
            SemaNode* case_block = stmt->switch_stmt.cases[i];
            emit_indent();
            for_n(j, 0, case_block->case_block.len) {
                SemaNode* match = case_block->case_block.matches[j];
                sb_append_c(sb, "case ");
                c_emit_constval(match->constval);
                sb_append_c(sb, ": ");
            }
            sb_append_c(sb, "case_");
            emit_expr_id(case_block);
            sb_append_c(sb, ":\n");
            indent_level++;
            if (case_block->case_block.block->kind != SN_STMT_BLOCK) {
                sb_append_c(sb, "{\n");
            }
            c_emit_stmt(m, case_block->case_block.block);
            emit_indent();
            if (case_block->case_block.block->kind != SN_STMT_BLOCK) {
                sb_append_c(sb, "} ");
            }
            sb_append_c(sb, "break;\n");
            indent_level--;
        }

        emit_indent();
        sb_append_c(sb, "}\n");
        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_break:\n");
        break;
    case SN_STMT_BLOCK:
        emit_indent();
        indent_level++;
        sb_append_c(sb, "{\n");
        for_n(i, 0, stmt->list.len) {
            c_emit_stmt(m, stmt->list.at[i]);
        }
        emit_indent();
        emit_expr_id(stmt);
        sb_append_c(sb, "_break:\n");
        indent_level--;
        emit_indent();
        sb_append_c(sb, "}\n");
        break;
    case SN_STMT_RETURN:
        if (stmt->return_stmt.value != NULL) {
            c_calculate_expr(m, stmt->return_stmt.value);
        }
        emit_indent();
        sb_append_c(sb, "return ");
        if (stmt->return_stmt.value != NULL) {
            emit_expr_id(stmt->return_stmt.value);
        }
        sb_append_c(sb, ";\n");
        break;
    case SN_VAR_DECL:
        if (stmt->decl.value)
            c_calculate_expr(m, stmt->decl.value);
        Entity* e = stmt->decl.entity;

        emit_indent();
        emit_typename(type(e->type));
        sb_append_c(sb, " ");
        switch (e->storage) {
        case STORAGE_PARAMETER:
            sb_append_c(sb, "p_");
            sb_append(sb, e->name);
            break;
        case STORAGE_LOCAL:
            sb_append_c(sb, "l_");
            sb_append(sb, e->name);
            break;
        default:
            emit_mangled(m, e->name);
        }
        sb_append_c(sb, " = ");
        if (stmt->decl.value) {
            emit_expr_id(stmt->decl.value);
        } else {
            c_emit_constval_zero(e->type);
        }
        sb_append_c(sb, ";\n");
        break;
    case SN_STMT_ASSIGN:
        c_calculate_expr(m, stmt->assign.rhs);
        c_calculate_expr_ptr(m, stmt->assign.lhs);

        emit_indent();
        sb_append_c(sb, "*(");
        emit_typename(type(stmt->assign.lhs->type));
        sb_append_c(sb, "*)");
        emit_expr_id(stmt->assign.lhs);
        sb_append_c(sb, " = ");
        emit_expr_id(stmt->assign.rhs);
        sb_append_c(sb, ";\n");
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
        "typedef struct Slice { Ptr raw; uint64_t len; } Slice;\n"
        "typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;\n"
    );

    // unwrap distinct on everything 
    for_n (t, _TYPE_SIMPLE_END, tg.handles.len) {
        tg.handles.at[t] = type(type_unwrap_distinct(t));
    }

    // emit typegraph
    for_n (t, _TYPE_SIMPLE_END, tg.handles.len) {
        emit_typedef(m, type(t));
    }

    sb_append_c(sb, "\n");

    // emit extern variable decls
    for_n (i, 0, m->global->len) {
        Entity* e = m->global->at[i];
        if (e->storage != STORAGE_GLOBAL && e->storage != STORAGE_EXTERN) {
            continue;
        }
        sb_append_c(sb, "extern ");
        emit_typename(type(e->type));
        sb_append_c(sb, " ");
        if (e->storage == STORAGE_EXTERN) {
            sb_append(sb, e->name);
        } else {
            emit_mangled(m, e->name);
        }
        sb_append_c(sb, ";\n");
    }

    // sb_append_c(sb, "\n");

    // emit extern fn decls
    for_n (i, 0, m->global->len) {
        Entity* e = m->global->at[i];
        if (e->storage != STORAGE_FUNCTION && e->storage != STORAGE_EXTERN_FUNCTION) {
            continue;
        }
        emit_typename(type(type(e->type)->as_function.ret_type));
        sb_append_c(sb, " ");
        if (e->storage == STORAGE_EXTERN_FUNCTION) {
            sb_append(sb, e->name);
        } else {
            emit_mangled(m, e->name);
        }
        sb_append_c(sb, "(");
        for_n(i, 0, type(e->type)->as_function.params.len) {
            if (i != 0) sb_append_c(sb, ", ");
            TypeFnParam f = type(e->type)->as_function.params.at[i];
            emit_typename(type(f.type));
            sb_append_c(sb, " p_");
            sb_append(sb, f.name);
        }
        sb_append_c(sb, ");\n");
    }

    // sb_append_c(sb, "\n");

    // emit actual variable decls
    for_n (i, 0, m->global->len) {
        Entity* e = m->global->at[i];
        if (e->storage != STORAGE_GLOBAL) {
            continue;
        }
        emit_typename(type(e->type));
        sb_append_c(sb, " ");
        emit_mangled(m, e->name);
        sb_append_c(sb, " = ");
        if (e->decl->decl.value == NULL) {        
            c_emit_constval_zero(e->type);
        } else {
            c_emit_constval(e->decl->decl.value->constval);
        }
        sb_append_c(sb, ";\n");
    }

    sb_append_c(sb, "\n");

    // emit actual fn decls
    for_n (i, 0, m->global->len) {
        Entity* e = m->global->at[i];
        if (e->storage != STORAGE_FUNCTION) {
            continue;
        }
        emit_typename(type(type(e->type)->as_function.ret_type));
        sb_append_c(sb, " ");
        emit_mangled(m, e->name);
        sb_append_c(sb, "(");
        for_n(i, 0, type(e->type)->as_function.params.len) {
            if (i != 0) sb_append_c(sb, ", ");
            TypeFnParam f = type(e->type)->as_function.params.at[i];
            emit_typename(type(f.type));
            sb_append_c(sb, " p_");
            sb_append(sb, f.name);
        }
        sb_append_c(sb, ") ");
        c_emit_stmt(m, e->decl->fn_decl.body);
        sb_append_c(sb, "\n");
    }

    string s = string_alloc(sb->len);
    sb_write(sb, s.raw);
    return s;
}