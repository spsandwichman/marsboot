#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_15528270ae0 {
  uint16_t kind;
  uint16_t type;
  Ptr next;
  Ptr prev;
} Struct_15528270ae0;
typedef struct Struct_15528275c90 {
  Ptr block;
} Struct_15528275c90;
typedef struct Struct_15528275ad0 {
  Ptr start;
  Ptr end;
  Ptr next_block;
  Ptr function;
} Struct_15528275ad0;
typedef struct Struct_155282753d0 {
  Ptr symbol;
  Ptr first_block;
} Struct_155282753d0;
typedef struct Struct_15528275bd0 {
  Slice name;
} Struct_15528275bd0;
typedef struct Struct_15528275d50 {
  Ptr sub;
} Struct_15528275d50;
typedef struct Struct_15528275c50 {
  Ptr lhs;
  Ptr rhs;
} Struct_15528275c50;
typedef struct Struct_155282752d0 {
  uint64_t val;
} Struct_155282752d0;
typedef struct Struct_15528275410 {
  Ptr ptr;
} Struct_15528275410;
typedef struct Struct_15528275a90 {
  Ptr ptr;
  Ptr val;
} Struct_15528275a90;
typedef Ptr (*Fun_155282755d0)(Ptr, uint64_t);
typedef Ptr (*Fun_155282756d0)(Ptr, Ptr);
typedef Ptr (*Fun_155282754d0)(Ptr, Ptr);
typedef Ptr (*Fun_15528275550)(Ptr, Ptr);
typedef Ptr (*Fun_15528275cd0)(Ptr, uint16_t, uint16_t);
typedef Ptr (*Fun_15528275c10)(Ptr);
typedef Ptr (*Fun_15528275b50)(Ptr, Ptr);
typedef Ptr (*Fun_15528275650)(Ptr);
typedef Ptr (*Fun_15528275dd0)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_15528275a10)(Ptr);
typedef Ptr (*Fun_15528275d10)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_15528275750)(Ptr);
typedef Ptr (*Fun_15528275ed0)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_15528275990)(Ptr);
typedef Ptr (*Fun_15528275590)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_15528275350)(Ptr);
typedef Ptr (*Fun_15528275fd0)(Ptr);

Ptr fe_malloc(Ptr p_f, uint64_t p_size);
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

Ptr fe_malloc(Ptr p_f, uint64_t p_size) {
  Ptr v15528276860 = (Ptr)0llu;
  return v15528276860;
  v15528276b30_break:
}

