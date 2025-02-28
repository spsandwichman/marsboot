#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_55c79e92e2e0)(int64_t);
typedef void (*Fun_55c79e92e660)(double);
typedef void (*Fun_55c79e92ea20)(Slice);
typedef uint64_t (*Fun_55c79e92eda0)(Dyn);

void dyntest_use_int(int64_t p_i);
void dyntest_use_float(double p_f);
void dyntest_use_string(Slice p_f);
uint64_t dyntest_use_dyn(Dyn p_d);

void dyntest_use_int(int64_t p_i) {
  v55c79e92e320_break:
}

void dyntest_use_float(double p_f) {
  v55c79e92e6a0_break:
}

void dyntest_use_string(Slice p_f) {
  v55c79e92ea60_break:
}

uint64_t dyntest_use_dyn(Dyn p_d) {
  Dyn v55c79e92eed0 = p_d;
  switch (v55c79e92eed0.id) {
  case (uint64_t)0x2ae1af192b331746: case_v55c79e92f0d0:
    {
      Dyn v55c79e92f450 = p_d;
      int64_t v55c79e92f400 = *(int64_t*)&v55c79e92f450.raw;
      int64_t l_i = v55c79e92f400;
      int64_t v55c79e92f560 = l_i;
      Fun_55c79e92e2e0 v55c79e92f510 = dyntest_use_int;
      v55c79e92f510(v55c79e92f560);
      v55c79e92f2c0_break:
    }
    break;
  case (uint64_t)0xdd744c18ff7ea1ab: case_v55c79e92f5b0:
    {
      Dyn v55c79e92f930 = p_d;
      double v55c79e92f8e0 = *(double*)&v55c79e92f930.raw;
      double l_f = v55c79e92f8e0;
      double v55c79e92fa40 = l_f;
      Fun_55c79e92e660 v55c79e92f9f0 = dyntest_use_float;
      v55c79e92f9f0(v55c79e92fa40);
      v55c79e92f7a0_break:
    }
    break;
  case (uint64_t)0xea73c5507c8986bf: case_v55c79e92fa90:
    {
      Dyn v55c79e92fe10 = p_d;
      Slice v55c79e92fdc0 = *(Slice*)v55c79e92fe10.raw;
      Slice l_s = v55c79e92fdc0;
      Slice v55c79e92ff20 = l_s;
      Fun_55c79e92ea20 v55c79e92fed0 = dyntest_use_string;
      v55c79e92fed0(v55c79e92ff20);
      Slice v55c79e9300a0 = l_s;
      union { Slice to; Slice from; } v55c79e930010_union;
      v55c79e930010_union.from = v55c79e9300a0;
      Slice v55c79e930010 = v55c79e930010_union.to;
      Slice l_y = v55c79e930010;
      int64_t v55c79e9301e0 = (int64_t)1ll;
      Slice v55c79e930190 = l_y;
      uint64_t v55c79e930140 = ((uint64_t*)v55c79e930190.raw)[v55c79e9301e0];
      return v55c79e930140;
      v55c79e92fc80_break:
    }
    break;
  }
  v55c79e92ee80_break:
  uint64_t v55c79e9302b0 = (uint64_t)0llu;
  return v55c79e9302b0;
  v55c79e92ede0_break:
}

