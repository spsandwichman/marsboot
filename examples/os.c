// on arch == .C, use compile with this to get some runtime stuff

#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;

typedef struct {
    uint8_t* raw;
    uintptr_t len;
} Slice_u8;

typedef struct {
    Dyn* raw;
    uintptr_t len;
} Slice_dyn;

void* os_alloc(uintptr_t size) {
    return malloc(size);
}

void os_free(void* ptr) {
    free(ptr);
}

void* os_realloc(void* ptr, uintptr_t new_size) {
    return realloc(ptr, new_size);
}

void os_print(Slice_u8 str) {
    for (uintptr_t i = 0; i < str.len; ++i) {
        putchar((str.raw)[i]);
    }
}