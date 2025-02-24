#include <stdint.h>

typedef uint8_t* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_55b094850a90)(int64_t);
typedef void (*Fun_55b094850df0)(double);
typedef void (*Fun_55b094851190)(Slice);
typedef void (*Fun_55b0948514f0)(Dyn);

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
  Dyn v55b094851600 = p_d;
  switch (v55b094851600.id) {
  case (uint64_t)0x2ae1af192b331746: 
    {
      Dyn v55b094851b20 = p_d;
      int64_t v55b094851ae0 = *(int64_t*)&v55b094851b20.raw;
      int64_t l_i = v55b094851ae0;
      int64_t v55b094851c00 = l_i;
      Fun_55b094850a90 v55b094851bc0 = dyntest_use_int;
      v55b094851bc0(v55b094851c00);
    }
    break;
  case (uint64_t)0xdd744c18ff7ea1ab: 
    {
      Dyn v55b094851f70 = p_d;
      double v55b094851f30 = *(double*)&v55b094851f70.raw;
      double l_f = v55b094851f30;
      double v55b094852050 = l_f;
      Fun_55b094850df0 v55b094852010 = dyntest_use_float;
      v55b094852010(v55b094852050);
    }
    break;
  case (uint64_t)0xea73c5507c8986bf: 
    {
      Dyn v55b0948523c0 = p_d;
      Slice v55b094852380 = *(Slice*)v55b0948523c0.raw;
      Slice l_s = v55b094852380;
      Slice v55b0948524a0 = l_s;
      Fun_55b094851190 v55b094852460 = dyntest_use_string;
      v55b094852460(v55b0948524a0);
    }
    break;
  }
}

