#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_56058c9c52f0)(int64_t);
typedef void (*Fun_56058c9c5670)(double);
typedef void (*Fun_56058c9c5a30)(Slice);
typedef uint64_t (*Fun_56058c9c5db0)(Dyn);

void dyntest_use_int(int64_t p_i);
void dyntest_use_float(double p_f);
void dyntest_use_string(Slice p_f);
uint64_t dyntest_use_dyn(Dyn p_d);

void dyntest_use_int(int64_t p_i) {
  v56058c9c5330_break:
}

void dyntest_use_float(double p_f) {
  v56058c9c56b0_break:
}

void dyntest_use_string(Slice p_f) {
  v56058c9c5a70_break:
}

uint64_t dyntest_use_dyn(Dyn p_d) {
  Dyn v56058c9c5ee0 = p_d;
  switch (v56058c9c5ee0.id) {
  case (uint64_t)0x2ae1af192b331746: case_v56058c9c60e0:
    {
      Dyn v56058c9c6460 = p_d;
      int64_t v56058c9c6410 = *(int64_t*)&v56058c9c6460.raw;
      int64_t l_i = v56058c9c6410;
      int64_t v56058c9c6570 = l_i;
      Fun_56058c9c52f0 v56058c9c6520 = dyntest_use_int;
      v56058c9c6520(v56058c9c6570);
      goto v56058c9c5e90_break;
      v56058c9c62d0_break:
    }
    break;
  case (uint64_t)0xdd744c18ff7ea1ab: case_v56058c9c6610:
    {
      Dyn v56058c9c6990 = p_d;
      double v56058c9c6940 = *(double*)&v56058c9c6990.raw;
      double l_f = v56058c9c6940;
      double v56058c9c6aa0 = l_f;
      Fun_56058c9c5670 v56058c9c6a50 = dyntest_use_float;
      v56058c9c6a50(v56058c9c6aa0);
      v56058c9c6800_break:
    }
    break;
  case (uint64_t)0xea73c5507c8986bf: case_v56058c9c6af0:
    {
      Dyn v56058c9c6e70 = p_d;
      Slice v56058c9c6e20 = *(Slice*)v56058c9c6e70.raw;
      Slice l_s = v56058c9c6e20;
      Slice v56058c9c6f80 = l_s;
      Fun_56058c9c5a30 v56058c9c6f30 = dyntest_use_string;
      v56058c9c6f30(v56058c9c6f80);
      Slice v56058c9c7100 = l_s;
      union { Slice to; Slice from; } v56058c9c7070_union;
      v56058c9c7070_union.from = v56058c9c7100;
      Slice v56058c9c7070 = v56058c9c7070_union.to;
      Slice l_y = v56058c9c7070;
      int64_t v56058c9c7240 = (int64_t)1ll;
      Slice v56058c9c71f0 = l_y;
      uint64_t v56058c9c71a0 = ((uint64_t*)v56058c9c71f0.raw)[v56058c9c7240];
      return v56058c9c71a0;
      v56058c9c6ce0_break:
    }
    break;
  }
  v56058c9c5e90_break:
  uint64_t v56058c9c7310 = (uint64_t)0llu;
  return v56058c9c7310;
  v56058c9c5df0_break:
}

