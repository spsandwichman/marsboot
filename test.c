#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef int64_t (*Fun_559d041f37e0)(uint64_t);
typedef uint64_t (*Fun_559d041f3ca0)();

int64_t test_some_int(uint64_t p_x);
uint64_t test_some_uint();

int64_t test_some_int(uint64_t p_x) {
  uint64_t v559d041f3960 = p_x;
  int64_t v559d041f3910 = (int64_t)v559d041f3960;
  int64_t v559d041f39b0 = (int64_t)10ll;
  int64_t v559d041f3a00 = (int64_t)(v559d041f3910 + v559d041f39b0);
  return v559d041f3a00;
  v559d041f3820_break:
}

uint64_t test_some_uint() {
  uint64_t v559d041f3e20 = (uint64_t)10llu;
  uint64_t l_x = v559d041f3e20;
  uint64_t v559d041f3fd0 = l_x;
  Fun_559d041f37e0 v559d041f3f80 = test_some_int;
  int64_t v559d041f3f10 = v559d041f3f80(v559d041f3fd0);
  int64_t v559d041f4020 = (int64_t)1ll;
  int64_t v559d041f4070 = (int64_t)(v559d041f3f10 + v559d041f4020);
  int64_t l_y = v559d041f4070;
  uint64_t v559d041f4110 = l_x;
  uint64_t v559d041f4160 = (uint64_t)1llu;
  uint64_t v559d041f41b0 = (uint64_t)(v559d041f4110 + v559d041f4160);
  return v559d041f41b0;
  v559d041f3ce0_break:
}

