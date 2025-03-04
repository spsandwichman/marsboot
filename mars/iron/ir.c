#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_55c17f99e070 {
  uint16_t kind;
  uint16_t type;
  Ptr next;
  Ptr prev;
} Struct_55c17f99e070;
typedef struct Struct_55c17f99e2b0 {
  Ptr block;
} Struct_55c17f99e2b0;
typedef struct Struct_55c17f99e320 {
  Ptr start;
  Ptr end;
  Ptr next_block;
  Ptr function;
} Struct_55c17f99e320;
typedef struct Struct_55c17f99e4c0 {
  Ptr symbol;
  Ptr first_block;
} Struct_55c17f99e4c0;
typedef struct Struct_55c17f99e550 {
  Slice name;
} Struct_55c17f99e550;
typedef struct Struct_55c17f99eb70 {
  Ptr sub;
} Struct_55c17f99eb70;
typedef struct Struct_55c17f99ed10 {
  Ptr lhs;
  Ptr rhs;
} Struct_55c17f99ed10;
typedef struct Struct_55c17f99eed0 {
  uint64_t val;
} Struct_55c17f99eed0;
typedef struct Struct_55c17f99f020 {
  Ptr ptr;
} Struct_55c17f99f020;
typedef struct Struct_55c17f99f1c0 {
  Ptr ptr;
  Ptr val;
} Struct_55c17f99f1c0;
typedef Ptr (*Fun_55c17f99f750)(Ptr, uint64_t);
typedef Ptr (*Fun_55c17f99fcb0)(Ptr, Ptr);
typedef Ptr (*Fun_55c17f9a1740)(Ptr, Ptr);
typedef Ptr (*Fun_55c17f9a29a0)(Ptr, Ptr);
typedef Ptr (*Fun_55c17f9a3c80)(Ptr, uint16_t, uint16_t);
typedef Ptr (*Fun_55c17f9a4640)(Ptr);
typedef Ptr (*Fun_55c17f9a4cc0)(Ptr, Ptr);
typedef Ptr (*Fun_55c17f9a56d0)(Ptr);
typedef Ptr (*Fun_55c17f9a61b0)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_55c17f9a6d80)(Ptr);
typedef Ptr (*Fun_55c17f9a78f0)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_55c17f9a84c0)(Ptr);
typedef Ptr (*Fun_55c17f9a9120)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_55c17f9a9cf0)(Ptr);
typedef Ptr (*Fun_55c17f9aa860)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_55c17f9ab430)(Ptr);
typedef Ptr (*Fun_55c17f9abeb0)(Ptr);

Ptr fe_alloc(Ptr p_f, uint64_t p_size);
Ptr fe_append(Ptr p_b, Ptr p_inst);
Ptr fe_insert_before(Ptr p_new, Ptr p_ref);
Ptr fe_insert_after(Ptr p_new, Ptr p_ref);
Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint16_t p_type);
Ptr fe_from_extra(Ptr p_extra);
Ptr fe_inst_bookend(Ptr p_f, Ptr p_block);
Ptr fe_to_bookend(Ptr p_inst);
Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_sub);
Ptr fe_to_unop(Ptr p_inst);
Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_lhs, Ptr p_rhs);
Ptr fe_to_binop(Ptr p_inst);
Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr);
Ptr fe_to_load(Ptr p_inst);
Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr, Ptr p_val);
Ptr fe_to_store(Ptr p_inst);
Ptr fe_new_block(Ptr p_f);

Ptr fe_alloc(Ptr p_f, uint64_t p_size) {
  Ptr v55c17f99f880 = (Ptr)0llu;
  return v55c17f99f880;
  v55c17f99f790_break:
}

