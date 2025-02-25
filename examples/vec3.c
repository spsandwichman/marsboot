#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_55c74195d040 {
  float x;
  float y;
  float z;
} Struct_55c74195d040;
typedef Struct_55c74195d040 (*Fun_55c74195da90)(Struct_55c74195d040);
typedef Struct_55c74195d040 (*Fun_55c74195e150)(Struct_55c74195d040);
typedef Struct_55c74195d040 (*Fun_55c74195e920)(Struct_55c74195d040, Struct_55c74195d040);
typedef Struct_55c74195d040 (*Fun_55c74195f1b0)(Struct_55c74195d040, Struct_55c74195d040);
typedef Struct_55c74195d040 (*Fun_55c74195fa40)(Struct_55c74195d040, Struct_55c74195d040);
typedef Struct_55c74195d040 (*Fun_55c7419602d0)(Struct_55c74195d040, Struct_55c74195d040);
typedef Struct_55c74195d040 (*Fun_55c741960b50)(Slice);

Struct_55c74195d040 vec3_neg(Struct_55c74195d040 p_a);
Struct_55c74195d040 vec3_inv(Struct_55c74195d040 p_a);
Struct_55c74195d040 vec3_add(Struct_55c74195d040 p_a, Struct_55c74195d040 p_b);
Struct_55c74195d040 vec3_sub(Struct_55c74195d040 p_a, Struct_55c74195d040 p_b);
Struct_55c74195d040 vec3_mul(Struct_55c74195d040 p_a, Struct_55c74195d040 p_b);
Struct_55c74195d040 vec3_div(Struct_55c74195d040 p_a, Struct_55c74195d040 p_b);
Struct_55c74195d040 vec3_sum_all(Slice p_vecs);

Struct_55c74195d040 vec3_neg(Struct_55c74195d040 p_a) {
  Ptr v55c74195dc40 = &p_a;
  float v55c74195dc00 = ((Struct_55c74195d040*)v55c74195dc40)->x;
  float v55c74195dc80 = -v55c74195dc00;
  Ptr v55c74195dd00 = &p_a;
  float v55c74195dcc0 = ((Struct_55c74195d040*)v55c74195dd00)->y;
  float v55c74195dd40 = -v55c74195dcc0;
  Ptr v55c74195ddc0 = &p_a;
  float v55c74195dd80 = ((Struct_55c74195d040*)v55c74195ddc0)->z;
  float v55c74195de00 = -v55c74195dd80;
  Struct_55c74195d040 v55c74195dba0 = {v55c74195dc80, v55c74195dd40, v55c74195de00};
  return v55c74195dba0;
}

Struct_55c74195d040 vec3_inv(Struct_55c74195d040 p_a) {
  float v55c74195e2c0 = (float)1.000000000000000000000;
  Ptr v55c74195e340 = &p_a;
  float v55c74195e300 = ((Struct_55c74195d040*)v55c74195e340)->x;
  float v55c74195e380 = v55c74195e2c0 / v55c74195e300;
  float v55c74195e3c0 = (float)1.000000000000000000000;
  Ptr v55c74195e440 = &p_a;
  float v55c74195e400 = ((Struct_55c74195d040*)v55c74195e440)->y;
  float v55c74195e480 = v55c74195e3c0 / v55c74195e400;
  float v55c74195e4c0 = (float)1.000000000000000000000;
  Ptr v55c74195e540 = &p_a;
  float v55c74195e500 = ((Struct_55c74195d040*)v55c74195e540)->z;
  float v55c74195e580 = v55c74195e4c0 / v55c74195e500;
  Struct_55c74195d040 v55c74195e260 = {v55c74195e380, v55c74195e480, v55c74195e580};
  return v55c74195e260;
}

