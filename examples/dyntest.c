#include <stdint.h>

typedef uint8_t* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef void (*Fun_21bab399ca0)(Slice);
typedef void (*Fun_21bab399d20)();
typedef struct Array_21bab39a060 { Dyn _[2]; } Array_21bab39a060;

void dyntest_vararg_test(Slice args);
void dyntest_main();

void dyntest_vararg_test(Slice args) {
}

void dyntest_main() {
  int64_t v21bab39a020 = (int64_t)1ll;
  int64_t l_x = v21bab39a020;
  uint8_t v21bab399e20 = (uint8_t)1llu;
  uint8_t l_y = v21bab399e20;
  Ptr v21bab39a220 = &l_x;
  Ptr v21bab399b60 = v21bab39a220;
  Dyn v21bab39a0a0 = {(void*)*(uint64_t*)&v21bab399b60, 0x2a2aa5b4d2685af6};
  Ptr v21bab399be0 = &l_y;
  Ptr v21bab39a260 = v21bab399be0;
  Dyn v21bab399de0 = {(void*)*(uint64_t*)&v21bab39a260, 0x4a302e67672850e8};
  Array_21bab39a060 v21bab399d60 = {._={v21bab39a0a0, v21bab399de0}};
  Slice v21bab399ea0 = {((Ptr)&v21bab399d60), 2};
  Fun_21bab399ca0 v21bab399ce0 = dyntest_vararg_test;
  v21bab399ce0(v21bab399ea0);
}

