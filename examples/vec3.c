#include <stdint.h>

typedef uint8_t* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_199b031e920 {
  float x;
  float y;
  float z;
} Struct_199b031e920;
typedef Struct_199b031e920 (*Fun_199b0321160)(Struct_199b031e920);
typedef Struct_199b031e920 (*Fun_199b03213e0)(Struct_199b031e920);
typedef Struct_199b031e920 (*Fun_199b0320ea0)(Struct_199b031e920, Struct_199b031e920);
typedef Struct_199b031e920 (*Fun_199b0322940)(Struct_199b031e920, Struct_199b031e920);
typedef Struct_199b031e920 (*Fun_199b0323340)(Struct_199b031e920, Struct_199b031e920);
typedef Struct_199b031e920 (*Fun_199b0323080)(Struct_199b031e920, Struct_199b031e920);
typedef Struct_199b031e920 (*Fun_199b03259d0)(Slice);

Struct_199b031e920 vec3_neg(Struct_199b031e920 a);
Struct_199b031e920 vec3_inv(Struct_199b031e920 a);
Struct_199b031e920 vec3_add(Struct_199b031e920 a, Struct_199b031e920 b);
Struct_199b031e920 vec3_sub(Struct_199b031e920 a, Struct_199b031e920 b);
Struct_199b031e920 vec3_mul(Struct_199b031e920 a, Struct_199b031e920 b);
Struct_199b031e920 vec3_div(Struct_199b031e920 a, Struct_199b031e920 b);
Struct_199b031e920 vec3_sum_all(Slice vecs);

Struct_199b031e920 vec3_neg(Struct_199b031e920 a) {
  Ptr v199b0320f60 = &a;
  float v199b0320fe0 = ((Struct_199b031e920*)v199b0320f60)->x;
  float v199b0321060 = -v199b0320fe0;
  Ptr v199b03219e0 = &a;
  float v199b0321020 = ((Struct_199b031e920*)v199b03219e0)->y;
  float v199b03218a0 = -v199b0321020;
  Ptr v199b03210a0 = &a;
  float v199b0321ae0 = ((Struct_199b031e920*)v199b03210a0)->z;
  float v199b0320ee0 = -v199b0321ae0;
  Struct_199b031e920 v199b03215a0 = {v199b0321060, v199b03218a0, v199b0320ee0};
  return v199b03215a0;
}

Struct_199b031e920 vec3_inv(Struct_199b031e920 a) {
  float v199b0321860 = (float)1.000000;
  Ptr v199b03214a0 = &a;
  float v199b03215e0 = ((Struct_199b031e920*)v199b03214a0)->x;
  float v199b0321960 = v199b0321860 / v199b03215e0;
  float v199b0321a60 = (float)1.000000;
  Ptr v199b03218e0 = &a;
  float v199b0321120 = ((Struct_199b031e920*)v199b03218e0)->y;
  float v199b0321c60 = v199b0321a60 / v199b0321120;
  float v199b0321b60 = (float)1.000000;
  Ptr v199b0321ba0 = &a;
  float v199b03214e0 = ((Struct_199b031e920*)v199b0321ba0)->z;
  float v199b0321360 = v199b0321b60 / v199b03214e0;
  Struct_199b031e920 v199b0321760 = {v199b0321960, v199b0321c60, v199b0321360};
  return v199b0321760;
}

Struct_199b031e920 vec3_add(Struct_199b031e920 a, Struct_199b031e920 b) {
  Ptr v199b0321ce0 = &a;
  float v199b0320f20 = ((Struct_199b031e920*)v199b0321ce0)->x;
  Ptr v199b03217e0 = &b;
  float v199b0321520 = ((Struct_199b031e920*)v199b03217e0)->x;
  float v199b0321560 = v199b0320f20 + v199b0321520;
  Ptr v199b03211a0 = &a;
  float v199b0321d20 = ((Struct_199b031e920*)v199b03211a0)->y;
  Ptr v199b0321660 = &b;
  float v199b03211e0 = ((Struct_199b031e920*)v199b0321660)->y;
  float v199b0321d60 = v199b0321d20 + v199b03211e0;
  Ptr v199b0321260 = &a;
  float v199b0321220 = ((Struct_199b031e920*)v199b0321260)->z;
  Ptr v199b0321320 = &b;
  float v199b03212a0 = ((Struct_199b031e920*)v199b0321320)->z;
  float v199b03216a0 = v199b0321220 + v199b03212a0;
  Struct_199b031e920 v199b0321c20 = {v199b0321560, v199b0321d60, v199b03216a0};
  return v199b0321c20;
}

Struct_199b031e920 vec3_sub(Struct_199b031e920 a, Struct_199b031e920 b) {
  Ptr v199b0323400 = &a;
  float v199b0322980 = ((Struct_199b031e920*)v199b0323400)->x;
  Ptr v199b0323300 = &b;
  float v199b0322c40 = ((Struct_199b031e920*)v199b0323300)->x;
  float v199b0322b40 = v199b0322980 - v199b0322c40;
  Ptr v199b0322f80 = &a;
  float v199b0322f40 = ((Struct_199b031e920*)v199b0322f80)->y;
  Ptr v199b0322fc0 = &b;
  float v199b0322ec0 = ((Struct_199b031e920*)v199b0322fc0)->y;
  float v199b0322dc0 = v199b0322f40 - v199b0322ec0;
  Ptr v199b0323480 = &a;
  float v199b0322c80 = ((Struct_199b031e920*)v199b0323480)->z;
  Ptr v199b0322bc0 = &b;
  float v199b0322b80 = ((Struct_199b031e920*)v199b0322bc0)->z;
  float v199b0322a00 = v199b0322c80 - v199b0322b80;
  Struct_199b031e920 v199b0322b00 = {v199b0322b40, v199b0322dc0, v199b0322a00};
  return v199b0322b00;
}

