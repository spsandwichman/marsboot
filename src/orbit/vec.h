#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

#define Vec(type) _Vec_##type
#define Vec_typedef(type) typedef struct Vec(type) { \
    type * at; \
    size_t len; \
    size_t cap; \
} Vec(type)

// basically just Vec(void)
typedef struct _VecGeneric {
    void * at;
    size_t len;
    size_t cap;
} _VecGeneric;

_VecGeneric* _vec_new(size_t stride, size_t initial_cap);

void _vec_init(_VecGeneric* v, size_t stride, size_t initial_cap);
void _vec_reserve(_VecGeneric* v, size_t stride, size_t slots);
void _vec_destroy(_VecGeneric* v);

#define vec_stride(v) sizeof(*(v)->at)
#define vec_new(type, initial_cap) (*(Vec(type)*) _vec_new(sizeof(type), initial_cap))

#define vec_init(v, initial_cap) _vec_init((_VecGeneric*)v, vec_stride(v), initial_cap)

#define vec_append(v, element) do { \
    _vec_reserve((_VecGeneric*)(v), vec_stride((v)), 1);\
    (v)->at[(v)->len - 1] = (element); \
} while (0)
#define vec_reserve(v, num_slots) _vec_reserve((_VecGeneric*)(v), vec_stride((v)), num_slots)

#define vec_pop(v) ((v)->at[--(v)->len])
#define vec_pop_front(v) ((v)->at[--(v)->len])

#define vec_clear(v) (v)->len = 0
#define vec_destroy(v) _vec_destroy((_VecGeneric*)v)

#define for_vec(decl, vec) \
    for (size_t _index = 0, _keep = 1; _index < (vec)->len; ++_index, _keep = !_keep) \
    for (decl = (vec)->at[_index]; _keep; _keep = !_keep)

#endif // VEC_H