Ptr fe_append(Ptr p_b, Ptr p_inst) {
  Ptr v15528276950 = p_b;
  Ptr v15528276f90 = ((Struct_15528275ad0*)v15528276950)->start;
  uint16_t v155282769a0 = ((Struct_15528270ae0*)v15528276f90)->kind;
  uint16_t v15528276540 = (uint16_t)1llu;
  _Bool v15528276fe0 = (_Bool)(v155282769a0 == v15528276540);
  if (v15528276fe0)
  {
    Ptr v15528277f60 = p_b;
    Ptr v15528278370 = ((Struct_15528275ad0*)v15528277f60)->start;
    Ptr l_bookend = v15528278370;
    Ptr v15528278140 = l_bookend;
    Ptr v15528277b00 = p_inst;
    Ptr v15528278230 = &((Struct_15528270ae0*)v15528277b00)->prev;
    *(Ptr*)v15528278230 = v15528278140;
    Ptr v15528278000 = l_bookend;
    Ptr v15528277880 = p_inst;
    Ptr v15528277ba0 = &((Struct_15528270ae0*)v15528277880)->next;
    *(Ptr*)v15528277ba0 = v15528278000;
    Ptr v15528277830 = p_inst;
    Ptr v15528277510 = l_bookend;
    Ptr v155282782d0 = &((Struct_15528270ae0*)v15528277510)->next;
    *(Ptr*)v155282782d0 = v15528277830;
    Ptr v155282780a0 = p_inst;
    Ptr v15528277c40 = l_bookend;
    Ptr v15528277e20 = &((Struct_15528270ae0*)v15528277c40)->prev;
    *(Ptr*)v15528277e20 = v155282780a0;
    Ptr v15528277bf0 = p_inst;
    Ptr v155282777e0 = p_b;
    Ptr v15528277b50 = &((Struct_15528275ad0*)v155282777e0)->start;
    *(Ptr*)v15528277b50 = v15528277bf0;
    Ptr v15528277d30 = p_inst;
    Ptr v15528277f10 = p_b;
    Ptr v15528277c90 = &((Struct_15528275ad0*)v15528277f10)->end;
    *(Ptr*)v15528277c90 = v15528277d30;
    v155282765e0_break:
  }
  else {
    {
      Ptr v15528278460 = p_b;
      Ptr v15528277d80 = ((Struct_15528275ad0*)v15528278460)->end;
      Ptr v15528277fb0 = p_inst;
      Ptr v15528277560 = &((Struct_15528270ae0*)v15528277fb0)->prev;
      *(Ptr*)v15528277560 = v15528277d80;
      Ptr v15528277ec0 = p_b;
      Ptr v15528277dd0 = ((Struct_15528275ad0*)v15528277ec0)->end;
      Ptr v155282775b0 = ((Struct_15528270ae0*)v15528277dd0)->next;
      Ptr v15528277970 = p_inst;
      Ptr v155282778d0 = &((Struct_15528270ae0*)v15528277970)->next;
      *(Ptr*)v155282778d0 = v155282775b0;
      Ptr v15528277650 = p_inst;
      Ptr v15528277600 = p_b;
      Ptr v155282776f0 = ((Struct_15528275ad0*)v15528277600)->end;
      Ptr v15528278410 = ((Struct_15528270ae0*)v155282776f0)->next;
      Ptr v15528277920 = &((Struct_15528270ae0*)v15528278410)->prev;
      *(Ptr*)v15528277920 = v15528277650;
      Ptr v155282779c0 = p_inst;
      Ptr v15528277790 = p_b;
      Ptr v15528277740 = ((Struct_15528275ad0*)v15528277790)->end;
      Ptr v15528277ce0 = &((Struct_15528270ae0*)v15528277740)->next;
      *(Ptr*)v15528277ce0 = v155282779c0;
      v15528278190_break:
    }
  }
  v15528276f40_break:
  Ptr v15528277030 = p_inst;
  return v15528277030;
  v15528277210_break:
}

Ptr fe_insert_before(Ptr p_new, Ptr p_ref) {
  Ptr v15528278af0 = p_ref;
  Ptr v155282790e0 = ((Struct_15528270ae0*)v15528278af0)->prev;
  uint16_t v15528278780 = ((Struct_15528270ae0*)v155282790e0)->kind;
  uint16_t v15528279540 = (uint16_t)1llu;
  _Bool v15528279450 = (_Bool)(v15528278780 == v15528279540);
  if (v15528279450)
  {
    Ptr v15528278a00 = p_ref;
    Ptr v15528278d70 = ((Struct_15528270ae0*)v15528278a00)->prev;
    Ptr v15528279270 = (Ptr)v15528278d70;
    Ptr l_bookend = v15528279270;
    Ptr v15528278b40 = p_new;
    Ptr v15528278730 = l_bookend;
    Ptr v155282794a0 = ((Struct_15528275c90*)v15528278730)->block;
    Ptr v155282791d0 = &((Struct_15528275ad0*)v155282794a0)->start;
    *(Ptr*)v155282791d0 = v15528278b40;
    v155282789b0_break:
  }
  v15528278960_break:
  Ptr v155282787d0 = p_ref;
  Ptr v15528279360 = p_new;
  Ptr v15528278d20 = &((Struct_15528270ae0*)v15528279360)->next;
  *(Ptr*)v15528278d20 = v155282787d0;
  Ptr v15528279130 = p_ref;
  Ptr v15528278e10 = ((Struct_15528270ae0*)v15528279130)->prev;
  Ptr v155282794f0 = p_new;
  Ptr v15528279400 = &((Struct_15528270ae0*)v155282794f0)->prev;
  *(Ptr*)v15528279400 = v15528278e10;
  Ptr v15528278be0 = p_new;
  Ptr v155282786e0 = p_ref;
  Ptr v15528278c80 = ((Struct_15528270ae0*)v155282786e0)->prev;
  Ptr v15528279090 = &((Struct_15528270ae0*)v15528278c80)->next;
  *(Ptr*)v15528279090 = v15528278be0;
  Ptr v15528278cd0 = p_new;
  Ptr v15528278c30 = p_ref;
  Ptr v15528279180 = &((Struct_15528270ae0*)v15528278c30)->prev;
  *(Ptr*)v15528279180 = v15528278cd0;
  Ptr v15528278820 = p_new;
  return v15528278820;
  v15528278910_break:
}