Ptr fe_append(Ptr p_b, Ptr p_inst) {
  Ptr v55c17f99fe80 = p_b;
  Ptr v55c17f99fe30 = ((Struct_55c17f99e320*)v55c17f99fe80)->start;
  uint16_t v55c17f99fde0 = ((Struct_55c17f99e070*)v55c17f99fe30)->kind;
  uint16_t v55c17f99fed0 = (uint16_t)1llu;
  _Bool v55c17f99ff20 = (_Bool)(v55c17f99fde0 == v55c17f99fed0);
  if (v55c17f99ff20)
  {
    Ptr v55c17f9a0230 = p_b;
    Ptr v55c17f9a01e0 = ((Struct_55c17f99e320*)v55c17f9a0230)->start;
    Ptr l_bookend = v55c17f9a01e0;
    Ptr v55c17f9a0370 = l_bookend;
    Ptr v55c17f9a0320 = p_inst;
    Ptr v55c17f9a02d0 = &((Struct_55c17f99e070*)v55c17f9a0320)->prev;
    *(Ptr*)v55c17f9a02d0 = v55c17f9a0370;
    Ptr v55c17f9a04b0 = l_bookend;
    Ptr v55c17f9a0460 = p_inst;
    Ptr v55c17f9a0410 = &((Struct_55c17f99e070*)v55c17f9a0460)->next;
    *(Ptr*)v55c17f9a0410 = v55c17f9a04b0;
    Ptr v55c17f9a05f0 = p_inst;
    Ptr v55c17f9a05a0 = l_bookend;
    Ptr v55c17f9a0550 = &((Struct_55c17f99e070*)v55c17f9a05a0)->next;
    *(Ptr*)v55c17f9a0550 = v55c17f9a05f0;
    Ptr v55c17f9a0760 = p_inst;
    Ptr v55c17f9a0710 = l_bookend;
    Ptr v55c17f9a06c0 = &((Struct_55c17f99e070*)v55c17f9a0710)->prev;
    *(Ptr*)v55c17f9a06c0 = v55c17f9a0760;
    Ptr v55c17f9a08a0 = p_inst;
    Ptr v55c17f9a0850 = p_b;
    Ptr v55c17f9a0800 = &((Struct_55c17f99e320*)v55c17f9a0850)->start;
    *(Ptr*)v55c17f9a0800 = v55c17f9a08a0;
    Ptr v55c17f9a0a20 = p_inst;
    Ptr v55c17f9a09d0 = p_b;
    Ptr v55c17f9a0980 = &((Struct_55c17f99e320*)v55c17f9a09d0)->end;
    *(Ptr*)v55c17f9a0980 = v55c17f9a0a20;
    v55c17f9a00a0_break:
  }
  else {
    {
      Ptr v55c17f9a0d60 = p_b;
      Ptr v55c17f9a0d10 = ((Struct_55c17f99e320*)v55c17f9a0d60)->end;
      Ptr v55c17f9a0cc0 = p_inst;
      Ptr v55c17f9a0c70 = &((Struct_55c17f99e070*)v55c17f9a0cc0)->prev;
      *(Ptr*)v55c17f9a0c70 = v55c17f9a0d10;
      Ptr v55c17f9a0f40 = p_b;
      Ptr v55c17f9a0ef0 = ((Struct_55c17f99e320*)v55c17f9a0f40)->end;
      Ptr v55c17f9a0ea0 = ((Struct_55c17f99e070*)v55c17f9a0ef0)->next;
      Ptr v55c17f9a0e50 = p_inst;
      Ptr v55c17f9a0e00 = &((Struct_55c17f99e070*)v55c17f9a0e50)->next;
      *(Ptr*)v55c17f9a0e00 = v55c17f9a0ea0;
      Ptr v55c17f9a1120 = p_inst;
      Ptr v55c17f9a10d0 = p_b;
      Ptr v55c17f9a1080 = ((Struct_55c17f99e320*)v55c17f9a10d0)->end;
      Ptr v55c17f9a1030 = ((Struct_55c17f99e070*)v55c17f9a1080)->next;
      Ptr v55c17f9a0fe0 = &((Struct_55c17f99e070*)v55c17f9a1030)->prev;
      *(Ptr*)v55c17f9a0fe0 = v55c17f9a1120;
      Ptr v55c17f9a12b0 = p_inst;
      Ptr v55c17f9a1260 = p_b;
      Ptr v55c17f9a1210 = ((Struct_55c17f99e320*)v55c17f9a1260)->end;
      Ptr v55c17f9a11c0 = &((Struct_55c17f99e070*)v55c17f9a1210)->next;
      *(Ptr*)v55c17f9a11c0 = v55c17f9a12b0;
      v55c17f9a0b80_break:
    }
  }
  v55c17f99fd90_break:
  Ptr v55c17f9a1380 = p_inst;
  return v55c17f9a1380;
  v55c17f99fcf0_break:
}

