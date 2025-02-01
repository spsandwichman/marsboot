#include "hmap.h"
#include <string.h>

_HMapGeneric* _hmap_new(size_t key_stride, size_t val_stride, size_t initial_cap) {
    _Thread_local static _HMapGeneric temp;
    // _vec_init(&temp, key_stride, val_stride, initial_cap);
    return &temp;
}

void _hmap_init(_HMapGeneric* v, size_t key_stride, size_t val_stride, size_t initial_cap) {

}

size_t _hmap_index(_HMapGeneric* v, size_t key_stride, size_t val_stride, void* element) {
    return 0;
}