#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_55a85f0104d0)(_Bool);

void examples_loop_test(_Bool p_b);

void examples_loop_test(_Bool p_b) {
  while (1) {
    _Bool v55a85f010600 = p_b;
    if (!v55a85f010600) break;
    {
      goto v55a85f0105b0_break;
    }
    v55a85f0105b0_continue:
  }
  v55a85f0105b0_break:
}

