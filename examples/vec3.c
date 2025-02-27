#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_23e88fc15a0 {
  float x;
  float y;
  float z;
} Struct_23e88fc15a0;
typedef Struct_23e88fc15a0 (*Fun_23e88fc0e60)(Struct_23e88fc15a0);
typedef Struct_23e88fc15a0 (*Fun_23e88fc0be0)(Struct_23e88fc15a0);
typedef Struct_23e88fc15a0 (*Fun_23e88fc0ce0)(Struct_23e88fc15a0, Struct_23e88fc15a0);
typedef Struct_23e88fc15a0 (*Fun_23e88fc16a0)(Struct_23e88fc15a0, Struct_23e88fc15a0);
typedef Struct_23e88fc15a0 (*Fun_23e88fc0fe0)(Struct_23e88fc15a0, Struct_23e88fc15a0);
typedef Struct_23e88fc15a0 (*Fun_23e88fc0c60)(Struct_23e88fc15a0, Struct_23e88fc15a0);
typedef Struct_23e88fc15a0 (*Fun_23e88fc1020)(Slice);

Struct_23e88fc15a0 vec3_neg(Struct_23e88fc15a0 p_a);
Struct_23e88fc15a0 vec3_inv(Struct_23e88fc15a0 p_a);
Struct_23e88fc15a0 vec3_add(Struct_23e88fc15a0 p_a, Struct_23e88fc15a0 p_b);
Struct_23e88fc15a0 vec3_sub(Struct_23e88fc15a0 p_a, Struct_23e88fc15a0 p_b);
Struct_23e88fc15a0 vec3_mul(Struct_23e88fc15a0 p_a, Struct_23e88fc15a0 p_b);
Struct_23e88fc15a0 vec3_div(Struct_23e88fc15a0 p_a, Struct_23e88fc15a0 p_b);
Struct_23e88fc15a0 vec3_sum_all(Slice p_vecs);

Struct_23e88fc15a0 vec3_neg(Struct_23e88fc15a0 p_a) {
  Ptr v23e88fc32c0 = &p_a;
  float v23e88fc3b80 = ((Struct_23e88fc15a0*)v23e88fc32c0)->x;
  float v23e88fc3310 = -v23e88fc3b80;
  Ptr v23e88fc3f90 = &p_a;
  float v23e88fc4120 = ((Struct_23e88fc15a0*)v23e88fc3f90)->y;
  float v23e88fc4080 = -v23e88fc4120;
  Ptr v23e88fc36d0 = &p_a;
  float v23e88fc3a90 = ((Struct_23e88fc15a0*)v23e88fc36d0)->z;
  float v23e88fc3720 = -v23e88fc3a90;
  Struct_23e88fc15a0 v23e88fc3270 = {v23e88fc3310, v23e88fc4080, v23e88fc3720};
  return v23e88fc3270;
}

Struct_23e88fc15a0 vec3_inv(Struct_23e88fc15a0 p_a) {
  float v23e88fc34f0 = (float)0;
  Ptr v23e88fc3810 = &p_a;
  float v23e88fc3c70 = ((Struct_23e88fc15a0*)v23e88fc3810)->x;
  float v23e88fc33b0 = v23e88fc34f0 / v23e88fc3c70;
  float v23e88fc3540 = (float)0;
  Ptr v23e88fc3ef0 = &p_a;
  float v23e88fc3590 = ((Struct_23e88fc15a0*)v23e88fc3ef0)->y;
  float v23e88fc3e50 = v23e88fc3540 / v23e88fc3590;
  float v23e88fc3b30 = (float)0;
  Ptr v23e88fc3860 = &p_a;
  float v23e88fc3630 = ((Struct_23e88fc15a0*)v23e88fc3860)->z;
  float v23e88fc38b0 = v23e88fc3b30 / v23e88fc3630;
  Struct_23e88fc15a0 v23e88fc3950 = {v23e88fc33b0, v23e88fc3e50, v23e88fc38b0};
  return v23e88fc3950;
}

