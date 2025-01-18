#include "mars.h"
#include "sema.h"
#include "strbuilder.h"

StringBuilder* sb;

static const char* const c_prelude = 
    "#include <stdint.h>\n"
    "\n"
    "typedef int8_t   i8;\n"
    "typedef int16_t  i16;\n"
    "typedef int32_t  i32;\n"
    "typedef int64_t  i64;\n"
    "typedef uint8_t  u8;\n"
    "typedef uint16_t u16;\n"
    "typedef uint32_t u32;\n"
    "typedef uint64_t u64;\n"
    "typedef _Float16 f16;\n"
    "typedef float    f32;\n"
    "typedef double   f64;\n"
    "\n"
    "#define false 0\n"
    "#define true  1\n"
    "typedef uint8_t bool;\n"
    "\n"
    "typedef uint64_t typeid;\n"
    "\n"
    "typedef struct {\n"
    "    void* _data_;\n"
    "    typeid _typeid_;\n"
    "} dyn;\n"
;

void c_emit_prelude() {
    sb_append_c(sb, c_prelude);
}

void c_emit_typename(Type t) {
    switch (t) {
    case TYPE_UNKNOWN: CRASH("emitting unknown type"); break;
    case TYPE_BOOL: sb_append_c(sb, "bool"); break;
    case TYPE_DYN: sb_append_c(sb, "dyn"); break;
    case TYPE_TYPEID: sb_append_c(sb, "typeid"); break;
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
        if (type_has_name(t)) {
            sb_append(sb, type_get_name(t));
        } else {
            sb_printf(sb, "_Type_%d", t);
        }
        break;
    }
}

void c_emit_typename_ptr(Type t) {
    c_emit_typename(t);
    sb_append_c(sb, "*");
}