Ptr fe_insert_after(Ptr p_new, Ptr p_ref) {
  Ptr v1552827c3b0 = p_ref;
  Ptr v1552827bd70 = ((Struct_15528270ae0*)v1552827c3b0)->prev;
  uint16_t v1552827b820 = ((Struct_15528270ae0*)v1552827bd70)->kind;
  uint16_t v1552827c130 = (uint16_t)1llu;
  _Bool v1552827c0e0 = (_Bool)(v1552827b820 == v1552827c130);
  if (v1552827c0e0)
  {
    Ptr v1552827be60 = p_ref;
    Ptr v1552827c180 = ((Struct_15528270ae0*)v1552827be60)->next;
    Ptr v1552827c450 = (Ptr)v1552827c180;
    Ptr l_bookend = v1552827c450;
    Ptr v1552827c590 = p_new;
    Ptr v1552827ba00 = l_bookend;
    Ptr v1552827b730 = ((Struct_15528275c90*)v1552827ba00)->block;
    Ptr v1552827bcd0 = &((Struct_15528275ad0*)v1552827b730)->end;
    *(Ptr*)v1552827bcd0 = v1552827c590;
    v1552827b780_break:
  }
  v1552827c090_break:
  Ptr v1552827beb0 = p_ref;
  Ptr v1552827b6e0 = p_new;
  Ptr v1552827baa0 = &((Struct_15528270ae0*)v1552827b6e0)->prev;
  *(Ptr*)v1552827baa0 = v1552827beb0;
  Ptr v1552827b7d0 = p_ref;
  Ptr v1552827c630 = ((Struct_15528270ae0*)v1552827b7d0)->next;
  Ptr v1552827bf00 = p_new;
  Ptr v1552827c540 = &((Struct_15528270ae0*)v1552827bf00)->next;
  *(Ptr*)v1552827c540 = v1552827c630;
  Ptr v1552827bc30 = p_new;
  Ptr v1552827b960 = p_ref;
  Ptr v1552827b910 = ((Struct_15528270ae0*)v1552827b960)->next;
  Ptr v1552827bff0 = &((Struct_15528270ae0*)v1552827b910)->prev;
  *(Ptr*)v1552827bff0 = v1552827bc30;
  Ptr v1552827c4a0 = p_new;
  Ptr v1552827b9b0 = p_ref;
  Ptr v1552827c040 = &((Struct_15528270ae0*)v1552827b9b0)->next;
  *(Ptr*)v1552827c040 = v1552827c4a0;
  Ptr v1552827bb40 = p_new;
  return v1552827bb40;
  v15528278ff0_break:
}

Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint16_t p_type) {
  Ptr v1552827c900 = p_f;
  uint64_t v1552827d120 = (uint64_t)24llu;
  Fun_155282755d0 v1552827c950 = fe_malloc;
  Ptr v1552827d1c0 = v1552827c950(v1552827c900, v1552827d120);
  Ptr l_inst = v1552827d1c0;
  uint16_t v1552827cb80 = p_type;
  Ptr v1552827cb30 = l_inst;
  Ptr v1552827cc20 = &((Struct_15528270ae0*)v1552827cb30)->type;
  *(uint16_t*)v1552827cc20 = v1552827cb80;
  uint16_t v1552827c9a0 = p_kind;
  Ptr v1552827c9f0 = l_inst;
  Ptr v1552827d580 = &((Struct_15528270ae0*)v1552827c9f0)->kind;
  *(uint16_t*)v1552827d580 = v1552827c9a0;
  Ptr v1552827cc70 = l_inst;
  return v1552827cc70;
  v1552827c360_break:
}

