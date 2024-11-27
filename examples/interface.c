#include <stdlib.h>
#include <stdint.h>

void* _host__alloc(uintptr_t size) {
    return malloc(size);
}

void _host__free(void* ptr) {
    free(ptr);
}

void* _host__realloc(void* ptr, uintptr_t new_size) {
    return realloc(ptr, new_size);
}