Struct_55c74195d040 vec3_add(Struct_55c74195d040 p_a, Struct_55c74195d040 p_b) {
  Ptr v55c74195ead0 = &p_a;
  float v55c74195ea90 = ((Struct_55c74195d040*)v55c74195ead0)->x;
  Ptr v55c74195eb50 = &p_b;
  float v55c74195eb10 = ((Struct_55c74195d040*)v55c74195eb50)->x;
  float v55c74195eb90 = v55c74195ea90 + v55c74195eb10;
  Ptr v55c74195ec10 = &p_a;
  float v55c74195ebd0 = ((Struct_55c74195d040*)v55c74195ec10)->y;
  Ptr v55c74195ec90 = &p_b;
  float v55c74195ec50 = ((Struct_55c74195d040*)v55c74195ec90)->y;
  float v55c74195ecd0 = v55c74195ebd0 + v55c74195ec50;
  Ptr v55c74195ed50 = &p_a;
  float v55c74195ed10 = ((Struct_55c74195d040*)v55c74195ed50)->z;
  Ptr v55c74195edd0 = &p_b;
  float v55c74195ed90 = ((Struct_55c74195d040*)v55c74195edd0)->z;
  float v55c74195ee10 = v55c74195ed10 + v55c74195ed90;
  Struct_55c74195d040 v55c74195ea30 = {v55c74195eb90, v55c74195ecd0, v55c74195ee10};
  return v55c74195ea30;
}

Struct_55c74195d040 vec3_sub(Struct_55c74195d040 p_a, Struct_55c74195d040 p_b) {
  Ptr v55c74195f360 = &p_a;
  float v55c74195f320 = ((Struct_55c74195d040*)v55c74195f360)->x;
  Ptr v55c74195f3e0 = &p_b;
  float v55c74195f3a0 = ((Struct_55c74195d040*)v55c74195f3e0)->x;
  float v55c74195f420 = v55c74195f320 - v55c74195f3a0;
  Ptr v55c74195f4a0 = &p_a;
  float v55c74195f460 = ((Struct_55c74195d040*)v55c74195f4a0)->y;
  Ptr v55c74195f520 = &p_b;
  float v55c74195f4e0 = ((Struct_55c74195d040*)v55c74195f520)->y;
  float v55c74195f560 = v55c74195f460 - v55c74195f4e0;
  Ptr v55c74195f5e0 = &p_a;
  float v55c74195f5a0 = ((Struct_55c74195d040*)v55c74195f5e0)->z;
  Ptr v55c74195f660 = &p_b;
  float v55c74195f620 = ((Struct_55c74195d040*)v55c74195f660)->z;
  float v55c74195f6a0 = v55c74195f5a0 - v55c74195f620;
  Struct_55c74195d040 v55c74195f2c0 = {v55c74195f420, v55c74195f560, v55c74195f6a0};
  return v55c74195f2c0;
}

Struct_55c74195d040 vec3_mul(Struct_55c74195d040 p_a, Struct_55c74195d040 p_b) {
  Ptr v55c74195fbf0 = &p_a;
  float v55c74195fbb0 = ((Struct_55c74195d040*)v55c74195fbf0)->x;
  Ptr v55c74195fc70 = &p_b;
  float v55c74195fc30 = ((Struct_55c74195d040*)v55c74195fc70)->x;
  float v55c74195fcb0 = v55c74195fbb0 * v55c74195fc30;
  Ptr v55c74195fd30 = &p_a;
  float v55c74195fcf0 = ((Struct_55c74195d040*)v55c74195fd30)->y;
  Ptr v55c74195fdb0 = &p_b;
  float v55c74195fd70 = ((Struct_55c74195d040*)v55c74195fdb0)->y;
  float v55c74195fdf0 = v55c74195fcf0 * v55c74195fd70;
  Ptr v55c74195fe70 = &p_a;
  float v55c74195fe30 = ((Struct_55c74195d040*)v55c74195fe70)->z;
  Ptr v55c74195fef0 = &p_b;
  float v55c74195feb0 = ((Struct_55c74195d040*)v55c74195fef0)->z;
  float v55c74195ff30 = v55c74195fe30 * v55c74195feb0;
  Struct_55c74195d040 v55c74195fb50 = {v55c74195fcb0, v55c74195fdf0, v55c74195ff30};
  return v55c74195fb50;
}

