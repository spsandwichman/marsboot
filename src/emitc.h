#ifndef EMITC_H
#define EMITC_H

#include "mars.h"
#include "sema.h"

string c_header(Module* m);

void c_emit_declaration(bool constant, string ident, Type t, bool force_full_type);

#endif // EMITC_H