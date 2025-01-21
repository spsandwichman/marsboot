#ifndef PTRMAP_H
#define PTRMAP_H

// ptrmap associates a void* with another void*.

#include "orbit.h"

typedef struct PtrMap {
    void** keys;
    void** vals;
    usize cap;
} PtrMap;

#define PTRMAP_NOT_FOUND ((void*)0xDEADBEEFDEADBEEF)

usize hashfunc(void* str);

void ptrmap_init(PtrMap* hm, usize capacity);
void ptrmap_reset(PtrMap* hm);
void ptrmap_destroy(PtrMap* hm);
void ptrmap_put(PtrMap* hm, void* key, void* val);
void ptrmap_remove(PtrMap* hm, void* key);
void* ptrmap_get(PtrMap* hm, void* key);

#endif // PTRMAP_H