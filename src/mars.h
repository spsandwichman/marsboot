#ifndef MARS_H
#define MARS_H

#include "orbit.h"
#include "orbit/vec.h"

typedef struct Module Module;

typedef struct SourceFile {
    string path;
    string text;
} SourceFile;

VecPtr_typedef(SourceFile);
VecPtr_typedef(Module);

typedef struct Context {
    VecPtr(SourceFile) files;
    VecPtr(Module) active_modules;
    
} Context;
extern Context ctx;

SourceFile* ctx_add_file(string path, string text);
SourceFile* ctx_find_from_substring(char* raw, usize len);

void emit_report(bool error, string source, string path, string highlight, char* message, va_list varargs);

typedef struct {
    bool check_only;
} Flags;

extern Flags flags;

#include "parse.h"

#endif