Ptr fe_insert_before(Ptr p_new, Ptr p_ref) {
  Ptr v55c17f9a1910 = p_ref;
  Ptr v55c17f9a18c0 = ((Struct_55c17f99e070*)v55c17f9a1910)->prev;
  uint16_t v55c17f9a1870 = ((Struct_55c17f99e070*)v55c17f9a18c0)->kind;
  uint16_t v55c17f9a1960 = (uint16_t)1llu;
  _Bool v55c17f9a19b0 = (_Bool)(v55c17f9a1870 == v55c17f9a1960);
  if (v55c17f9a19b0)
  {
    Ptr v55c17f9a1da0 = p_ref;
    Ptr v55c17f9a1d50 = ((Struct_55c17f99e070*)v55c17f9a1da0)->prev;
    Ptr v55c17f9a1c70 = (Ptr)v55c17f9a1d50;
    Ptr l_bookend = v55c17f9a1c70;
    Ptr v55c17f9a1f30 = p_new;
    Ptr v55c17f9a1ee0 = l_bookend;
    Ptr v55c17f9a1e90 = ((Struct_55c17f99e2b0*)v55c17f9a1ee0)->block;
    Ptr v55c17f9a1e40 = &((Struct_55c17f99e320*)v55c17f9a1e90)->start;
    *(Ptr*)v55c17f9a1e40 = v55c17f9a1f30;
    v55c17f9a1b30_break:
  }
  v55c17f9a1820_break:
  Ptr v55c17f9a2070 = p_ref;
  Ptr v55c17f9a2020 = p_new;
  Ptr v55c17f9a1fd0 = &((Struct_55c17f99e070*)v55c17f9a2020)->next;
  *(Ptr*)v55c17f9a1fd0 = v55c17f9a2070;
  Ptr v55c17f9a2200 = p_ref;
  Ptr v55c17f9a21b0 = ((Struct_55c17f99e070*)v55c17f9a2200)->prev;
  Ptr v55c17f9a2160 = p_new;
  Ptr v55c17f9a2110 = &((Struct_55c17f99e070*)v55c17f9a2160)->prev;
  *(Ptr*)v55c17f9a2110 = v55c17f9a21b0;
  Ptr v55c17f9a2390 = p_new;
  Ptr v55c17f9a2340 = p_ref;
  Ptr v55c17f9a22f0 = ((Struct_55c17f99e070*)v55c17f9a2340)->prev;
  Ptr v55c17f9a22a0 = &((Struct_55c17f99e070*)v55c17f9a22f0)->next;
  *(Ptr*)v55c17f9a22a0 = v55c17f9a2390;
  Ptr v55c17f9a2500 = p_new;
  Ptr v55c17f9a24b0 = p_ref;
  Ptr v55c17f9a2460 = &((Struct_55c17f99e070*)v55c17f9a24b0)->prev;
  *(Ptr*)v55c17f9a2460 = v55c17f9a2500;
  Ptr v55c17f9a25a0 = p_new;
  return v55c17f9a25a0;
  v55c17f9a1780_break:
}

