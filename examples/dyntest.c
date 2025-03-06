#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_55fdbb366e50)(int64_t);
typedef void (*Fun_55fdbb3670e0)(double);
typedef void (*Fun_55fdbb3673b0)(Slice);
typedef uint64_t (*Fun_55fdbb367690)(Dyn);
typedef void (*Fun_55fdbb368050)(int64_t);
typedef void (*Fun_55fdbb3687c0)(double);
typedef void (*Fun_55fdbb368f30)(Slice);

void use_int(int64_t p_i);
void use_float(double p_f);
void use_string(Slice p_f);
uint64_t dyntest_use_dyn(Dyn p_d);

uint64_t dyntest_use_dyn(Dyn p_d) {
  Dyn v55fdbb3677c0 = p_d;
  switch (v55fdbb3677c0.id) {
  case (uint64_t)0x2ae1af192b331746: case_v55fdbb3679c0:
    {
      Dyn v55fdbb367d40 = p_d;
      int64_t v55fdbb367cf0 = *(int64_t*)&v55fdbb367d40.raw;
      int64_t l_i = v55fdbb367cf0;
      int64_t v55fdbb3680e0 = l_i;
      Fun_55fdbb368050 v55fdbb368090 = use_int;
      v55fdbb368090(v55fdbb3680e0);
      v55fdbb367bb0_break:
    }
    break;
  case (uint64_t)0xdd744c18ff7ea1ab: case_v55fdbb368130:
    {
      Dyn v55fdbb3684b0 = p_d;
      double v55fdbb368460 = *(double*)&v55fdbb3684b0.raw;
      double l_f = v55fdbb368460;
      double v55fdbb368850 = l_f;
      Fun_55fdbb3687c0 v55fdbb368800 = use_float;
      v55fdbb368800(v55fdbb368850);
      v55fdbb368320_break:
    }
    break;
  case (uint64_t)0xea73c5507c8986bf: case_v55fdbb3688a0:
    {
      Dyn v55fdbb368c20 = p_d;
      Slice v55fdbb368bd0 = *(Slice*)v55fdbb368c20.raw;
      Slice l_s = v55fdbb368bd0;
      Slice v55fdbb368fc0 = l_s;
      Fun_55fdbb368f30 v55fdbb368f70 = use_string;
      v55fdbb368f70(v55fdbb368fc0);
      Slice v55fdbb369140 = l_s;
      union { Slice to; Slice from; } v55fdbb3690b0_union;
      v55fdbb3690b0_union.from = v55fdbb369140;
      Slice v55fdbb3690b0 = v55fdbb3690b0_union.to;
      Slice l_y = v55fdbb3690b0;
      int64_t v55fdbb369280 = (int64_t)1ll;
      Slice v55fdbb369230 = l_y;
      uint64_t v55fdbb3691e0 = ((uint64_t*)v55fdbb369230.raw)[v55fdbb369280];
      return v55fdbb3691e0;
      v55fdbb368a90_break:
    }
    break;
  }
  v55fdbb367770_break:
  uint64_t v55fdbb369350 = (uint64_t)0llu;
  return v55fdbb369350;
  v55fdbb3676d0_break:
}

