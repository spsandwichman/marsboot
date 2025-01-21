// on arch == .C, use compile with this to get some runtime stuff

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    const uint8_t* raw;
    uintptr_t len;
} slice_let_u8;

void* os_alloc(uintptr_t size) {
    return malloc(size);
}

void os_free(void* ptr) {
    free(ptr);
}

void* os_realloc(void* ptr, uintptr_t new_size) {
    return realloc(ptr, new_size);
}

void os_print(slice_let_u8 str) {
    for (uintptr_t i = 0; i < str.len; ++i) {
        putchar(str.raw[i]);
    }
}

void os_putc(uint8_t c) {
    putchar(c);
}