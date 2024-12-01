#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t* raw;
    uintptr_t len;
} builtin_slice_u8;

void* _host__alloc(uintptr_t size) {
    return malloc(size);
}

void _host__free(void* ptr) {
    free(ptr);
}

void* _host__realloc(void* ptr, uintptr_t new_size) {
    return realloc(ptr, new_size);
}

void _host__print(builtin_slice_u8 str) {
    for (uintptr_t i = 0; i < str.len; ++i) {
        putchar(str.raw[i]);
    }
}