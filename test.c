#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_564177380ef0 {
  int64_t data;
  Ptr next;
  Ptr prev;
} Struct_564177380ef0;
typedef void (*Fun_5641773814c0)(Ptr, Ptr);
typedef void (*Fun_564177382500)(Ptr, Ptr);
typedef void (*Fun_564177383320)(Ptr, Ptr);

extern _Bool test_x;
void test_append_back(Ptr p_list, Ptr p_new);
void test_insert_before(Ptr p_node, Ptr p_new);
void test_insert_after(Ptr p_node, Ptr p_new);
_Bool test_x = (_Bool)1;

void test_append_back(Ptr p_list, Ptr p_new) {
  Ptr v5641773815f0 = p_list;
  _Bool v564177381640 = (_Bool)v5641773815f0;
  _Bool v564177381690 = !v564177381640;
  _Bool v5641773817d0 = v564177381690;
  if (!v564177381690) {
    Ptr v5641773816e0 = p_new;
    _Bool v564177381730 = (_Bool)v5641773816e0;
    _Bool v564177381780 = !v564177381730;
    v5641773817d0 = v564177381780;
  }
;
  if (v5641773817d0)
  {
    return ;
  }
  v5641773815a0_break:
  Ptr v564177381a40 = p_list;
  Ptr l_end = v564177381a40;
  while (1) {
    Ptr v564177381b30 = l_end;
    Ptr v564177381ae0 = ((Struct_564177380ef0*)v564177381b30)->next;
    _Bool v564177381b80 = (_Bool)v564177381ae0;
    if (!v564177381b80) break;
    {
      Ptr v564177381e90 = l_end;
      Ptr v564177381e40 = ((Struct_564177380ef0*)v564177381e90)->next;
      Ptr v564177381df0 = &l_end;
      *(Ptr*)v564177381df0 = v564177381e40;
      v564177381d00_break:
    }
    v564177381a90_continue:
  }
  v564177381a90_break:
  Ptr v564177381fd0 = p_new;
  Ptr v564177381f80 = l_end;
  Ptr v564177381f30 = &((Struct_564177380ef0*)v564177381f80)->next;
  *(Ptr*)v564177381f30 = v564177381fd0;
  Ptr v564177382140 = l_end;
  Ptr v5641773820f0 = p_new;
  Ptr v5641773820a0 = &((Struct_564177380ef0*)v5641773820f0)->prev;
  *(Ptr*)v5641773820a0 = v564177382140;
  v564177381500_break:
}

void test_insert_before(Ptr p_node, Ptr p_new) {
  Ptr v564177382630 = p_node;
  _Bool v564177382680 = (_Bool)v564177382630;
  _Bool v5641773826d0 = !v564177382680;
  _Bool v564177382810 = v5641773826d0;
  if (!v5641773826d0) {
    Ptr v564177382720 = p_new;
    _Bool v564177382770 = (_Bool)v564177382720;
    _Bool v5641773827c0 = !v564177382770;
    v564177382810 = v5641773827c0;
  }
;
  if (v564177382810)
  {
    return ;
  }
  v5641773825e0_break:
  Ptr v564177382b20 = p_node;
  Ptr v564177382ad0 = ((Struct_564177380ef0*)v564177382b20)->prev;
  Ptr v564177382a80 = p_new;
  Ptr v564177382a30 = &((Struct_564177380ef0*)v564177382a80)->prev;
  *(Ptr*)v564177382a30 = v564177382ad0;
  Ptr v564177382c60 = p_node;
  Ptr v564177382c10 = p_new;
  Ptr v564177382bc0 = &((Struct_564177380ef0*)v564177382c10)->next;
  *(Ptr*)v564177382bc0 = v564177382c60;
  Ptr v564177382df0 = p_new;
  Ptr v564177382da0 = p_node;
  Ptr v564177382d50 = ((Struct_564177380ef0*)v564177382da0)->prev;
  Ptr v564177382d00 = &((Struct_564177380ef0*)v564177382d50)->next;
  *(Ptr*)v564177382d00 = v564177382df0;
  Ptr v564177382f60 = p_new;
  Ptr v564177382f10 = p_node;
  Ptr v564177382ec0 = &((Struct_564177380ef0*)v564177382f10)->prev;
  *(Ptr*)v564177382ec0 = v564177382f60;
  v564177382540_break:
}

void test_insert_after(Ptr p_node, Ptr p_new) {
  Ptr v564177383450 = p_node;
  _Bool v5641773834a0 = (_Bool)v564177383450;
  _Bool v5641773834f0 = !v5641773834a0;
  _Bool v564177383630 = v5641773834f0;
  if (!v5641773834f0) {
    Ptr v564177383540 = p_new;
    _Bool v564177383590 = (_Bool)v564177383540;
    _Bool v5641773835e0 = !v564177383590;
    v564177383630 = v5641773835e0;
  }
;
  if (v564177383630)
  {
    return ;
  }
  v564177383400_break:
  Ptr v564177383940 = p_node;
  Ptr v5641773838f0 = ((Struct_564177380ef0*)v564177383940)->next;
  Ptr v5641773838a0 = p_new;
  Ptr v564177383850 = &((Struct_564177380ef0*)v5641773838a0)->next;
  *(Ptr*)v564177383850 = v5641773838f0;
  Ptr v564177383a80 = p_node;
  Ptr v564177383a30 = p_new;
  Ptr v5641773839e0 = &((Struct_564177380ef0*)v564177383a30)->prev;
  *(Ptr*)v5641773839e0 = v564177383a80;
  Ptr v564177383c60 = p_new;
  Ptr v564177383c10 = p_node;
  Ptr v564177383bc0 = ((Struct_564177380ef0*)v564177383c10)->next;
  Ptr v564177383b70 = &((Struct_564177380ef0*)v564177383bc0)->prev;
  *(Ptr*)v564177383b70 = v564177383c60;
  Ptr v564177383e20 = p_new;
  Ptr v564177383dd0 = p_node;
  Ptr v564177383d80 = &((Struct_564177380ef0*)v564177383dd0)->next;
  *(Ptr*)v564177383d80 = v564177383e20;
  v564177383360_break:
}

