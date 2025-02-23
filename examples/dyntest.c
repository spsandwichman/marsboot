#include <stdint.h>

typedef uint8_t* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_236724e9080)(Dyn);
typedef void (*Fun_236724e93c0)();

void dyntest_pass_dyn(Dyn x);
void dyntest_main();

void dyntest_pass_dyn(Dyn x) {
}

void dyntest_main() {
  int64_t v236724e8f40 = (int64_t)1ll;
  int64_t x = v236724e8f40;
  int64_t v236724e9180 = x;
  Dyn v236724e8fc0 = (Dyn)v236724e9180;
  Fun_236724e9080 v236724e9100 = dyntest_pass_dyn;
  void v236724e9440 = v236724e9100(v236724e8fc0);
}

