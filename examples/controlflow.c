#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_177143285f0)(_Bool);

void examples_loop_test(_Bool p_b);

void examples_loop_test(_Bool p_b) {
  while (1) {
    _Bool v1771432abc0 = p_b;
    if (!v1771432abc0) break;
    {
      while (1) {
        _Bool v1771432ae30 = p_b;
        if (!v1771432ae30) break;
        {
          goto v1771432ab70_break;
        }
        v1771432ade0_continue:
      }
      v1771432ade0_break:
    }
    v1771432ab70_continue:
  }
  v1771432ab70_break:
}

