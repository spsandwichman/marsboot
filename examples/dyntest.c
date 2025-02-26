#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_556bc5032c30)(int64_t);
typedef void (*Fun_556bc5032f90)(double);
typedef void (*Fun_556bc5033330)(Slice);
typedef void (*Fun_556bc5033690)(Dyn);

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
  Dyn v556bc50337a0 = p_d;
  switch (v556bc50337a0.id) {
  case (uint64_t)0x2ae1af192b331746: 
    {
      Dyn v556bc5033cc0 = p_d;
      int64_t v556bc5033c80 = *(int64_t*)&v556bc5033cc0.raw;
      int64_t l_i = v556bc5033c80;
      int64_t v556bc5033da0 = l_i;
      Fun_556bc5032c30 v556bc5033d60 = dyntest_use_int;
      v556bc5033d60(v556bc5033da0);
    }
    break;
  case (uint64_t)0xdd744c18ff7ea1ab: 
    {
      Dyn v556bc5034110 = p_d;
      double v556bc50340d0 = *(double*)&v556bc5034110.raw;
      double l_f = v556bc50340d0;
      double v556bc50341f0 = l_f;
      Fun_556bc5032f90 v556bc50341b0 = dyntest_use_float;
      v556bc50341b0(v556bc50341f0);
    }
    break;
  case (uint64_t)0xea73c5507c8986bf: 
    {
      Dyn v556bc5034560 = p_d;
      Slice v556bc5034520 = *(Slice*)v556bc5034560.raw;
      Slice l_s = v556bc5034520;
      Slice v556bc5034640 = l_s;
      Fun_556bc5033330 v556bc5034600 = dyntest_use_string;
      v556bc5034600(v556bc5034640);
      Slice v556bc5034790 = l_s;
      union { Slice to; Slice from; } v556bc5034710_union;
      v556bc5034710_union.from = v556bc5034790;
      Slice v556bc5034710 = v556bc5034710_union.to;
      Slice l_y = v556bc5034710;
    }
    break;
  }
}