Ptr fe_insert_after(Ptr p_new, Ptr p_ref) {
  Ptr v55c17f9a2b70 = p_ref;
  Ptr v55c17f9a2b20 = ((Struct_55c17f99e070*)v55c17f9a2b70)->prev;
  uint16_t v55c17f9a2ad0 = ((Struct_55c17f99e070*)v55c17f9a2b20)->kind;
  uint16_t v55c17f9a2bc0 = (uint16_t)1llu;
  _Bool v55c17f9a2c10 = (_Bool)(v55c17f9a2ad0 == v55c17f9a2bc0);
  if (v55c17f9a2c10)
  {
    Ptr v55c17f9a2fc0 = p_ref;
    Ptr v55c17f9a2f70 = ((Struct_55c17f99e070*)v55c17f9a2fc0)->next;
    Ptr v55c17f9a2ed0 = (Ptr)v55c17f9a2f70;
    Ptr l_bookend = v55c17f9a2ed0;
    Ptr v55c17f9a3150 = p_new;
    Ptr v55c17f9a3100 = l_bookend;
    Ptr v55c17f9a30b0 = ((Struct_55c17f99e2b0*)v55c17f9a3100)->block;
    Ptr v55c17f9a3060 = &((Struct_55c17f99e320*)v55c17f9a30b0)->end;
    *(Ptr*)v55c17f9a3060 = v55c17f9a3150;
    v55c17f9a2d90_break:
  }
  v55c17f9a2a80_break:
  Ptr v55c17f9a3290 = p_ref;
  Ptr v55c17f9a3240 = p_new;
  Ptr v55c17f9a31f0 = &((Struct_55c17f99e070*)v55c17f9a3240)->prev;
  *(Ptr*)v55c17f9a31f0 = v55c17f9a3290;
  Ptr v55c17f9a3420 = p_ref;
  Ptr v55c17f9a33d0 = ((Struct_55c17f99e070*)v55c17f9a3420)->next;
  Ptr v55c17f9a3380 = p_new;
  Ptr v55c17f9a3330 = &((Struct_55c17f99e070*)v55c17f9a3380)->next;
  *(Ptr*)v55c17f9a3330 = v55c17f9a33d0;
  Ptr v55c17f9a35b0 = p_new;
  Ptr v55c17f9a3560 = p_ref;
  Ptr v55c17f9a3510 = ((Struct_55c17f99e070*)v55c17f9a3560)->next;
  Ptr v55c17f9a34c0 = &((Struct_55c17f99e070*)v55c17f9a3510)->prev;
  *(Ptr*)v55c17f9a34c0 = v55c17f9a35b0;
  Ptr v55c17f9a36f0 = p_new;
  Ptr v55c17f9a36a0 = p_ref;
  Ptr v55c17f9a3650 = &((Struct_55c17f99e070*)v55c17f9a36a0)->next;
  *(Ptr*)v55c17f9a3650 = v55c17f9a36f0;
  Ptr v55c17f9a3790 = p_new;
  return v55c17f9a3790;
  v55c17f9a29e0_break:
}

Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint16_t p_type) {
  Ptr v55c17f9a3f10 = p_f;
  uint64_t v55c17f9a3f60 = (uint64_t)24llu;
  Fun_55c17f99f750 v55c17f9a3ec0 = fe_alloc;
  Ptr v55c17f9a3e50 = v55c17f9a3ec0(v55c17f9a3f10, v55c17f9a3f60);
  Ptr l_inst = v55c17f9a3e50;
  uint16_t v55c17f9a4140 = p_type;
  Ptr v55c17f9a40f0 = l_inst;
  Ptr v55c17f9a40a0 = &((Struct_55c17f99e070*)v55c17f9a40f0)->type;
  *(uint16_t*)v55c17f9a40a0 = v55c17f9a4140;
  uint16_t v55c17f9a4280 = p_kind;
  Ptr v55c17f9a4230 = l_inst;
  Ptr v55c17f9a41e0 = &((Struct_55c17f99e070*)v55c17f9a4230)->kind;
  *(uint16_t*)v55c17f9a41e0 = v55c17f9a4280;
  Ptr v55c17f9a4320 = l_inst;
  return v55c17f9a4320;
  v55c17f9a3cc0_break:
}

Ptr fe_from_extra(Ptr p_extra) {
  Ptr v55c17f9a4770 = p_extra;
  Ptr v55c17f9a47c0 = (Ptr)24llu;
  Ptr v55c17f9a4860 = (Ptr)(v55c17f9a4770 - v55c17f9a47c0);
  Ptr v55c17f9a48b0 = (Ptr)v55c17f9a4860;
  return v55c17f9a48b0;
  v55c17f9a4680_break:
}

Ptr fe_inst_bookend(Ptr p_f, Ptr p_block) {
  Ptr v55c17f9a4f30 = p_f;
  uint64_t v55c17f9a50c0 = (uint64_t)32llu;
  Fun_55c17f99f750 v55c17f9a4ee0 = fe_alloc;
  Ptr v55c17f9a4e90 = v55c17f9a4ee0(v55c17f9a4f30, v55c17f9a50c0);
  Ptr l_inst = v55c17f9a4e90;
  uint16_t v55c17f9a5250 = (uint16_t)1llu;
  Ptr v55c17f9a5200 = l_inst;
  Ptr v55c17f9a51b0 = &((Struct_55c17f99e070*)v55c17f9a5200)->kind;
  *(uint16_t*)v55c17f9a51b0 = v55c17f9a5250;
  Ptr v55c17f9a5a30 = l_inst;
  Fun_55c17f9a56d0 v55c17f9a59e0 = fe_to_bookend;
  Ptr v55c17f9a5340 = v55c17f9a59e0(v55c17f9a5a30);
  Ptr l_bookend = v55c17f9a5340;
  Ptr v55c17f9a5b70 = p_block;
  Ptr v55c17f9a5b20 = l_bookend;
  Ptr v55c17f9a5ad0 = &((Struct_55c17f99e2b0*)v55c17f9a5b20)->block;
  *(Ptr*)v55c17f9a5ad0 = v55c17f9a5b70;
  Ptr v55c17f9a5c10 = l_inst;
  return v55c17f9a5c10;
  v55c17f9a4d00_break:
}

