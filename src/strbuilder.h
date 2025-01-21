#ifndef STRBUILDER_H
#define STRBUILDER_H

#include "orbit.h"

typedef struct StringBuilder {
    char* buffer;
    usize len;
    usize cap;
} StringBuilder;

void sb_init(StringBuilder* sb);
void sb_destroy(StringBuilder* sb);

void sb_append(StringBuilder* sb, string s);
void sb_append_c(StringBuilder* sb, const char* s);
void sb_append_char(StringBuilder* sb, char c);
void sb_printf(StringBuilder* sb, const char* fmt, ...);

void sb_write(StringBuilder* sb, char* buffer);

#endif // STRBUILDER_H