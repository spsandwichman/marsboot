#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_21872dccd10 {
  int64_t data;
  Ptr next;
  Ptr prev;
} Struct_21872dccd10;
typedef void (*Fun_21872dcc390)(Ptr, Ptr);
typedef void (*Fun_21872dccc10)(Ptr, Ptr);
typedef void (*Fun_21872dccf50)(Ptr, Ptr);

void test_append_back(Ptr p_list, Ptr p_new);
void test_insert_before(Ptr p_node, Ptr p_new);
void test_insert_after(Ptr p_node, Ptr p_new);

void test_append_back(Ptr p_list, Ptr p_new) {
  Ptr v21872dce6f0 = p_list;
  _Bool v21872dcea60 = (_Bool)v21872dce6f0;
  _Bool v21872dce970 = !v21872dcea60;
  _Bool v21872dce740 = v21872dce970;
  if (!v21872dce970) {
    Ptr v21872dcea10 = p_new;
    _Bool v21872dceb00 = (_Bool)v21872dcea10;
    _Bool v21872dceab0 = !v21872dceb00;
    v21872dce740 = v21872dceab0;
  }
;
  if (v21872dce740)
  {
    return ;
  }
  Ptr v21872dce7e0 = p_list;
  Ptr l_end = v21872dce7e0;
  while (1) {
    Ptr v21872dceba0 = l_end;
    Ptr v21872dced30 = ((Struct_21872dccd10*)v21872dceba0)->next;
    _Bool v21872dced80 = (_Bool)v21872dced30;
    if (!v21872dced80) break;
    {
      Ptr v21872dce8d0 = l_end;
      Ptr v21872dce6a0 = ((Struct_21872dccd10*)v21872dce8d0)->next;
      Ptr v21872dce880 = &l_end;
      *(Ptr*)v21872dce880 = v21872dce6a0;
    }
    v21872dcec90_continue:
  }
  v21872dcec90_break:
  Ptr v21872e40460 = p_new;
  Ptr v21872e40aa0 = l_end;
  Ptr v21872dcec40 = &((Struct_21872dccd10*)v21872e40aa0)->next;
  *(Ptr*)v21872dcec40 = v21872e40460;
  Ptr v21872e408c0 = l_end;
  Ptr v21872e40960 = p_new;
  Ptr v21872e40910 = &((Struct_21872dccd10*)v21872e40960)->prev;
  *(Ptr*)v21872e40910 = v21872e408c0;
}

void test_insert_before(Ptr p_node, Ptr p_new) {
  Ptr v21872e40f00 = p_node;
  _Bool v21872e40690 = (_Bool)v21872e40f00;
  _Bool v21872e405a0 = !v21872e40690;
  _Bool v21872e406e0 = v21872e405a0;
  if (!v21872e405a0) {
    Ptr v21872e40c80 = p_new;
    _Bool v21872e40fa0 = (_Bool)v21872e40c80;
    _Bool v21872e40870 = !v21872e40fa0;
    v21872e406e0 = v21872e40870;
  }
;
  if (v21872e406e0)
  {
    return ;
  }
  Ptr v21872e409b0 = p_node;
  Ptr v21872e40f50 = ((Struct_21872dccd10*)v21872e409b0)->prev;
  Ptr v21872e40820 = p_new;
  Ptr v21872e40be0 = &((Struct_21872dccd10*)v21872e40820)->prev;
  *(Ptr*)v21872e40be0 = v21872e40f50;
  Ptr v21872e40cd0 = p_node;
  Ptr v21872e40a50 = p_new;
  Ptr v21872e40a00 = &((Struct_21872dccd10*)v21872e40a50)->next;
  *(Ptr*)v21872e40a00 = v21872e40cd0;
  Ptr v21872e41090 = p_new;
  Ptr v21872e40c30 = p_node;
  Ptr v21872e40230 = ((Struct_21872dccd10*)v21872e40c30)->prev;
  Ptr v21872e40280 = &((Struct_21872dccd10*)v21872e40230)->next;
  *(Ptr*)v21872e40280 = v21872e41090;
  Ptr v21872e40b40 = p_new;
  Ptr v21872e40af0 = p_node;
  Ptr v21872e410e0 = &((Struct_21872dccd10*)v21872e40af0)->prev;
  *(Ptr*)v21872e410e0 = v21872e40b40;
}

void test_insert_after(Ptr p_node, Ptr p_new) {
  Ptr v21872e40e60 = p_node;
  _Bool v21872e40eb0 = (_Bool)v21872e40e60;
  _Bool v21872e42070 = !v21872e40eb0;
  _Bool v21872e42250 = v21872e42070;
  if (!v21872e42070) {
    Ptr v21872e422a0 = p_new;
    _Bool v21872e41fd0 = (_Bool)v21872e422a0;
    _Bool v21872e41b20 = !v21872e41fd0;
    v21872e42250 = v21872e41b20;
  }
;
  if (v21872e42250)
  {
    return ;
  }
  Ptr v21872e41d50 = p_node;
  Ptr v21872e42200 = ((Struct_21872dccd10*)v21872e41d50)->next;
  Ptr v21872e420c0 = p_new;
  Ptr v21872e41df0 = &((Struct_21872dccd10*)v21872e420c0)->next;
  *(Ptr*)v21872e41df0 = v21872e42200;
  Ptr v21872e41850 = p_node;
  Ptr v21872e41e40 = p_new;
  Ptr v21872e41f80 = &((Struct_21872dccd10*)v21872e41e40)->prev;
  *(Ptr*)v21872e41f80 = v21872e41850;
  Ptr v21872e418a0 = p_new;
  Ptr v21872e41e90 = p_node;
  Ptr v21872e42110 = ((Struct_21872dccd10*)v21872e41e90)->next;
  Ptr v21872e41ee0 = &((Struct_21872dccd10*)v21872e42110)->prev;
  *(Ptr*)v21872e41ee0 = v21872e418a0;
  Ptr v21872e423e0 = p_new;
  Ptr v21872e41800 = p_node;
  Ptr v21872e42160 = &((Struct_21872dccd10*)v21872e41800)->next;
  *(Ptr*)v21872e42160 = v21872e423e0;
}

