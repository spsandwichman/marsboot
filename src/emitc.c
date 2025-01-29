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

*/

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
    "    typedef _Bool    bool;\n"
    "    typedef uint64_t typeid;\n"
    "    typedef struct dyn {\n"
    "        void* _raw;\n"
    "        typeid _id;\n"
    "    } dyn;\n"
    "    #define false (bool)0\n"
    "    #define true  (bool)1\n"
    "#endif // NO_MARS_BUILTINS\n\n"
;

static void c_emit_type(Type t, bool force_full_type) {
    if (!force_full_type && type_has_name(t)) {
        sb_append(sb, type_get_name(t));
        return;
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
            c_emit_declaration(true, field->name, field->type, false);
            sb_append_c(sb, ";\n");
        }
        sb_append_c(sb, "}");
        break;
    case TYPE_UNION:
        sb_append_c(sb, "union {\n");
        for_n(i, 0, type(t)->as_record.len) {
            TypeRecordField* field = &type(t)->as_record.at[i];
            sb_append_c(sb, "    ");
            c_emit_declaration(false, field->name, field->type, false);
            sb_append_c(sb, ";\n");
        }
        sb_append_c(sb, "}");
        break;
    case TYPE_SLICE:
    case TYPE_ENUM:
    default:
        UNREACHABLE;
    }

}

static bool is_standalone_type(Type t) {
    if (t < _TYPE_SIMPLE_END || type_has_name(t)) {
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

static void c_emit_declarator(bool mutable, string ident, Type base, da(Type)* declarators) {
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
        
        if (!type(declarator)->as_ref.mutable) {
            sb_append_c(sb, "const ");
        }
        sb_append_c(sb, "(*");
        c_emit_declarator(mutable, ident, base, declarators);
        sb_append_c(sb, ")");
        break;
    case TYPE_ARRAY:
        c_emit_declarator(mutable, ident, base, declarators);
        sb_printf(sb, "[%zu]", type(declarator)->as_array.len);
        break;
    default:
        UNREACHABLE;
    }
    
}


// force_full_type is used for type declarations and stuff
void c_emit_declaration(bool mutable, string ident, Type t, bool force_full_type) {
    static da(Type) declarators;
    if (declarators.at == NULL) { // there should really be a macro for checking uninitialized lol
        da_init(&declarators, 4);
    }
    
    // find base type
    Type base = t;
    while (!is_standalone_type(base)) {
        da_append(&declarators, base);
        switch (type(base)->kind) {
        case TYPE_POINTER:
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
    c_emit_declarator(mutable, ident, base, &declarators);

    da_clear(&declarators);
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

    // mangle type names
    for_n(t, _TYPE_SIMPLE_END, tg.handles.len) {
        if (type_has_name(t)) {
            string mangled = gen_mangled(m, type_get_name(t));
            type_attach_name(t, mangled);
        } else {
            string mangled = gen_mangled_anon(m, "type", type(t));
            type_attach_name(t, mangled);
        }
    }

    // emit predecl typedefs
    
    for_n(t, _TYPE_SIMPLE_END, tg.handles.len) {
        string typename = type_get_name(t);
        sb_append_c(sb, "typedef ");
        c_emit_declaration(true, typename, t, true);
        sb_append_c(sb, ";\n\n");
    }

    sb_append_c(sb, "#endif // ");
    c_emit_header_define_symbol(m->name);
    sb_append_c(sb, "\n");
}

string c_header(Module* mod) {
    // type_condense();

    StringBuilder strbuilder;
    sb = &strbuilder;

    sb_init(sb);
    c_header_internal(mod);

    string s = string_alloc(sb->len);
    sb_write(sb, s.raw);
    return s;
}