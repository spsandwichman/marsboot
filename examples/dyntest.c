#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_55ec8675bb60)(Dyn);
typedef void (*Fun_55ec8675be70)();

void dyntest_pass_dyn(Dyn x);
void dyntest_main();

void dyntest_pass_dyn(Dyn x) {
}

void dyntest_main() {
  int64_t v55ec8675bfd0 = (int64_t)1ll;
  int64_t x = v55ec8675bfd0;
  int64_t v55ec8675c0b0 = x;
  Dyn v55ec8675c0f0 = (Dyn)v55ec8675c0b0;
  Fun_55ec8675bb60 v55ec8675c070 = dyntest_pass_dyn;
  void v55ec8675c010 = v55ec8675c070(v55ec8675c0f0);
}

