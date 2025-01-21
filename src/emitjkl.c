#include "emitjkl.h"
#include "strbuilder.h"

static StringBuilder* sb;

// included in every module's header file.
static const char* const builtin_prelude = 
    "TYPE i8:   BYTE\n"
    "TYPE u8:  UBYTE\n"
    "TYPE i16:   INT\n"
    "TYPE u16:  UINT\n"
    "TYPE i32:  LONG\n"
    "TYPE u32: ULONG\n"
    "TYPE i64:  QUAD\n"
    "TYPE u64: UQUAD\n"
    "TYPE typeid: UWORD\n"
    "TYPE bool: UBYTE\n"
    "STRUCT dyn\n"
    "    data: ^VOID,\n"
    "    id: typeid,\n"
    "END\n"
    "\n"
;

void jkl_emit_type(Type t) {
    if (type_has_name(t)) {
        sb_append(sb, type_get_name(t));
        return;
    }

    switch (type(t)->kind) {
    case TYPE_POINTER:
        sb_append_c(sb, "^");
        jkl_emit_type(type(t)->as_ref.pointee);
        break;
    default:
        UNREACHABLE;
    }
}

// ensure this type is safe to emit
void jkl_ensure_typedecl(Type t) {
    if (t < _TYPE_SIMPLE_END || type(t)->visited) {
        return;
    }

    if (type_has_name(t)) {
        jkl_emit_typedecl(t, type_get_name(t));
        return;
    }

    if (type(t)->kind == TYPE_ARRAY) {
        jkl_ensure_typedecl(type(t)->as_array.sub);
    }
}

void jkl_emit_typedecl(Type t, string typename) {

    if (t < _TYPE_SIMPLE_END) {
        return; // primitive types already emitted lol
    }

    type(t)->visited = true;

    switch (type(t)->kind) {
    case TYPE_STRUCT:
        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField* field = &type(t)->as_record.at[i];
            jkl_ensure_typedecl(field->type);
        }

        sb_append_c(sb, "STRUCT ");
        sb_append(sb, typename);
        sb_append_c(sb, "\n");

        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField* field = &type(t)->as_record.at[i];
            sb_append_c(sb, "    ");
            sb_append(sb, field->name);
            sb_append_c(sb, ": ");
            jkl_emit_type(field->type);
            sb_append_c(sb, ",\n");
        }
        sb_append_c(sb, "END\n");

        break;
    case TYPE_DISTINCT:
        jkl_ensure_typedecl(type(t)->as_distinct);
        break;
    default:
        UNREACHABLE;
    }
}

static void jkl_header_internal(Module* m) {
    // builtin types
    sb_append_c(sb, builtin_prelude);

    // todo...
    for_n(t, _TYPE_SIMPLE_END, tg.handles.len) {
        if (type_has_name(t) && !type(t)->visited) {
            jkl_emit_typedecl(t, type_get_name(t));
        }
    }

}

string jkl_header(Module* m) {
    StringBuilder strbuilder;
    sb = &strbuilder;

    sb_init(sb);
    jkl_header_internal(m);

    string s = string_alloc(sb->len);
    sb_write(sb, s.raw);
    return s;
}