#ifndef MARS_H
#define MARS_H

#include "orbit.h"
#include "parse.h"

typedef struct SourceFile {
    string path;
    string text;
} SourceFile;


da_typedef(SourceFile);
typedef struct Context {
    da(SourceFile) files;
} Context;
extern Context ctx;

SourceFile ctx_add_file(string path, string text);
SourceFile* ctx_find_from_substring(char* raw, usize len);

void emit_report(bool error, string source, string path, string highlight, char* message, va_list varargs);

#endif