Struct_23e88fc15a0 vec3_add(Struct_23e88fc15a0 p_a, Struct_23e88fc15a0 p_b) {
  Ptr v23e88fc5040 = &p_a;
  float v23e88fc4be0 = ((Struct_23e88fc15a0*)v23e88fc5040)->x;
  Ptr v23e88fc5090 = &p_b;
  float v23e88fc5770 = ((Struct_23e88fc15a0*)v23e88fc5090)->x;
  float v23e88fc5720 = v23e88fc4be0 + v23e88fc5770;
  Ptr v23e88fc48c0 = &p_a;
  float v23e88fc4c30 = ((Struct_23e88fc15a0*)v23e88fc48c0)->y;
  Ptr v23e88fc4b40 = &p_b;
  float v23e88fc4820 = ((Struct_23e88fc15a0*)v23e88fc4b40)->y;
  float v23e88fc4af0 = v23e88fc4c30 + v23e88fc4820;
  Ptr v23e88fc5400 = &p_a;
  float v23e88fc4a50 = ((Struct_23e88fc15a0*)v23e88fc5400)->z;
  Ptr v23e88fc4cd0 = &p_b;
  float v23e88fc5220 = ((Struct_23e88fc15a0*)v23e88fc4cd0)->z;
  float v23e88fc4ff0 = v23e88fc4a50 + v23e88fc5220;
  Struct_23e88fc15a0 v23e88fc3ea0 = {v23e88fc5720, v23e88fc4af0, v23e88fc4ff0};
  return v23e88fc3ea0;
}

Struct_23e88fc15a0 vec3_sub(Struct_23e88fc15a0 p_a, Struct_23e88fc15a0 p_b) {
  Ptr v23e88fc4960 = &p_a;
  float v23e88fc5130 = ((Struct_23e88fc15a0*)v23e88fc4960)->x;
  Ptr v23e88fc5310 = &p_b;
  float v23e88fc5680 = ((Struct_23e88fc15a0*)v23e88fc5310)->x;
  float v23e88fc4d20 = v23e88fc5130 - v23e88fc5680;
  Ptr v23e88fc5540 = &p_a;
  float v23e88fc5360 = ((Struct_23e88fc15a0*)v23e88fc5540)->y;
  Ptr v23e88fc4910 = &p_b;
  float v23e88fc55e0 = ((Struct_23e88fc15a0*)v23e88fc4910)->y;
  float v23e88fc49b0 = v23e88fc5360 - v23e88fc55e0;
  Ptr v23e88fc4e10 = &p_a;
  float v23e88fc54a0 = ((Struct_23e88fc15a0*)v23e88fc4e10)->z;
  Ptr v23e88fc53b0 = &p_b;
  float v23e88fc4b90 = ((Struct_23e88fc15a0*)v23e88fc53b0)->z;
  float v23e88fc4d70 = v23e88fc54a0 - v23e88fc4b90;
  Struct_23e88fc15a0 v23e88fc4c80 = {v23e88fc4d20, v23e88fc49b0, v23e88fc4d70};
  return v23e88fc4c80;
}

Struct_23e88fc15a0 vec3_mul(Struct_23e88fc15a0 p_a, Struct_23e88fc15a0 p_b) {
  Ptr v23e88fc4f50 = &p_a;
  float v23e88fc4f00 = ((Struct_23e88fc15a0*)v23e88fc4f50)->x;
  Ptr v23e88fc7200 = &p_b;
  float v23e88fc7610 = ((Struct_23e88fc15a0*)v23e88fc7200)->x;
  float v23e88fc6da0 = v23e88fc4f00 * v23e88fc7610;
  Ptr v23e88fc74d0 = &p_a;
  float v23e88fc7070 = ((Struct_23e88fc15a0*)v23e88fc74d0)->y;
  Ptr v23e88fc73e0 = &p_b;
  float v23e88fc7520 = ((Struct_23e88fc15a0*)v23e88fc73e0)->y;
  float v23e88fc7160 = v23e88fc7070 * v23e88fc7520;
  Ptr v23e88fc6c60 = &p_a;
  float v23e88fc7570 = ((Struct_23e88fc15a0*)v23e88fc6c60)->z;
  Ptr v23e88fc7430 = &p_b;
  float v23e88fc6f30 = ((Struct_23e88fc15a0*)v23e88fc7430)->z;
  float v23e88fc6fd0 = v23e88fc7570 * v23e88fc6f30;
  Struct_23e88fc15a0 v23e88fc4eb0 = {v23e88fc6da0, v23e88fc7160, v23e88fc6fd0};
  return v23e88fc4eb0;
}

