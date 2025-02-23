#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_2672449e4e0 {
  float x;
  float y;
  float z;
} Struct_2672449e4e0;
typedef Struct_2672449e4e0 (*Fun_267244a18e0)(Struct_2672449e4e0);
typedef Struct_2672449e4e0 (*Fun_267244a1aa0)(Struct_2672449e4e0);
typedef Struct_2672449e4e0 (*Fun_267244a1820)(Struct_2672449e4e0, Struct_2672449e4e0);
typedef Struct_2672449e4e0 (*Fun_267244a34c0)(Struct_2672449e4e0, Struct_2672449e4e0);
typedef Struct_2672449e4e0 (*Fun_267244a2900)(Struct_2672449e4e0, Struct_2672449e4e0);
typedef Struct_2672449e4e0 (*Fun_267244a2b40)(Struct_2672449e4e0, Struct_2672449e4e0);
typedef Struct_2672449e4e0 (*Fun_267244a5310)(Slice);

Struct_2672449e4e0 vec3_neg(Struct_2672449e4e0 a);
Struct_2672449e4e0 vec3_inv(Struct_2672449e4e0 a);
Struct_2672449e4e0 vec3_add(Struct_2672449e4e0 a, Struct_2672449e4e0 b);
Struct_2672449e4e0 vec3_sub(Struct_2672449e4e0 a, Struct_2672449e4e0 b);
Struct_2672449e4e0 vec3_mul(Struct_2672449e4e0 a, Struct_2672449e4e0 b);
Struct_2672449e4e0 vec3_div(Struct_2672449e4e0 a, Struct_2672449e4e0 b);
Struct_2672449e4e0 vec3_sum_all(Slice vecs);

Struct_2672449e4e0 vec3_neg(Struct_2672449e4e0 a) {
  Ptr v267244a1420 = &a;
  float v267244a12e0 = ((Struct_2672449e4e0*)v267244a1420)->x;
  float v267244a1360 = -v267244a12e0;
  Ptr v267244a1ce0 = &a;
  float v267244a1a20 = ((Struct_2672449e4e0*)v267244a1ce0)->y;
  float v267244a19a0 = -v267244a1a20;
  Ptr v267244a1320 = &a;
  float v267244a1be0 = ((Struct_2672449e4e0*)v267244a1320)->z;
  float v267244a1ca0 = -v267244a1be0;
  Struct_2672449e4e0 v267244a19e0 = {v267244a1360, v267244a19a0, v267244a1ca0};
  return v267244a19e0;
}

Struct_2672449e4e0 vec3_inv(Struct_2672449e4e0 a) {
  float v267244a11a0 = (float)1.000000;
  Ptr v267244a1b20 = &a;
  float v267244a1220 = ((Struct_2672449e4e0*)v267244a1b20)->x;
  float v267244a1160 = v267244a11a0 / v267244a1220;
  float v267244a16a0 = (float)1.000000;
  Ptr v267244a1120 = &a;
  float v267244a17a0 = ((Struct_2672449e4e0*)v267244a1120)->y;
  float v267244a15e0 = v267244a16a0 / v267244a17a0;
  float v267244a1460 = (float)1.000000;
  Ptr v267244a14a0 = &a;
  float v267244a17e0 = ((Struct_2672449e4e0*)v267244a14a0)->z;
  float v267244a1b60 = v267244a1460 / v267244a17e0;
  Struct_2672449e4e0 v267244a1c20 = {v267244a1160, v267244a15e0, v267244a1b60};
  return v267244a1c20;
}

