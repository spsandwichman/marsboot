#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_1709f2a8ff0)(int64_t);
typedef void (*Fun_1709f2a9230)(double);
typedef void (*Fun_1709f2ad250)(Slice);
typedef void (*Fun_1709f2ada50)(Dyn);

void dyntest_use_int(int64_t p_i);
void dyntest_use_float(double p_f);
void dyntest_use_string(Slice p_f);
void dyntest_use_dyn(Dyn p_d);

void dyntest_use_int(int64_t p_i) {
}

void dyntest_use_float(double p_f) {
}

void dyntest_use_string(Slice p_f) {
}

void dyntest_use_dyn(Dyn p_d) {
  Dyn v1709f2acc20 = p_d;
  switch (v1709f2acc20.id) {
  case (uint64_t)0x2ae1af192b331746: 
    {
      Dyn v1709f2acd10 = p_d;
      int64_t v1709f2acb30 = *(int64_t*)&v1709f2acd10.raw;
      int64_t l_i = v1709f2acb30;
      int64_t v1709f2ac6d0 = l_i;
      Fun_1709f2a8ff0 v1709f2ac680 = dyntest_use_int;
      v1709f2ac680(v1709f2ac6d0);
    }
    break;
  case (uint64_t)0xdd744c18ff7ea1ab: 
    {
      Dyn v1709f2b1860 = p_d;
      double v1709f2b17c0 = *(double*)&v1709f2b1860.raw;
      double l_f = v1709f2b17c0;
      double v1709f2b1a40 = l_f;
      Fun_1709f2a9230 v1709f2b1270 = dyntest_use_float;
      v1709f2b1270(v1709f2b1a40);
    }
    break;
  case (uint64_t)0xea73c5507c8986bf: 
    {
      Dyn v1709f2b1400 = p_d;
      Slice v1709f2b1680 = *(Slice*)v1709f2b1400.raw;
      Slice l_s = v1709f2b1680;
      Slice v1709f2b1ea0 = l_s;
      Fun_1709f2ad250 v1709f2b14a0 = dyntest_use_string;
      v1709f2b14a0(v1709f2b1ea0);
      Slice v1709f2b1540 = l_s;
      union { Slice to; Slice from; } v1709f2b10e0_union;
      v1709f2b10e0_union.from = v1709f2b1540;
      Slice v1709f2b10e0 = v1709f2b10e0_union.to;
      Slice l_y = v1709f2b10e0;
    }
    break;
  }
}