Struct_23e88fc15a0 vec3_div(Struct_23e88fc15a0 p_a, Struct_23e88fc15a0 p_b) {
  Ptr v23e88fc70c0 = &p_a;
  float v23e88fc71b0 = ((Struct_23e88fc15a0*)v23e88fc70c0)->x;
  Ptr v23e88fc6f80 = &p_b;
  float v23e88fc6d50 = ((Struct_23e88fc15a0*)v23e88fc6f80)->x;
  float v23e88fc7660 = v23e88fc71b0 / v23e88fc6d50;
  Ptr v23e88fc7340 = &p_a;
  float v23e88fc7110 = ((Struct_23e88fc15a0*)v23e88fc7340)->y;
  Ptr v23e88fc7700 = &p_b;
  float v23e88fc76b0 = ((Struct_23e88fc15a0*)v23e88fc7700)->y;
  float v23e88fc6ad0 = v23e88fc7110 / v23e88fc76b0;
  Ptr v23e88fc72a0 = &p_a;
  float v23e88fc6b70 = ((Struct_23e88fc15a0*)v23e88fc72a0)->z;
  Ptr v23e88fc7750 = &p_b;
  float v23e88fc7390 = ((Struct_23e88fc15a0*)v23e88fc7750)->z;
  float v23e88fc77a0 = v23e88fc6b70 / v23e88fc7390;
  Struct_23e88fc15a0 v23e88fc75c0 = {v23e88fc7660, v23e88fc6ad0, v23e88fc77a0};
  return v23e88fc75c0;
}

Struct_23e88fc15a0 vec3_sum_all(Slice p_vecs) {
  Struct_23e88fc15a0 v23e88fc6bc0 = (Struct_23e88fc15a0){0};
  Struct_23e88fc15a0 l_sum = v23e88fc6bc0;
  {
    uint64_t v23e88fc6ee0 = (uint64_t)0llu;
    uint64_t l_i = v23e88fc6ee0;
    while (1) {
      uint64_t v23e88fc6e90 = l_i;
      Slice v23e88fc8ae0 = p_vecs;
      uint64_t v23e88fc85e0 = v23e88fc8ae0.len;
      if (!(v23e88fc6e90 < v23e88fc85e0)) break;
      {
        Ptr v23e88fc8900 = &l_sum;
        float v23e88fc8400 = ((Struct_23e88fc15a0*)v23e88fc8900)->x;
        uint64_t v23e88fc7c30 = l_i;
        Slice v23e88fc8950 = p_vecs;
        Ptr v23e88fc7d70 = &((Struct_23e88fc15a0*)v23e88fc8950.raw)[v23e88fc7c30];
        float v23e88fc7d20 = ((Struct_23e88fc15a0*)v23e88fc7d70)->x;
        float v23e88fc7fa0 = v23e88fc8400 + v23e88fc7d20;
        Ptr v23e88fc8630 = &l_sum;
        Ptr v23e88fc8450 = &((Struct_23e88fc15a0*)v23e88fc8630)->x;
        *(float*)v23e88fc8450 = v23e88fc7fa0;
        Ptr v23e88fc7e10 = &l_sum;
        float v23e88fc8810 = ((Struct_23e88fc15a0*)v23e88fc7e10)->y;
        uint64_t v23e88fc7b90 = l_i;
        Slice v23e88fc83b0 = p_vecs;
        Ptr v23e88fc7f50 = &((Struct_23e88fc15a0*)v23e88fc83b0.raw)[v23e88fc7b90];
        float v23e88fc81d0 = ((Struct_23e88fc15a0*)v23e88fc7f50)->y;
        float v23e88fc84a0 = v23e88fc8810 + v23e88fc81d0;
        Ptr v23e88fc7dc0 = &l_sum;
        Ptr v23e88fc8040 = &((Struct_23e88fc15a0*)v23e88fc7dc0)->y;
        *(float*)v23e88fc8040 = v23e88fc84a0;
        Ptr v23e88fc7ff0 = &l_sum;
        float v23e88fc7c80 = ((Struct_23e88fc15a0*)v23e88fc7ff0)->z;
        uint64_t v23e88fc8680 = l_i;
        Slice v23e88fc89a0 = p_vecs;
        Ptr v23e88fc7eb0 = &((Struct_23e88fc15a0*)v23e88fc89a0.raw)[v23e88fc8680];
        float v23e88fc8090 = ((Struct_23e88fc15a0*)v23e88fc7eb0)->z;
        float v23e88fc8130 = v23e88fc7c80 + v23e88fc8090;
        Ptr v23e88fc8a40 = &l_sum;
        Ptr v23e88fc86d0 = &((Struct_23e88fc15a0*)v23e88fc8a40)->z;
        *(float*)v23e88fc86d0 = v23e88fc8130;
      }
      v23e88fc6c10_continue:
      ++l_i;
    }
  }
  v23e88fc6c10_break:
  Struct_23e88fc15a0 v23e88fc8720 = l_sum;
  return v23e88fc8720;
}