Ptr fe_from_extra(Ptr p_extra) {
  Ptr v1552827d300 = p_extra;
  Ptr v1552827d350 = (Ptr)24llu;
  Ptr v1552827ccc0 = (Ptr)(v1552827d300 - v1552827d350);
  Ptr v1552827ca90 = (Ptr)v1552827ccc0;
  return v1552827ca90;
  v1552827d6c0_break:
}

Ptr fe_inst_bookend(Ptr p_f, Ptr p_block) {
  Ptr v1552827c860 = p_f;
  uint64_t v1552827cdb0 = (uint64_t)32llu;
  Fun_155282755d0 v1552827d670 = fe_malloc;
  Ptr v1552827c810 = v1552827d670(v1552827c860, v1552827cdb0);
  Ptr l_inst = v1552827c810;
  uint16_t v1552827cea0 = (uint16_t)1llu;
  Ptr v1552827ce50 = l_inst;
  Ptr v1552827d530 = &((Struct_15528270ae0*)v1552827ce50)->kind;
  *(uint16_t*)v1552827d530 = v1552827cea0;
  Ptr v1552827e780 = l_inst;
  Fun_15528275650 v1552827e1e0 = fe_to_bookend;
  Ptr v1552827dd80 = v1552827e1e0(v1552827e780);
  Ptr l_bookend = v1552827dd80;
  Ptr v1552827e640 = p_block;
  Ptr v1552827df10 = l_bookend;
  Ptr v1552827e3c0 = &((Struct_15528275c90*)v1552827df10)->block;
  *(Ptr*)v1552827e3c0 = v1552827e640;
  Ptr v1552827e5f0 = l_inst;
  return v1552827e5f0;
  v1552827cd10_break:
}

Ptr fe_to_bookend(Ptr p_inst) {
  Ptr v1552827dfb0 = p_inst;
  Ptr v1552827de70 = (Ptr)v1552827dfb0;
  int64_t v1552827dec0 = (int64_t)24ll;
  Ptr v1552827e410 = (Ptr)(v1552827de70 + v1552827dec0);
  return v1552827e410;
  v1552827e0f0_break:
}

Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_sub) {
  Ptr v1552827e690 = p_f;
  uint64_t v1552827da10 = (uint64_t)32llu;
  Fun_155282755d0 v1552827e500 = fe_malloc;
  Ptr v1552827dab0 = v1552827e500(v1552827e690, v1552827da10);
  Ptr l_inst = v1552827dab0;
  uint16_t v1552827dbf0 = p_type;
  Ptr v1552827dba0 = l_inst;
  Ptr v1552827e6e0 = &((Struct_15528270ae0*)v1552827dba0)->type;
  *(uint16_t*)v1552827e6e0 = v1552827dbf0;
  uint16_t v1552827f340 = p_kind;
  Ptr v1552827ec60 = l_inst;
  Ptr v1552827fa70 = &((Struct_15528270ae0*)v1552827ec60)->kind;
  *(uint16_t*)v1552827fa70 = v1552827f340;
  Ptr v1552827eb20 = l_inst;
  Fun_15528275a10 v1552827edf0 = fe_to_unop;
  Ptr v1552827ee40 = v1552827edf0(v1552827eb20);
  Ptr l_unop = v1552827ee40;
  Ptr v1552827f8e0 = p_sub;
  Ptr v1552827ee90 = l_unop;
  Ptr v1552827ed50 = &((Struct_15528275d50*)v1552827ee90)->sub;
  *(Ptr*)v1552827ed50 = v1552827f8e0;
  Ptr v1552827f610 = l_inst;
  return v1552827f610;
  v1552827e370_break:
}

