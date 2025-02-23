#include <stdint.h>

typedef uint8_t* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_1f3ece08e80)(Dyn);
typedef void (*Fun_1f3ece09100)();

void dyntest_pass_dyn(Dyn x);
void dyntest_main();

void dyntest_pass_dyn(Dyn x) {
}

void dyntest_main() {
  int64_t v1f3ece08f80 = (int64_t)1ll;
  int64_t l_x = v1f3ece08f80;
  int64_t v1f3ece09240 = l_x;
  Dyn v1f3ece091c0 = (Dyn)v1f3ece09240;
  Fun_1f3ece08e80 v1f3ece094c0 = dyntest_pass_dyn;
  v1f3ece094c0(v1f3ece091c0);
}