Ptr fe_to_bookend(Ptr p_inst) {
  Ptr v55c17f9a58a0 = p_inst;
  Ptr v55c17f9a5800 = (Ptr)v55c17f9a58a0;
  int64_t v55c17f9a58f0 = (int64_t)24ll;
  Ptr v55c17f9a5990 = (Ptr)(v55c17f9a5800 + v55c17f9a58f0);
  return v55c17f9a5990;
  v55c17f9a5710_break:
}

Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_sub) {
  Ptr v55c17f9a6440 = p_f;
  uint64_t v55c17f9a65d0 = (uint64_t)32llu;
  Fun_55c17f99f750 v55c17f9a63f0 = fe_alloc;
  Ptr v55c17f9a6380 = v55c17f9a63f0(v55c17f9a6440, v55c17f9a65d0);
  Ptr l_inst = v55c17f9a6380;
  uint16_t v55c17f9a6760 = p_type;
  Ptr v55c17f9a6710 = l_inst;
  Ptr v55c17f9a66c0 = &((Struct_55c17f99e070*)v55c17f9a6710)->type;
  *(uint16_t*)v55c17f9a66c0 = v55c17f9a6760;
  uint16_t v55c17f9a68a0 = p_kind;
  Ptr v55c17f9a6850 = l_inst;
  Ptr v55c17f9a6800 = &((Struct_55c17f99e070*)v55c17f9a6850)->kind;
  *(uint16_t*)v55c17f9a6800 = v55c17f9a68a0;
  Ptr v55c17f9a70e0 = l_inst;
  Fun_55c17f9a6d80 v55c17f9a7090 = fe_to_unop;
  Ptr v55c17f9a6990 = v55c17f9a7090(v55c17f9a70e0);
  Ptr l_unop = v55c17f9a6990;
  Ptr v55c17f9a7220 = p_sub;
  Ptr v55c17f9a71d0 = l_unop;
  Ptr v55c17f9a7180 = &((Struct_55c17f99eb70*)v55c17f9a71d0)->sub;
  *(Ptr*)v55c17f9a7180 = v55c17f9a7220;
  Ptr v55c17f9a72c0 = l_inst;
  return v55c17f9a72c0;
  v55c17f9a61f0_break:
}

