#include <string.h>
#include <stdio.h>

#include "vec.h"
#include "crash.h"

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

void _vec_append(_VecGeneric* v, size_t stride, void* element) {
    if (1 + v->len > v->cap) {
        v->cap *= 2;
        v->at = realloc(v->at, v->cap * stride);
    }
    memcpy(v->at + v->len * stride, element, stride);
    ++v->len;
}

// these can easily be macros, but i want to keep all implementations in this file.
// any compiler worth its salt should be able to inline this no trouble
void _vec_clear(_VecGeneric* v) {
    v->len = 0;
}

void _vec_destroy(_VecGeneric* v) {
    *v = (_VecGeneric){0};
}