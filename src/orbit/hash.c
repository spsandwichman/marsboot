#include "hash.h"

#define OFFSET_BASIS 14695981039346656037ull
#define PRIME 1099511628211ull

Hash hash_new() {
    return OFFSET_BASIS;
}

void hash_fnv1a(Hash* h, void* raw, uint64_t len) {
    uint8_t* data = (uint8_t*) raw;
    for (uintptr_t i = 0; i < len; ++i) {
        *h ^= data[i];
        *h *= PRIME;
    }
}

void hash_fnv1a_u64(Hash* h, uint64_t value) {
    hash_fnv1a(h, &value, sizeof(value));
}