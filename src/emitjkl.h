#ifndef EMITJKL_H
#define EMITJKL_H

#include "mars.h"
#include "sema.h"

string jkl_header(Module* m);

void jkl_emit_typedecl(Type t, string typename);
void jkl_ensure_typedecl(Type t);

#endif // EMITJKL_H