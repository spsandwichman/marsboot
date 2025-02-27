#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_55de25acb760)(_Bool);

void examples_loop_test(_Bool p_b);

void examples_loop_test(_Bool p_b) {
  {
    int64_t v55de25acba60 = (int64_t)0ll;
    int64_t l_i = v55de25acba60;
    while (1) {
      int64_t v55de25acbab0 = l_i;
      int64_t v55de25acbb00 = (int64_t)10ll;
      _Bool v55de25acbb50 = v55de25acbab0 < v55de25acbb00;
      if (!v55de25acbb50) break;
      {
        goto v55de25acb840_break;
        v55de25acbd30_break:
      }
      v55de25acb840_continue:
      int64_t v55de25acbba0 = l_i;
      int64_t v55de25acbbf0 = (int64_t)1ll;
      int64_t v55de25acbc40 = v55de25acbba0 + v55de25acbbf0;
      Ptr v55de25acbce0 = &l_i;
      *(int64_t*)v55de25acbce0 = v55de25acbc40;
    }
  }
  v55de25acb840_break:
  v55de25acb7a0_break:
}