Struct_55c74195d040 vec3_div(Struct_55c74195d040 p_a, Struct_55c74195d040 p_b) {
  Ptr v55c741960480 = &p_a;
  float v55c741960440 = ((Struct_55c74195d040*)v55c741960480)->x;
  Ptr v55c741960500 = &p_b;
  float v55c7419604c0 = ((Struct_55c74195d040*)v55c741960500)->x;
  float v55c741960540 = v55c741960440 / v55c7419604c0;
  Ptr v55c7419605c0 = &p_a;
  float v55c741960580 = ((Struct_55c74195d040*)v55c7419605c0)->y;
  Ptr v55c741960640 = &p_b;
  float v55c741960600 = ((Struct_55c74195d040*)v55c741960640)->y;
  float v55c741960680 = v55c741960580 / v55c741960600;
  Ptr v55c741960700 = &p_a;
  float v55c7419606c0 = ((Struct_55c74195d040*)v55c741960700)->z;
  Ptr v55c741960780 = &p_b;
  float v55c741960740 = ((Struct_55c74195d040*)v55c741960780)->z;
  float v55c7419607c0 = v55c7419606c0 / v55c741960740;
  Struct_55c74195d040 v55c7419603e0 = {v55c741960540, v55c741960680, v55c7419607c0};
  return v55c7419603e0;
}

Struct_55c74195d040 vec3_sum_all(Slice p_vecs) {
  Struct_55c74195d040 v55c741960cf0 = (Struct_55c74195d040){0};
  Struct_55c74195d040 l_sum = v55c741960cf0;
  {
    uint64_t v55c741960ef0 = (uint64_t)0llu;
    uint64_t l_i = v55c741960ef0;
    while (1) {
      uint64_t v55c741960ea0 = l_i;
      Slice v55c741960f70 = p_vecs;
      uint64_t v55c741960f30 = v55c741960f70.len;
      if (!(v55c741960ea0 < v55c741960f30)) break;
      {
        Ptr v55c7419610c0 = &l_sum;
        float v55c741961080 = ((Struct_55c74195d040*)v55c7419610c0)->x;
        uint64_t v55c7419611c0 = l_i;
        Slice v55c741961180 = p_vecs;
        Ptr v55c741961140 = &((Struct_55c74195d040*)v55c741961180.raw)[v55c7419611c0];
        float v55c741961100 = ((Struct_55c74195d040*)v55c741961140)->x;
        float v55c741961200 = v55c741961080 + v55c741961100;
        Ptr v55c7419612c0 = &l_sum;
        Ptr v55c741961280 = &((Struct_55c74195d040*)v55c7419612c0)->x;
        *(float*)v55c741961280 = v55c741961200;
        Ptr v55c741961340 = &l_sum;
        float v55c741961300 = ((Struct_55c74195d040*)v55c741961340)->y;
        uint64_t v55c741961440 = l_i;
        Slice v55c741961400 = p_vecs;
        Ptr v55c7419613c0 = &((Struct_55c74195d040*)v55c741961400.raw)[v55c741961440];
        float v55c741961380 = ((Struct_55c74195d040*)v55c7419613c0)->y;
        float v55c741961480 = v55c741961300 + v55c741961380;
        Ptr v55c741961540 = &l_sum;
        Ptr v55c741961500 = &((Struct_55c74195d040*)v55c741961540)->y;
        *(float*)v55c741961500 = v55c741961480;
        Ptr v55c7419615c0 = &l_sum;
        float v55c741961580 = ((Struct_55c74195d040*)v55c7419615c0)->z;
        uint64_t v55c7419616c0 = l_i;
        Slice v55c741961680 = p_vecs;
        Ptr v55c741961640 = &((Struct_55c74195d040*)v55c741961680.raw)[v55c7419616c0];
        float v55c741961600 = ((Struct_55c74195d040*)v55c741961640)->z;
        float v55c741961700 = v55c741961580 + v55c741961600;
        Ptr v55c7419617c0 = &l_sum;
        Ptr v55c741961780 = &((Struct_55c74195d040*)v55c7419617c0)->z;
        *(float*)v55c741961780 = v55c741961700;
      }
      ++l_i;
    }
  }
  Struct_55c74195d040 v55c741961840 = l_sum;
  return v55c741961840;
}

