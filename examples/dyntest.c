#include <stdint.h>

typedef uint8_t* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_14ebdfb91c0)(Dyn);
typedef void (*Fun_14ebdfb9040)();

void dyntest_pass_dyn(Dyn x);
void dyntest_main();

void dyntest_pass_dyn(Dyn x) {
}

void dyntest_main() {
  int8_t v14ebdfb95c0 = (int8_t)2ll;
  int8_t l_x = v14ebdfb95c0;
  uint64_t v14ebdfb8f80 = (uint64_t)0x8b70a07b55bfd94;
  uint64_t l_ty = v14ebdfb8f80;
  int8_t v14ebdfb9480 = l_x;
  Dyn v14ebdfb8e80 = {(void*)&v14ebdfb8e80, 0x8b70a07b55bfd94};
  Fun_14ebdfb91c0 v14ebdfb9280 = dyntest_pass_dyn;
  v14ebdfb9280(v14ebdfb8e80);
}