Ptr fe_to_unop(Ptr p_inst) {
  Ptr v1552827f070 = p_inst;
  Ptr v1552827eda0 = (Ptr)v1552827f070;
  int64_t v1552827f2a0 = (int64_t)24ll;
  Ptr v1552827f840 = (Ptr)(v1552827eda0 + v1552827f2a0);
  return v1552827f840;
  v1552827f390_break:
}

Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_lhs, Ptr p_rhs) {
  Ptr v1552827f980 = p_f;
  uint64_t v15528280c80 = (uint64_t)40llu;
  Fun_155282755d0 v1552827f5c0 = fe_malloc;
  Ptr v1552827f570 = v1552827f5c0(v1552827f980, v15528280c80);
  Ptr l_inst = v1552827f570;
  uint16_t v1552827fce0 = p_type;
  Ptr v155282800a0 = l_inst;
  Ptr v15528280820 = &((Struct_15528270ae0*)v155282800a0)->type;
  *(uint16_t*)v15528280820 = v1552827fce0;
  uint16_t v155282805a0 = p_kind;
  Ptr v15528280230 = l_inst;
  Ptr v15528280460 = &((Struct_15528270ae0*)v15528280230)->kind;
  *(uint16_t*)v15528280460 = v155282805a0;
  Ptr v155282802d0 = l_inst;
  Fun_15528275750 v15528280870 = fe_to_binop;
  Ptr v15528280cd0 = v15528280870(v155282802d0);
  Ptr l_binop = v15528280cd0;
  Ptr v15528280b40 = p_lhs;
  Ptr v15528280640 = l_binop;
  Ptr v15528280370 = &((Struct_15528275c50*)v15528280640)->lhs;
  *(Ptr*)v15528280370 = v15528280b40;
  Ptr v15528280ff0 = p_rhs;
  Ptr v15528280410 = l_binop;
  Ptr v15528280eb0 = &((Struct_15528275c50*)v15528280410)->rhs;
  *(Ptr*)v15528280eb0 = v15528280ff0;
  Ptr v15528280e10 = l_inst;
  return v15528280e10;
  v1552827f200_break:
}

Ptr fe_to_binop(Ptr p_inst) {
  Ptr v15528280320 = p_inst;
  Ptr v15528280910 = (Ptr)v15528280320;
  int64_t v15528280dc0 = (int64_t)24ll;
  Ptr v155282806e0 = (Ptr)(v15528280910 + v15528280dc0);
  return v155282806e0;
  v15528280d70_break:
}

Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr) {
  Ptr v1552827fe70 = p_f;
  uint64_t v1552827ff60 = (uint64_t)32llu;
  Fun_155282755d0 v1552827fd80 = fe_malloc;
  Ptr v15528280a50 = v1552827fd80(v1552827fe70, v1552827ff60);
  Ptr l_inst = v15528280a50;
  uint16_t v155282810e0 = p_type;
  Ptr v155282809b0 = l_inst;
  Ptr v15528280050 = &((Struct_15528270ae0*)v155282809b0)->type;
  *(uint16_t*)v15528280050 = v155282810e0;
  uint16_t v15528281270 = p_kind;
  Ptr v15528281b30 = l_inst;
  Ptr v15528281bd0 = &((Struct_15528270ae0*)v15528281b30)->kind;
  *(uint16_t*)v15528281bd0 = v15528281270;
  Ptr v15528281a40 = l_inst;
  Fun_15528275990 v155282819f0 = fe_to_load;
  Ptr v15528281770 = v155282819f0(v15528281a40);
  Ptr l_load = v15528281770;
  Ptr v15528281ae0 = p_ptr;
  Ptr v15528281180 = l_load;
  Ptr v155282813b0 = &((Struct_15528275410*)v15528281180)->ptr;
  *(Ptr*)v155282813b0 = v15528281ae0;
  Ptr v15528281450 = l_inst;
  return v15528281450;
  v15528280960_break:
}

