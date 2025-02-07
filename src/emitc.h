#ifndef EMITC_H
#define EMITC_H

#include "mars.h"
#include "sema.h"

void c_prepare(Module* m);
string c_header(Module* m);
string c_body(Module* m);

// void c_emit_declaration(bool constant, string ident, Type t, bool force_full_type, bool is_ret_type);

void c_emit_constval(SemaNode* n);

#endif // EMITC_H