Struct_199b031e920 vec3_mul(Struct_199b031e920 a, Struct_199b031e920 b) {
  Ptr v199b0322d40 = &a;
  float v199b0322800 = ((Struct_199b031e920*)v199b0322d40)->x;
  Ptr v199b0322f00 = &b;
  float v199b0322d80 = ((Struct_199b031e920*)v199b0322f00)->x;
  float v199b0322700 = v199b0322800 * v199b0322d80;
  Ptr v199b0322e40 = &a;
  float v199b0322e00 = ((Struct_199b031e920*)v199b0322e40)->y;
  Ptr v199b0322e80 = &b;
  float v199b0323200 = ((Struct_199b031e920*)v199b0322e80)->y;
  float v199b03225c0 = v199b0322e00 * v199b0323200;
  Ptr v199b0322840 = &a;
  float v199b0323380 = ((Struct_199b031e920*)v199b0322840)->z;
  Ptr v199b0323440 = &b;
  float v199b0323500 = ((Struct_199b031e920*)v199b0323440)->z;
  float v199b0322640 = v199b0323380 * v199b0323500;
  Struct_199b031e920 v199b03231c0 = {v199b0322700, v199b03225c0, v199b0322640};
  return v199b03231c0;
}

Struct_199b031e920 vec3_div(Struct_199b031e920 a, Struct_199b031e920 b) {
  Ptr v199b03228c0 = &a;
  float v199b0322740 = ((Struct_199b031e920*)v199b03228c0)->x;
  Ptr v199b0323240 = &b;
  float v199b0323140 = ((Struct_199b031e920*)v199b0323240)->x;
  float v199b0323280 = v199b0322740 / v199b0323140;
  Ptr v199b03233c0 = &a;
  float v199b03232c0 = ((Struct_199b031e920*)v199b03233c0)->y;
  Ptr v199b0322780 = &b;
  float v199b0322880 = ((Struct_199b031e920*)v199b0322780)->y;
  float v199b03227c0 = v199b03232c0 / v199b0322880;
  Ptr v199b03229c0 = &a;
  float v199b0322900 = ((Struct_199b031e920*)v199b03229c0)->z;
  Ptr v199b0322ac0 = &b;
  float v199b0322a80 = ((Struct_199b031e920*)v199b0322ac0)->z;
  float v199b0325c50 = v199b0322900 / v199b0322a80;
  Struct_199b031e920 v199b03226c0 = {v199b0323280, v199b03227c0, v199b0325c50};
  return v199b03226c0;
}

Struct_199b031e920 vec3_sum_all(Slice vecs) {
  Struct_199b031e920 v199b0325cd0 = (Struct_199b031e920){0};
  Struct_199b031e920 sum = v199b0325cd0;
  {
    uint64_t v199b0325710 = (uint64_t)0llu;
    uint64_t i = v199b0325710;
    while (1) {
      uint64_t v199b03268e0 = i;
      Slice v199b0325a50 = vecs;
      uint64_t v199b0325b50 = v199b0325a50.len;
      if (!(v199b03268e0 < v199b0325b50)) break;
      {
        Ptr v199b0325690 = &sum;
        float v199b0325e90 = ((Struct_199b031e920*)v199b0325690)->x;
        uint64_t v199b0325b90 = i;
        Slice v199b0325790 = vecs;
        Ptr v199b03258d0 = &((Struct_199b031e920*)v199b0325790.raw)[v199b0325b90];
        float v199b0325910 = ((Struct_199b031e920*)v199b03258d0)->x;
        float v199b0325450 = v199b0325e90 + v199b0325910;
        Ptr v199b0325750 = &sum;
        Ptr v199b0325bd0 = &((Struct_199b031e920*)v199b0325750)->x;
        *(float*)v199b0325bd0 = v199b0325450;
        Ptr v199b0325810 = &sum;
        float v199b0325950 = ((Struct_199b031e920*)v199b0325810)->y;
        uint64_t v199b0325d10 = i;
        Slice v199b0325850 = vecs;
        Ptr v199b0325490 = &((Struct_199b031e920*)v199b0325850.raw)[v199b0325d10];
        float v199b0325c10 = ((Struct_199b031e920*)v199b0325490)->y;
        float v199b0325890 = v199b0325950 + v199b0325c10;
        Ptr v199b0325a10 = &sum;
        Ptr v199b0325350 = &((Struct_199b031e920*)v199b0325a10)->y;
        *(float*)v199b0325350 = v199b0325890;
        Ptr v199b03256d0 = &sum;
        float v199b0325550 = ((Struct_199b031e920*)v199b03256d0)->z;
        uint64_t v199b0325ad0 = i;
        Slice v199b0325650 = vecs;
        Ptr v199b0325610 = &((Struct_199b031e920*)v199b0325650.raw)[v199b0325ad0];
        float v199b0325a90 = ((Struct_199b031e920*)v199b0325610)->z;
        float v199b0325d50 = v199b0325550 + v199b0325a90;
        Ptr v199b0325dd0 = &sum;
        Ptr v199b0325d90 = &((Struct_199b031e920*)v199b0325dd0)->z;
        *(float*)v199b0325d90 = v199b0325d50;
      }
      ++i;
    }
  }
  Struct_199b031e920 v199b0325e50 = sum;
  return v199b0325e50;
}