Struct_2672449e4e0 vec3_add(Struct_2672449e4e0 a, Struct_2672449e4e0 b) {
  Ptr v267244a0f60 = &a;
  float v267244a0f20 = ((Struct_2672449e4e0*)v267244a0f60)->x;
  Ptr v267244a14e0 = &b;
  float v267244a1860 = ((Struct_2672449e4e0*)v267244a14e0)->x;
  float v267244a1960 = v267244a0f20 + v267244a1860;
  Ptr v267244a1260 = &a;
  float v267244a1520 = ((Struct_2672449e4e0*)v267244a1260)->y;
  Ptr v267244a13e0 = &b;
  float v267244a1560 = ((Struct_2672449e4e0*)v267244a13e0)->y;
  float v267244a0e60 = v267244a1520 + v267244a1560;
  Ptr v267244a0ea0 = &a;
  float v267244a15a0 = ((Struct_2672449e4e0*)v267244a0ea0)->z;
  Ptr v267244a0fe0 = &b;
  float v267244a0ee0 = ((Struct_2672449e4e0*)v267244a0fe0)->z;
  float v267244a1020 = v267244a15a0 + v267244a0ee0;
  Struct_2672449e4e0 v267244a13a0 = {v267244a1960, v267244a0e60, v267244a1020};
  return v267244a13a0;
}

Struct_2672449e4e0 vec3_sub(Struct_2672449e4e0 a, Struct_2672449e4e0 b) {
  Ptr v267244a33c0 = &a;
  float v267244a2c40 = ((Struct_2672449e4e0*)v267244a33c0)->x;
  Ptr v267244a3400 = &b;
  float v267244a2c00 = ((Struct_2672449e4e0*)v267244a3400)->x;
  float v267244a2dc0 = v267244a2c40 - v267244a2c00;
  Ptr v267244a2a80 = &a;
  float v267244a3440 = ((Struct_2672449e4e0*)v267244a2a80)->y;
  Ptr v267244a3500 = &b;
  float v267244a2cc0 = ((Struct_2672449e4e0*)v267244a3500)->y;
  float v267244a2e40 = v267244a3440 - v267244a2cc0;
  Ptr v267244a26c0 = &a;
  float v267244a2700 = ((Struct_2672449e4e0*)v267244a26c0)->z;
  Ptr v267244a2e80 = &b;
  float v267244a2740 = ((Struct_2672449e4e0*)v267244a2e80)->z;
  float v267244a2940 = v267244a2700 - v267244a2740;
  Struct_2672449e4e0 v267244a3380 = {v267244a2dc0, v267244a2e40, v267244a2940};
  return v267244a3380;
}

Struct_2672449e4e0 vec3_mul(Struct_2672449e4e0 a, Struct_2672449e4e0 b) {
  Ptr v267244a2d40 = &a;
  float v267244a2d00 = ((Struct_2672449e4e0*)v267244a2d40)->x;
  Ptr v267244a2ec0 = &b;
  float v267244a25c0 = ((Struct_2672449e4e0*)v267244a2ec0)->x;
  float v267244a2600 = v267244a2d00 * v267244a25c0;
  Ptr v267244a2a00 = &a;
  float v267244a3280 = ((Struct_2672449e4e0*)v267244a2a00)->y;
  Ptr v267244a3240 = &b;
  float v267244a2780 = ((Struct_2672449e4e0*)v267244a3240)->y;
  float v267244a3200 = v267244a3280 * v267244a2780;
  Ptr v267244a27c0 = &a;
  float v267244a2fc0 = ((Struct_2672449e4e0*)v267244a27c0)->z;
  Ptr v267244a2640 = &b;
  float v267244a2d80 = ((Struct_2672449e4e0*)v267244a2640)->z;
  float v267244a2f40 = v267244a2fc0 * v267244a2d80;
  Struct_2672449e4e0 v267244a2f00 = {v267244a2600, v267244a3200, v267244a2f40};
  return v267244a2f00;
}