Ptr fe_to_unop(Ptr p_inst) {
  Ptr v55c17f9a6f50 = p_inst;
  Ptr v55c17f9a6eb0 = (Ptr)v55c17f9a6f50;
  int64_t v55c17f9a6fa0 = (int64_t)24ll;
  Ptr v55c17f9a7040 = (Ptr)(v55c17f9a6eb0 + v55c17f9a6fa0);
  return v55c17f9a7040;
  v55c17f9a6dc0_break:
}

Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_lhs, Ptr p_rhs) {
  Ptr v55c17f9a7b80 = p_f;
  uint64_t v55c17f9a7d10 = (uint64_t)40llu;
  Fun_55c17f99f750 v55c17f9a7b30 = fe_alloc;
  Ptr v55c17f9a7ac0 = v55c17f9a7b30(v55c17f9a7b80, v55c17f9a7d10);
  Ptr l_inst = v55c17f9a7ac0;
  uint16_t v55c17f9a7ea0 = p_type;
  Ptr v55c17f9a7e50 = l_inst;
  Ptr v55c17f9a7e00 = &((Struct_55c17f99e070*)v55c17f9a7e50)->type;
  *(uint16_t*)v55c17f9a7e00 = v55c17f9a7ea0;
  uint16_t v55c17f9a7fe0 = p_kind;
  Ptr v55c17f9a7f90 = l_inst;
  Ptr v55c17f9a7f40 = &((Struct_55c17f99e070*)v55c17f9a7f90)->kind;
  *(uint16_t*)v55c17f9a7f40 = v55c17f9a7fe0;
  Ptr v55c17f9a8820 = l_inst;
  Fun_55c17f9a84c0 v55c17f9a87d0 = fe_to_binop;
  Ptr v55c17f9a80d0 = v55c17f9a87d0(v55c17f9a8820);
  Ptr l_binop = v55c17f9a80d0;
  Ptr v55c17f9a8960 = p_lhs;
  Ptr v55c17f9a8910 = l_binop;
  Ptr v55c17f9a88c0 = &((Struct_55c17f99ed10*)v55c17f9a8910)->lhs;
  *(Ptr*)v55c17f9a88c0 = v55c17f9a8960;
  Ptr v55c17f9a8aa0 = p_rhs;
  Ptr v55c17f9a8a50 = l_binop;
  Ptr v55c17f9a8a00 = &((Struct_55c17f99ed10*)v55c17f9a8a50)->rhs;
  *(Ptr*)v55c17f9a8a00 = v55c17f9a8aa0;
  Ptr v55c17f9a8b80 = l_inst;
  return v55c17f9a8b80;
  v55c17f9a7930_break:
}

Ptr fe_to_binop(Ptr p_inst) {
  Ptr v55c17f9a8690 = p_inst;
  Ptr v55c17f9a85f0 = (Ptr)v55c17f9a8690;
  int64_t v55c17f9a86e0 = (int64_t)24ll;
  Ptr v55c17f9a8780 = (Ptr)(v55c17f9a85f0 + v55c17f9a86e0);
  return v55c17f9a8780;
  v55c17f9a8500_break:
}

Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr) {
  Ptr v55c17f9a93b0 = p_f;
  uint64_t v55c17f9a9540 = (uint64_t)32llu;
  Fun_55c17f99f750 v55c17f9a9360 = fe_alloc;
  Ptr v55c17f9a92f0 = v55c17f9a9360(v55c17f9a93b0, v55c17f9a9540);
  Ptr l_inst = v55c17f9a92f0;
  uint16_t v55c17f9a96d0 = p_type;
  Ptr v55c17f9a9680 = l_inst;
  Ptr v55c17f9a9630 = &((Struct_55c17f99e070*)v55c17f9a9680)->type;
  *(uint16_t*)v55c17f9a9630 = v55c17f9a96d0;
  uint16_t v55c17f9a9810 = p_kind;
  Ptr v55c17f9a97c0 = l_inst;
  Ptr v55c17f9a9770 = &((Struct_55c17f99e070*)v55c17f9a97c0)->kind;
  *(uint16_t*)v55c17f9a9770 = v55c17f9a9810;
  Ptr v55c17f9aa050 = l_inst;
  Fun_55c17f9a9cf0 v55c17f9aa000 = fe_to_load;
  Ptr v55c17f9a9900 = v55c17f9aa000(v55c17f9aa050);
  Ptr l_load = v55c17f9a9900;
  Ptr v55c17f9aa190 = p_ptr;
  Ptr v55c17f9aa140 = l_load;
  Ptr v55c17f9aa0f0 = &((Struct_55c17f99f020*)v55c17f9aa140)->ptr;
  *(Ptr*)v55c17f9aa0f0 = v55c17f9aa190;
  Ptr v55c17f9aa230 = l_inst;
  return v55c17f9aa230;
  v55c17f9a9160_break:
}

