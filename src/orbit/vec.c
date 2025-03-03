#include <string.h>

#include "vec.h"

_VecGeneric* _vec_new(size_t stride, size_t initial_cap) {
    // store the vec statically so it lives after vec_new has been called,
    // long enough for it to be copied out on the caller side
    _Thread_local static _VecGeneric temp;
    _vec_init(&temp, stride, initial_cap);
    return &temp;
}

void _vec_init(_VecGeneric* v, size_t stride, size_t initial_cap) {
    v->at = malloc(stride * initial_cap);
    v->cap = initial_cap;
    v->len = 0;
}

void _vec_reserve(_VecGeneric* v, size_t stride, size_t slots) {
    if (slots + v->len > v->cap) {
        v->cap += slots;
        v->at = realloc(v->at, v->cap * stride);
    }
}

void _vec_destroy(_VecGeneric* v) {
    free(v->at);
    *v = (_VecGeneric){0};
}
