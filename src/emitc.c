#include "mars.h"
#include "sema.h"
#include "strbuilder.h"

StringBuilder* sb;

void c_emit_typename(Type t) {
    switch (t) {
    case TYPE_UNKNOWN: CRASH("emitting unknown type"); break;
    case TYPE_BOOL: sb_append_c(sb, "Mars_bool"); break;
    case TYPE_DYN: sb_append_c(sb, "Mars_dyn"); break;
    case TYPE_TYPEID: sb_append_c(sb, "Mars_typeid"); break;
    case TYPE_I8: sb_append_c(sb,  "i8"); break;
    case TYPE_I16: sb_append_c(sb, "i16"); break;
    case TYPE_I32: sb_append_c(sb, "i32"); break;
    case TYPE_I64: sb_append_c(sb, "i64"); break;
    case TYPE_U8:  sb_append_c(sb, "u8"); break;
    case TYPE_U16: sb_append_c(sb, "u16"); break;
    case TYPE_U32: sb_append_c(sb, "u32"); break;
    case TYPE_U64: sb_append_c(sb, "u64"); break;
    case TYPE_F16: sb_append_c(sb, "f16"); break;
    case TYPE_F32: sb_append_c(sb, "f32"); break;
    case TYPE_F64: sb_append_c(sb, "f64"); break;
    default:
        sb_printf(sb, "Type_%d", t);
        break;
    }
}