Struct_2672449e4e0 vec3_div(Struct_2672449e4e0 a, Struct_2672449e4e0 b) {
  Ptr v267244a2a40 = &a;
  float v267244a2840 = ((Struct_2672449e4e0*)v267244a2a40)->x;
  Ptr v267244a2ac0 = &b;
  float v267244a28c0 = ((Struct_2672449e4e0*)v267244a2ac0)->x;
  float v267244a2b80 = v267244a2840 / v267244a28c0;
  Ptr v267244a2e00 = &a;
  float v267244a2bc0 = ((Struct_2672449e4e0*)v267244a2e00)->y;
  Ptr v267244a2f80 = &b;
  float v267244a3000 = ((Struct_2672449e4e0*)v267244a2f80)->y;
  float v267244a3040 = v267244a2bc0 / v267244a3000;
  Ptr v267244a30c0 = &a;
  float v267244a3080 = ((Struct_2672449e4e0*)v267244a30c0)->z;
  Ptr v267244a3180 = &b;
  float v267244a3100 = ((Struct_2672449e4e0*)v267244a3180)->z;
  float v267244a5090 = v267244a3080 / v267244a3100;
  Struct_2672449e4e0 v267244a2680 = {v267244a2b80, v267244a3040, v267244a5090};
  return v267244a2680;
}

Struct_2672449e4e0 vec3_sum_all(Slice vecs) {
  Struct_2672449e4e0 v267244a5150 = (Struct_2672449e4e0){0};
  Struct_2672449e4e0 sum = v267244a5150;
  {
    uint64_t v267244a5e90 = (uint64_t)0llu;
    uint64_t i = v267244a5e90;
    while (1) {
      uint64_t v267244a6b60 = i;
      Slice v267244a5e10 = vecs;
      uint64_t v267244a5590 = v267244a5e10.len;
      if (!(v267244a6b60 < v267244a5590)) break;
      {
        Ptr v267244a58d0 = &sum;
        float v267244a5890 = ((Struct_2672449e4e0*)v267244a58d0)->x;
        uint64_t v267244a5190 = i;
        Slice v267244a4fd0 = vecs;
        Ptr v267244a59d0 = &((Struct_2672449e4e0*)v267244a4fd0.raw)[v267244a5190];
        float v267244a5550 = ((Struct_2672449e4e0*)v267244a59d0)->x;
        float v267244a4f90 = v267244a5890 + v267244a5550;
        Ptr v267244a5c50 = &sum;
        Ptr v267244a5710 = &((Struct_2672449e4e0*)v267244a5c50)->x;
        *(float*)v267244a5710 = v267244a4f90;
        Ptr v267244a5010 = &sum;
        float v267244a5350 = ((Struct_2672449e4e0*)v267244a5010)->y;
        uint64_t v267244a5050 = i;
        Slice v267244a53d0 = vecs;
        Ptr v267244a5390 = &((Struct_2672449e4e0*)v267244a53d0.raw)[v267244a5050];
        float v267244a5e50 = ((Struct_2672449e4e0*)v267244a5390)->y;
        float v267244a5410 = v267244a5350 + v267244a5e50;
        Ptr v267244a5810 = &sum;
        Ptr v267244a50d0 = &((Struct_2672449e4e0*)v267244a5810)->y;
        *(float*)v267244a50d0 = v267244a5410;
        Ptr v267244a5a10 = &sum;
        float v267244a51d0 = ((Struct_2672449e4e0*)v267244a5a10)->z;
        uint64_t v267244a5250 = i;
        Slice v267244a5a50 = vecs;
        Ptr v267244a5790 = &((Struct_2672449e4e0*)v267244a5a50.raw)[v267244a5250];
        float v267244a5210 = ((Struct_2672449e4e0*)v267244a5790)->z;
        float v267244a5b10 = v267244a51d0 + v267244a5210;
        Ptr v267244a54d0 = &sum;
        Ptr v267244a5490 = &((Struct_2672449e4e0*)v267244a54d0)->z;
        *(float*)v267244a5490 = v267244a5b10;
      }
      ++i;
    }
  }
  Struct_2672449e4e0 v267244a5610 = sum;
  return v267244a5610;
}

