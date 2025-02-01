#ifndef HASH_H
#define HASH_H

#include <stdint.h>

typedef uint64_t Hash;

Hash hash_new();
void hash_fnv1a(Hash* h, void* raw, uint64_t len);
void hash_fnv1a_u64(Hash* h, uint64_t value);

#endif // HASH_H