Ptr fe_to_load(Ptr p_inst) {
  Ptr v55c17f9a9ec0 = p_inst;
  Ptr v55c17f9a9e20 = (Ptr)v55c17f9a9ec0;
  int64_t v55c17f9a9f10 = (int64_t)24ll;
  Ptr v55c17f9a9fb0 = (Ptr)(v55c17f9a9e20 + v55c17f9a9f10);
  return v55c17f9a9fb0;
  v55c17f9a9d30_break:
}

Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr, Ptr p_val) {
  Ptr v55c17f9aaaf0 = p_f;
  uint64_t v55c17f9aac80 = (uint64_t)40llu;
  Fun_55c17f99f750 v55c17f9aaaa0 = fe_alloc;
  Ptr v55c17f9aaa30 = v55c17f9aaaa0(v55c17f9aaaf0, v55c17f9aac80);
  Ptr l_inst = v55c17f9aaa30;
  uint16_t v55c17f9aae10 = p_type;
  Ptr v55c17f9aadc0 = l_inst;
  Ptr v55c17f9aad70 = &((Struct_55c17f99e070*)v55c17f9aadc0)->type;
  *(uint16_t*)v55c17f9aad70 = v55c17f9aae10;
  uint16_t v55c17f9aaf50 = p_kind;
  Ptr v55c17f9aaf00 = l_inst;
  Ptr v55c17f9aaeb0 = &((Struct_55c17f99e070*)v55c17f9aaf00)->kind;
  *(uint16_t*)v55c17f9aaeb0 = v55c17f9aaf50;
  Ptr v55c17f9ab790 = l_inst;
  Fun_55c17f9ab430 v55c17f9ab740 = fe_to_store;
  Ptr v55c17f9ab040 = v55c17f9ab740(v55c17f9ab790);
  Ptr l_store = v55c17f9ab040;
  Ptr v55c17f9ab8d0 = p_ptr;
  Ptr v55c17f9ab880 = l_store;
  Ptr v55c17f9ab830 = &((Struct_55c17f99f1c0*)v55c17f9ab880)->ptr;
  *(Ptr*)v55c17f9ab830 = v55c17f9ab8d0;
  Ptr v55c17f9aba10 = p_val;
  Ptr v55c17f9ab9c0 = l_store;
  Ptr v55c17f9ab970 = &((Struct_55c17f99f1c0*)v55c17f9ab9c0)->val;
  *(Ptr*)v55c17f9ab970 = v55c17f9aba10;
  Ptr v55c17f9abaf0 = l_inst;
  return v55c17f9abaf0;
  v55c17f9aa8a0_break:
}

Ptr fe_to_store(Ptr p_inst) {
  Ptr v55c17f9ab600 = p_inst;
  Ptr v55c17f9ab560 = (Ptr)v55c17f9ab600;
  int64_t v55c17f9ab650 = (int64_t)24ll;
  Ptr v55c17f9ab6f0 = (Ptr)(v55c17f9ab560 + v55c17f9ab650);
  return v55c17f9ab6f0;
  v55c17f9ab470_break:
}

Ptr fe_new_block(Ptr p_f) {
  Ptr v55c17f9ac140 = p_f;
  uint64_t v55c17f9ac190 = (uint64_t)32llu;
  Fun_55c17f99f750 v55c17f9ac0f0 = fe_alloc;
  Ptr v55c17f9ac080 = v55c17f9ac0f0(v55c17f9ac140, v55c17f9ac190);
  Ptr l_block = v55c17f9ac080;
  Ptr v55c17f9ac3e0 = p_f;
  Ptr v55c17f9ac430 = l_block;
  Fun_55c17f9a4cc0 v55c17f9ac390 = fe_inst_bookend;
  Ptr v55c17f9ac320 = v55c17f9ac390(v55c17f9ac3e0, v55c17f9ac430);
  Ptr l_bookend = v55c17f9ac320;
  Ptr v55c17f9ac570 = (Ptr)0llu;
  Ptr v55c17f9ac520 = l_block;
  Ptr v55c17f9ac4d0 = &((Struct_55c17f99e320*)v55c17f9ac520)->start;
  *(Ptr*)v55c17f9ac4d0 = v55c17f9ac570;
  Ptr v55c17f9ac6b0 = l_bookend;
  Ptr v55c17f9ac660 = l_block;
  Ptr v55c17f9ac610 = &((Struct_55c17f99e320*)v55c17f9ac660)->end;
  *(Ptr*)v55c17f9ac610 = v55c17f9ac6b0;
  Ptr v55c17f9ac7f0 = p_f;
  Ptr v55c17f9ac7a0 = l_block;
  Ptr v55c17f9ac750 = &((Struct_55c17f99e320*)v55c17f9ac7a0)->function;
  *(Ptr*)v55c17f9ac750 = v55c17f9ac7f0;
  Ptr v55c17f9ac890 = l_block;
  return v55c17f9ac890;
  v55c17f9abef0_break:
}

