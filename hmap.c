#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_1b8391d9420 {
  Ptr keys;
  Ptr vals;
  uint64_t cap;
} Struct_1b8391d9420;
typedef uint64_t (*Fun_1b8391d89a0)(Slice);

uint64_t hmap_hash(Slice p_key);

uint64_t hmap_hash(Slice p_key) {
  uint64_t v1b8391dc100 = (uint64_t)14695981039346656037llu;
  uint64_t l_h = v1b8391dc100;
  {
    uint64_t v1b8391dc060 = (uint64_t)0llu;
    uint64_t l_i = v1b8391dc060;
    while (1) {
      uint64_t v1b8391dbf20 = l_i;
      Slice v1b8391dbfc0 = p_key;
      uint64_t v1b8391dbf70 = v1b8391dbfc0.len;
      if (!(v1b8391dbf20 < v1b8391dbf70)) break;
      {
        uint64_t v1b8391dc2e0 = l_h;
        uint64_t v1b8391dc330 = l_i;
        Slice v1b8391dc1a0 = p_key;
        uint8_t v1b8391dc240 = ((uint8_t*)v1b8391dc1a0.raw)[v1b8391dc330];
        uint64_t v1b8391dbe30 = (uint64_t)v1b8391dc240;
        uint64_t v1b8391dc1f0 = v1b8391dc2e0 ^ v1b8391dbe30;
        Ptr v1b8391dc290 = &l_h;
        *(uint64_t*)v1b8391dc290 = v1b8391dc1f0;
        uint64_t v1b8391dc420 = l_h;
        uint64_t v1b8391dbcf0 = (uint64_t)1099511628211llu;
        uint64_t v1b8391dbd40 = v1b8391dc420 * v1b8391dbcf0;
        Ptr v1b8391dbde0 = &l_h;
        *(uint64_t*)v1b8391dbde0 = v1b8391dbd40;
      }
      v1b8391dbe80_continue:
      ++l_i;
    }
  }
  v1b8391dbe80_break:
  uint64_t v1b8391dc9f0 = l_h;
  return v1b8391dc9f0;
}

