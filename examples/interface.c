#include <stdlib.h>

void* memalloc(uintptr_t size) {
    return malloc(size);
}

void memfree(void* ptr) {
    free(ptr);
}

void* memrealloc(void* ptr, uintptr_t new_size) {
    return realloc(ptr, new_size);
}