Ptr fe_to_load(Ptr p_inst) {
  Ptr v15528281900 = p_inst;
  Ptr v15528281720 = (Ptr)v15528281900;
  int64_t v15528281950 = (int64_t)24ll;
  Ptr v15528281310 = (Ptr)(v15528281720 + v15528281950);
  return v15528281310;
  v15528281810_break:
}

Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr, Ptr p_val) {
  Ptr v155282838d0 = p_f;
  uint64_t v15528282980 = (uint64_t)40llu;
  Fun_155282755d0 v155282830b0 = fe_malloc;
  Ptr v155282834c0 = v155282830b0(v155282838d0, v15528282980);
  Ptr l_inst = v155282834c0;
  uint16_t v15528283600 = p_type;
  Ptr v15528283100 = l_inst;
  Ptr v15528283920 = &((Struct_15528270ae0*)v15528283100)->type;
  *(uint16_t*)v15528283920 = v15528283600;
  uint16_t v155282828e0 = p_kind;
  Ptr v15528283010 = l_inst;
  Ptr v15528283650 = &((Struct_15528270ae0*)v15528283010)->kind;
  *(uint16_t*)v15528283650 = v155282828e0;
  Ptr v15528283470 = l_inst;
  Fun_15528275350 v15528282840 = fe_to_store;
  Ptr v15528283420 = v15528282840(v15528283470);
  Ptr l_store = v15528283420;
  Ptr v15528282b10 = p_ptr;
  Ptr v15528283290 = l_store;
  Ptr v155282829d0 = &((Struct_15528275a90*)v15528283290)->ptr;
  *(Ptr*)v155282829d0 = v15528282b10;
  Ptr v155282832e0 = p_val;
  Ptr v15528282a20 = l_store;
  Ptr v15528282bb0 = &((Struct_15528275a90*)v15528282a20)->val;
  *(Ptr*)v15528282bb0 = v155282832e0;
  Ptr v155282836a0 = l_inst;
  return v155282836a0;
  v15528283330_break:
}

Ptr fe_to_store(Ptr p_inst) {
  Ptr v15528283880 = p_inst;
  Ptr v15528283150 = (Ptr)v15528283880;
  int64_t v15528282b60 = (int64_t)24ll;
  Ptr v155282831a0 = (Ptr)(v15528283150 + v15528282b60);
  return v155282831a0;
  v15528283a10_break:
}

Ptr fe_new_block(Ptr p_f) {
  Ptr v15528282fc0 = p_f;
  uint64_t v15528283060 = (uint64_t)32llu;
  Fun_155282755d0 v15528282f70 = fe_malloc;
  Ptr v15528282f20 = v15528282f70(v15528282fc0, v15528283060);
  Ptr l_block = v15528282f20;
  Ptr v15528284000 = p_f;
  Ptr v15528284320 = l_block;
  Fun_15528275b50 v155282846e0 = fe_inst_bookend;
  Ptr v15528283dd0 = v155282846e0(v15528284000, v15528284320);
  Ptr l_bookend = v15528283dd0;
  Ptr v15528284140 = l_bookend;
  Ptr v15528284460 = l_block;
  Ptr v15528283f10 = &((Struct_15528275ad0*)v15528284460)->start;
  *(Ptr*)v15528283f10 = v15528284140;
  Ptr v15528284410 = l_bookend;
  Ptr v15528283d30 = l_block;
  Ptr v155282843c0 = &((Struct_15528275ad0*)v15528283d30)->end;
  *(Ptr*)v155282843c0 = v15528284410;
  Ptr v15528284640 = p_f;
  Ptr v15528284500 = l_block;
  Ptr v155282845f0 = &((Struct_15528275ad0*)v15528284500)->function;
  *(Ptr*)v155282845f0 = v15528284640;
  Ptr v15528284230 = l_block;
  return v15528284230;
  v15528283380_break:
}

