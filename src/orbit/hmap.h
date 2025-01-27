#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#define HMap(K, V) _HMap_##K##_##V
#define HMap_typedef(K, V) typedef struct HMap(K, V) { \
    K* keys; \
    V* vals; \
    size_t cap; \
} HMap(K, V);

typedef struct _HMapGeneric {
    void* keys;
    void* vals;
    size_t cap;
} _HMapGeneric;

#define hmap_key_stride(hm) sizeof(*(hm)->keys)
#define hmap_val_stride(hm) sizeof(*(hm)->vals)

_HMapGeneric* _hmap_new(size_t key_stride, size_t val_stride, size_t initial_cap);

#define hmap_new(K, V, cap) (*(HMap(K, V)*)_hmap_new(sizeof(K), sizeof(V), cap)

#define hmap_put(hm, key)
#define hmap_get(hm, key)

#endif // MAP_h