#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_21991aabe40 {
  int64_t data;
  Ptr next;
  Ptr prev;
} Struct_21991aabe40;
typedef void (*Fun_21991aaba00)(Ptr, Ptr);
typedef void (*Fun_21991aada30)(Ptr, Ptr);
typedef void (*Fun_21991aaddb0)(Ptr, Ptr);

void test_append_back(Ptr list, Ptr new);
void test_insert_before(Ptr node, Ptr new);
void test_insert_after(Ptr node, Ptr new);

void test_append_back(Ptr list, Ptr new) {
  Ptr v21991aabcc0 = list;
  _Bool v21991aabd00 = (_Bool)v21991aabcc0;
  _Bool v21991aabf80 = !v21991aabd00;
  _Bool v21991aac0c0 = v21991aabf80;
  if (!v21991aabf80) {
    Ptr v21991aabe00 = new;
    _Bool v21991aabc00 = (_Bool)v21991aabe00;
    _Bool v21991aabd80 = !v21991aabc00;
    v21991aac0c0 = v21991aabd80;
  }
  if (v21991aac0c0)
  {
    return ;
  }
  Ptr v21991aabf00 = list;
  Ptr end = v21991aabf00;
  while (1) {
    Ptr v21991aad270 = end;
    Ptr v21991aad970 = ((Struct_21991aabe40*)v21991aad270)->prev;
    _Bool v21991aad230 = (_Bool)v21991aad970;
    if (!v21991aad230) break;
    {
      Ptr v21991aadcb0 = end;
      Ptr v21991aad870 = ((Struct_21991aabe40*)v21991aadcb0)->prev;
      Ptr v21991aad2b0 = &end;
      *(Ptr*)v21991aad2b0 = v21991aad870;
    }
  }
  Ptr v21991aad770 = new;
  Ptr v21991aad9f0 = end;
  Ptr v21991aad330 = &((Struct_21991aabe40*)v21991aad9f0)->prev;
  *(Ptr*)v21991aad330 = v21991aad770;
  Ptr v21991aad6f0 = end;
  Ptr v21991aada70 = new;
  Ptr v21991aad7f0 = &((Struct_21991aabe40*)v21991aada70)->next;
  *(Ptr*)v21991aad7f0 = v21991aad6f0;
}

void test_insert_before(Ptr node, Ptr new) {
  Ptr v21991aad5f0 = node;
  _Bool v21991aad3f0 = (_Bool)v21991aad5f0;
  _Bool v21991aaddf0 = !v21991aad3f0;
  _Bool v21991aad6b0 = v21991aaddf0;
  if (!v21991aaddf0) {
    Ptr v21991aad670 = new;
    _Bool v21991aadaf0 = (_Bool)v21991aad670;
    _Bool v21991aad370 = !v21991aadaf0;
    v21991aad6b0 = v21991aad370;
  }
  if (v21991aad6b0)
  {
    return ;
  }
  Ptr v21991aad3b0 = node;
  Ptr v21991aad730 = ((Struct_21991aabe40*)v21991aad3b0)->prev;
  Ptr v21991aad7b0 = new;
  Ptr v21991aad8b0 = &((Struct_21991aabe40*)v21991aad7b0)->prev;
  *(Ptr*)v21991aad8b0 = v21991aad730;
  Ptr v21991aad470 = node;
  Ptr v21991aad8f0 = new;
  Ptr v21991aacf70 = &((Struct_21991aabe40*)v21991aad8f0)->next;
  *(Ptr*)v21991aacf70 = v21991aad470;
  Ptr v21991aadbb0 = new;
  Ptr v21991aad930 = node;
  Ptr v21991aad830 = ((Struct_21991aabe40*)v21991aad930)->prev;
  Ptr v21991aad0b0 = &((Struct_21991aabe40*)v21991aad830)->next;
  *(Ptr*)v21991aad0b0 = v21991aadbb0;
  Ptr v21991aadc70 = new;
  Ptr v21991aad1f0 = node;
  Ptr v21991aadbf0 = &((Struct_21991aabe40*)v21991aad1f0)->prev;
  *(Ptr*)v21991aadbf0 = v21991aadc70;
}

void test_insert_after(Ptr node, Ptr new) {
  Ptr v21991aacfb0 = node;
  _Bool v21991aacff0 = (_Bool)v21991aacfb0;
  _Bool v21991aad030 = !v21991aacff0;
  _Bool v21991aad170 = v21991aad030;
  if (!v21991aad030) {
    Ptr v21991aad070 = new;
    _Bool v21991aad0f0 = (_Bool)v21991aad070;
    _Bool v21991aad130 = !v21991aad0f0;
    v21991aad170 = v21991aad130;
  }
  if (v21991aad170)
  {
    return ;
  }
  Ptr v21991ab0a90 = node;
  Ptr v21991ab0650 = ((Struct_21991aabe40*)v21991ab0a90)->next;
  Ptr v21991ab07d0 = new;
  Ptr v21991ab05d0 = &((Struct_21991aabe40*)v21991ab07d0)->next;
  *(Ptr*)v21991ab05d0 = v21991ab0650;
  Ptr v21991ab0550 = node;
  Ptr v21991ab0f10 = new;
  Ptr v21991ab06d0 = &((Struct_21991aabe40*)v21991ab0f10)->prev;
  *(Ptr*)v21991ab06d0 = v21991ab0550;
  Ptr v21991ab0610 = new;
  Ptr v21991ab0810 = node;
  Ptr v21991ab0850 = ((Struct_21991aabe40*)v21991ab0810)->next;
  Ptr v21991ab0390 = &((Struct_21991aabe40*)v21991ab0850)->prev;
  *(Ptr*)v21991ab0390 = v21991ab0610;
  Ptr v21991ab0e50 = new;
  Ptr v21991ab0710 = node;
  Ptr v21991ab0910 = &((Struct_21991aabe40*)v21991ab0710)->next;
  *(Ptr*)v21991ab0910 = v21991ab0e50;
}

