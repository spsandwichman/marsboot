#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef int64_t Fn_257882db7c0(Slice, int64_t, int64_t);
typedef Ptr Fn_257882dbb00(Slice, uint64_t);
typedef int64_t Fn_257882db900(Slice);


int64_t test_store(Slice arr, int64_t index, int64_t value);
Ptr test_access(Slice array, uint64_t index);
int64_t test_sum_array(Slice array);


int64_t test_store(Slice arr, int64_t index, int64_t value) {
  int64_t v257882d93b0 = index;
  Slice v257882d98f0 = arr;
  int64_t v257882d9380 = ((int64_t*)v257882d98f0.raw)[v257882d93b0];
  int64_t previous = v257882d9380;
  int64_t v257882d9710 = value;
  int64_t v257882d9170 = index;
  Slice v257882d9350 = arr;
  Ptr v257882d92c0 = &((int64_t*)v257882d9350.raw)[v257882d9170];
  *(int64_t*)v257882d92c0 = v257882d9710;
  int64_t v257882d9620 = previous;
  return v257882d9620;
}
Ptr test_access(Slice array, uint64_t index) {
  uint64_t v257882d9440 = index;
  uint64_t v257882d9470 = (uint64_t)0llu;
  Slice v257882d97d0 = array;
  uint64_t v257882d97a0 = v257882d97d0.len;
  _Bool v257882d96e0 = v257882d9470 <= v257882d9440 && v257882d9440 < v257882d97a0;
  if (v257882d96e0)
  {
    uint64_t v257882d91d0 = index;
    Slice v257882d9830 = array;
    Ptr v257882d9260 = &((int64_t*)v257882d9830.raw)[v257882d91d0];
    Ptr v257882d9800 = v257882d9260;
    return v257882d9800;
  }
  Ptr v257882d9320 = (Ptr)0llu;
  return v257882d9320;
}
int64_t test_sum_array(Slice array) {
  int64_t v257882ddd80 = (int64_t)0ll;
  int64_t sum = v257882ddd80;
  {
    uint64_t v257882ddba0 = (uint64_t)0llu;
    uint64_t i = v257882ddba0;
    while (1) {
      uint64_t v257882dd410 = i;
      Slice v257882ddbd0 = array;
      uint64_t v257882dddb0 = v257882ddbd0.len;
      if (!(v257882dd410 < v257882dddb0)) break;
      {
        int64_t v257882dd9f0 = sum;
        uint64_t v257882dd9c0 = i;
        Slice v257882ddb10 = array;
        int64_t v257882ddc00 = ((int64_t*)v257882ddb10.raw)[v257882dd9c0];
        int64_t v257882dde10 = v257882dd9f0 + v257882ddc00;
        Ptr v257882ddde0 = &sum;
        *(int64_t*)v257882ddde0 = v257882dde10;
      }
      ++i;
    }
  }
  int64_t v257882ddea0 = sum;
  return v257882ddea0;
}
