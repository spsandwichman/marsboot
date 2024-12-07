// on arch == .C, use compile with this to get some runtime stuff

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    const uint8_t* raw;
    uintptr_t len;
} MARS_BUILTIN__SLICE_let_u8;

void* _os__alloc(uintptr_t size) {
    return malloc(size);
}

void _os__free(void* ptr) {
    free(ptr);
}

void* _os__realloc(void* ptr, uintptr_t new_size) {
    return realloc(ptr, new_size);
}

void _os__print(MARS_BUILTIN__SLICE_let_u8 str) {
    for (uintptr_t i = 0; i < str.len; ++i) {
        putchar(str.raw[i]);
    }
}