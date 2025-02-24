#include <stdint.h>

typedef uint8_t* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_55d8098e7210)(Dyn);

void dyntest_a(Dyn p_d);

void dyntest_a(Dyn p_d) {
  Dyn v55d8098e7320 = p_d;
  switch (v55d8098e7320.id) {
  case (uint64_t)0x2ae1af192b331746: 
    {
      Dyn v55d8098e7840 = p_d;
      int64_t v55d8098e7800 = *(int64_t*)&v55d8098e7840.raw;
      int64_t l_i = v55d8098e7800;
    }
    break;
  case (uint64_t)0xdd744c18ff7ea1ab: 
    {
      Dyn v55d8098e7bb0 = p_d;
      double v55d8098e7b70 = *(double*)&v55d8098e7bb0.raw;
      double l_f = v55d8098e7b70;
    }
    break;
  }
}

