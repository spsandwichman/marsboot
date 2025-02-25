#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_55e12c0d3640)(int64_t);
typedef void (*Fun_55e12c0d39a0)(double);
typedef void (*Fun_55e12c0d3d40)(Slice);
typedef void (*Fun_55e12c0d40a0)(Dyn);

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
  Dyn v55e12c0d41b0 = p_d;
  switch (v55e12c0d41b0.id) {
  case (uint64_t)0x2ae1af192b331746: 
    {
      Dyn v55e12c0d46d0 = p_d;
      int64_t v55e12c0d4690 = *(int64_t*)&v55e12c0d46d0.raw;
      int64_t l_i = v55e12c0d4690;
      int64_t v55e12c0d47b0 = l_i;
      Fun_55e12c0d3640 v55e12c0d4770 = dyntest_use_int;
      v55e12c0d4770(v55e12c0d47b0);
    }
    break;
  case (uint64_t)0xdd744c18ff7ea1ab: 
    {
      Dyn v55e12c0d4b20 = p_d;
      double v55e12c0d4ae0 = *(double*)&v55e12c0d4b20.raw;
      double l_f = v55e12c0d4ae0;
      double v55e12c0d4c00 = l_f;
      Fun_55e12c0d39a0 v55e12c0d4bc0 = dyntest_use_float;
      v55e12c0d4bc0(v55e12c0d4c00);
    }
    break;
  case (uint64_t)0xea73c5507c8986bf: 
    {
      Dyn v55e12c0d4f70 = p_d;
      Slice v55e12c0d4f30 = *(Slice*)v55e12c0d4f70.raw;
      Slice l_s = v55e12c0d4f30;
      Slice v55e12c0d5050 = l_s;
      Fun_55e12c0d3d40 v55e12c0d5010 = dyntest_use_string;
      v55e12c0d5010(v55e12c0d5050);
      Slice v55e12c0d51a0 = l_s;
      union { Slice to; Slice from; } v55e12c0d5120_union;
      v55e12c0d5120_union.from = v55e12c0d51a0;
      Slice v55e12c0d5120 = v55e12c0d5120_union.to;
      Slice l_y = v55e12c0d5120;
    }
    break;
  }
  int64_t v55e12c0d5270 = (int64_t)1ll;
  int64_t l_x = v55e12c0d5270;
  int64_t v55e12c0d52f0 = l_x;
  switch (v55e12c0d52f0) {
  case (int64_t)1ll: case (int64_t)2ll: case (int64_t)3ll: 
    {
    }
    break;
  case (int64_t)4ll: 
    {
    }
    break;
  case (int64_t)5ll: 
    {
    }
    break;
  }
}

