#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef int64_t (*Fun_558286ca75a0)(int64_t);

int64_t test_x(int64_t p_i);

int64_t test_x(int64_t p_i) {
  int64_t v558286ca76d0 = p_i;
  switch (v558286ca76d0) {
  case (int64_t)0ll: case_v558286ca78d0:
    {
      int64_t v558286ca7bb0 = (int64_t)10ll;
      return v558286ca7bb0;
      v558286ca7ac0_break:
    }
    break;
  case (int64_t)1ll: case (int64_t)2ll: case_v558286ca7c00:
    {
      int64_t v558286ca81a0 = (int64_t)11ll;
      return v558286ca81a0;
      v558286ca80b0_break:
    }
    break;
  case (int64_t)3ll: case_v558286ca81f0:
    {
      int64_t v558286ca84d0 = (int64_t)12ll;
      return v558286ca84d0;
      v558286ca83e0_break:
    }
    break;
  }
  v558286ca7680_break:
  int64_t v558286ca85c0 = (int64_t)-1ll;
  return v558286ca85c0;
  v558286ca75e0_break:
}

