#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_1d2fb1600a0 {
  uint16_t kind;
  uint16_t type;
  Ptr next;
  Ptr prev;
} Struct_1d2fb1600a0;
typedef struct Struct_1d2fb1653d0 {
  Ptr block;
} Struct_1d2fb1653d0;
typedef struct Struct_1d2fb166150 {
  Ptr start;
  Ptr end;
  Ptr next_block;
  Ptr function;
} Struct_1d2fb166150;
typedef struct Struct_1d2fb165610 {
  Ptr symbol;
  Ptr first_block;
} Struct_1d2fb165610;
typedef struct Struct_1d2fb165fd0 {
  Slice name;
} Struct_1d2fb165fd0;
typedef struct Struct_1d2fb165990 {
  Ptr sub;
} Struct_1d2fb165990;
typedef struct Struct_1d2fb165c50 {
  Ptr lhs;
  Ptr rhs;
} Struct_1d2fb165c50;
typedef struct Struct_1d2fb1652d0 {
  uint64_t val;
} Struct_1d2fb1652d0;
typedef struct Struct_1d2fb165310 {
  Ptr ptr;
} Struct_1d2fb165310;
typedef struct Struct_1d2fb1660d0 {
  Ptr ptr;
  Ptr val;
} Struct_1d2fb1660d0;
typedef Ptr (*Fun_1d2fb165f50)(Ptr, uint64_t);
typedef Ptr (*Fun_1d2fb165790)(Ptr, Ptr);
typedef Ptr (*Fun_1d2fb165d90)(Ptr, Ptr);
typedef Ptr (*Fun_1d2fb1657d0)(Ptr, Ptr);
typedef Ptr (*Fun_1d2fb165910)(Ptr, uint16_t, uint16_t);
typedef Ptr (*Fun_1d2fb165810)(Ptr);
typedef Ptr (*Fun_1d2fb165cd0)(Ptr, Ptr);
typedef Ptr (*Fun_1d2fb165a10)(Ptr);
typedef Ptr (*Fun_1d2fb165f90)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_1d2fb165a90)(Ptr);
typedef Ptr (*Fun_1d2fb165ad0)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_1d2fb165690)(Ptr);
typedef Ptr (*Fun_1d2fb165510)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_1d2fb165490)(Ptr);
typedef Ptr (*Fun_1d2fb1654d0)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_1d2fb165550)(Ptr);
typedef Ptr (*Fun_1d2fb165b90)(Ptr);

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
  Ptr v1d2fb1668b0 = (Ptr)0llu;
  return v1d2fb1668b0;
  v1d2fb1665e0_break:
}

Ptr fe_append(Ptr p_b, Ptr p_inst) {
  Ptr v1d2fb166d60 = p_b;
  Ptr v1d2fb166d10 = ((Struct_1d2fb166150*)v1d2fb166d60)->start;
  uint16_t v1d2fb166c70 = ((Struct_1d2fb1600a0*)v1d2fb166d10)->kind;
  uint16_t v1d2fb166db0 = (uint16_t)1llu;
  _Bool v1d2fb166ea0 = (_Bool)(v1d2fb166c70 == v1d2fb166db0);
  if (v1d2fb166ea0)
  {
    Ptr v1d2fb167560 = p_b;
    Ptr v1d2fb167e70 = ((Struct_1d2fb166150*)v1d2fb167560)->start;
    Ptr l_bookend = v1d2fb167e70;
    Ptr v1d2fb168460 = l_bookend;
    Ptr v1d2fb1680a0 = p_inst;
    Ptr v1d2fb167ec0 = &((Struct_1d2fb1600a0*)v1d2fb1680a0)->prev;
    *(Ptr*)v1d2fb167ec0 = v1d2fb168460;
    Ptr v1d2fb167510 = l_bookend;
    Ptr v1d2fb167fb0 = p_inst;
    Ptr v1d2fb167790 = &((Struct_1d2fb1600a0*)v1d2fb167fb0)->next;
    *(Ptr*)v1d2fb167790 = v1d2fb167510;
    Ptr v1d2fb167b50 = p_inst;
    Ptr v1d2fb167b00 = l_bookend;
    Ptr v1d2fb167ab0 = &((Struct_1d2fb1600a0*)v1d2fb167b00)->next;
    *(Ptr*)v1d2fb167ab0 = v1d2fb167b50;
    Ptr v1d2fb1679c0 = p_inst;
    Ptr v1d2fb167e20 = l_bookend;
    Ptr v1d2fb168000 = &((Struct_1d2fb1600a0*)v1d2fb167e20)->prev;
    *(Ptr*)v1d2fb168000 = v1d2fb1679c0;
    Ptr v1d2fb1681e0 = p_inst;
    Ptr v1d2fb167bf0 = p_b;
    Ptr v1d2fb168370 = &((Struct_1d2fb166150*)v1d2fb167bf0)->start;
    *(Ptr*)v1d2fb168370 = v1d2fb1681e0;
    Ptr v1d2fb167600 = p_inst;
    Ptr v1d2fb1675b0 = p_b;
    Ptr v1d2fb168050 = &((Struct_1d2fb166150*)v1d2fb1675b0)->end;
    *(Ptr*)v1d2fb168050 = v1d2fb167600;
    v1d2fb1664a0_break:
  }
  else {
    {
      Ptr v1d2fb168140 = p_b;
      Ptr v1d2fb1676f0 = ((Struct_1d2fb166150*)v1d2fb168140)->end;
      Ptr v1d2fb168190 = p_inst;
      Ptr v1d2fb167650 = &((Struct_1d2fb1600a0*)v1d2fb168190)->prev;
      *(Ptr*)v1d2fb167650 = v1d2fb1676f0;
      Ptr v1d2fb1682d0 = p_b;
      Ptr v1d2fb167dd0 = ((Struct_1d2fb166150*)v1d2fb1682d0)->end;
      Ptr v1d2fb1683c0 = ((Struct_1d2fb1600a0*)v1d2fb167dd0)->next;
      Ptr v1d2fb1680f0 = p_inst;
      Ptr v1d2fb168410 = &((Struct_1d2fb1600a0*)v1d2fb1680f0)->next;
      *(Ptr*)v1d2fb168410 = v1d2fb1683c0;
      Ptr v1d2fb168320 = p_inst;
      Ptr v1d2fb167a10 = p_b;
      Ptr v1d2fb168280 = ((Struct_1d2fb166150*)v1d2fb167a10)->end;
      Ptr v1d2fb167ce0 = ((Struct_1d2fb1600a0*)v1d2fb168280)->next;
      Ptr v1d2fb167c90 = &((Struct_1d2fb1600a0*)v1d2fb167ce0)->prev;
      *(Ptr*)v1d2fb167c90 = v1d2fb168320;
      Ptr v1d2fb167920 = p_inst;
      Ptr v1d2fb167830 = p_b;
      Ptr v1d2fb1677e0 = ((Struct_1d2fb166150*)v1d2fb167830)->end;
      Ptr v1d2fb167740 = &((Struct_1d2fb1600a0*)v1d2fb1677e0)->next;
      *(Ptr*)v1d2fb167740 = v1d2fb167920;
      v1d2fb167970_break:
    }
  }
  v1d2fb166c20_break:
  Ptr v1d2fb166ef0 = p_inst;
  return v1d2fb166ef0;
  v1d2fb1667c0_break:
}

Ptr fe_insert_before(Ptr p_new, Ptr p_ref) {
  Ptr v1d2fb168910 = p_ref;
  Ptr v1d2fb168870 = ((Struct_1d2fb1600a0*)v1d2fb168910)->prev;
  uint16_t v1d2fb168ff0 = ((Struct_1d2fb1600a0*)v1d2fb168870)->kind;
  uint16_t v1d2fb1688c0 = (uint16_t)1llu;
  _Bool v1d2fb1687d0 = (_Bool)(v1d2fb168ff0 == v1d2fb1688c0);
  if (v1d2fb1687d0)
  {
    Ptr v1d2fb1693b0 = p_ref;
    Ptr v1d2fb168cd0 = ((Struct_1d2fb1600a0*)v1d2fb1693b0)->prev;
    Ptr v1d2fb168820 = (Ptr)v1d2fb168cd0;
    Ptr l_bookend = v1d2fb168820;
    Ptr v1d2fb168d70 = p_new;
    Ptr v1d2fb1689b0 = l_bookend;
    Ptr v1d2fb168d20 = ((Struct_1d2fb1653d0*)v1d2fb1689b0)->block;
    Ptr v1d2fb1691d0 = &((Struct_1d2fb166150*)v1d2fb168d20)->start;
    *(Ptr*)v1d2fb1691d0 = v1d2fb168d70;
    v1d2fb168be0_break:
  }
  v1d2fb168b90_break:
  Ptr v1d2fb168690 = p_ref;
  Ptr v1d2fb1686e0 = p_new;
  Ptr v1d2fb169400 = &((Struct_1d2fb1600a0*)v1d2fb1686e0)->next;
  *(Ptr*)v1d2fb169400 = v1d2fb168690;
  Ptr v1d2fb168aa0 = p_ref;
  Ptr v1d2fb168dc0 = ((Struct_1d2fb1600a0*)v1d2fb168aa0)->prev;
  Ptr v1d2fb169310 = p_new;
  Ptr v1d2fb168fa0 = &((Struct_1d2fb1600a0*)v1d2fb169310)->prev;
  *(Ptr*)v1d2fb168fa0 = v1d2fb168dc0;
  Ptr v1d2fb169220 = p_new;
  Ptr v1d2fb1694a0 = p_ref;
  Ptr v1d2fb169040 = ((Struct_1d2fb1600a0*)v1d2fb1694a0)->prev;
  Ptr v1d2fb1694f0 = &((Struct_1d2fb1600a0*)v1d2fb169040)->next;
  *(Ptr*)v1d2fb1694f0 = v1d2fb169220;
  Ptr v1d2fb168e10 = p_new;
  Ptr v1d2fb168af0 = p_ref;
  Ptr v1d2fb168730 = &((Struct_1d2fb1600a0*)v1d2fb168af0)->prev;
  *(Ptr*)v1d2fb168730 = v1d2fb168e10;
  Ptr v1d2fb168e60 = p_new;
  return v1d2fb168e60;
  v1d2fb169360_break:
}

Ptr fe_insert_after(Ptr p_new, Ptr p_ref) {
  Ptr v1d2fb16c3b0 = p_ref;
  Ptr v1d2fb16bf00 = ((Struct_1d2fb1600a0*)v1d2fb16c3b0)->prev;
  uint16_t v1d2fb16bc80 = ((Struct_1d2fb1600a0*)v1d2fb16bf00)->kind;
  uint16_t v1d2fb16c360 = (uint16_t)1llu;
  _Bool v1d2fb16baf0 = (_Bool)(v1d2fb16bc80 == v1d2fb16c360);
  if (v1d2fb16baf0)
  {
    Ptr v1d2fb16bb90 = p_ref;
    Ptr v1d2fb16bd20 = ((Struct_1d2fb1600a0*)v1d2fb16bb90)->next;
    Ptr v1d2fb16bb40 = (Ptr)v1d2fb16bd20;
    Ptr l_bookend = v1d2fb16bb40;
    Ptr v1d2fb16bcd0 = p_new;
    Ptr v1d2fb16b7d0 = l_bookend;
    Ptr v1d2fb16be10 = ((Struct_1d2fb1653d0*)v1d2fb16b7d0)->block;
    Ptr v1d2fb16bbe0 = &((Struct_1d2fb166150*)v1d2fb16be10)->end;
    *(Ptr*)v1d2fb16bbe0 = v1d2fb16bcd0;
    v1d2fb16c0e0_break:
  }
  v1d2fb16b960_break:
  Ptr v1d2fb16c1d0 = p_ref;
  Ptr v1d2fb16ba00 = p_new;
  Ptr v1d2fb16c5e0 = &((Struct_1d2fb1600a0*)v1d2fb16ba00)->prev;
  *(Ptr*)v1d2fb16c5e0 = v1d2fb16c1d0;
  Ptr v1d2fb16b910 = p_ref;
  Ptr v1d2fb16c540 = ((Struct_1d2fb1600a0*)v1d2fb16b910)->next;
  Ptr v1d2fb16b820 = p_new;
  Ptr v1d2fb16bff0 = &((Struct_1d2fb1600a0*)v1d2fb16b820)->next;
  *(Ptr*)v1d2fb16bff0 = v1d2fb16c540;
  Ptr v1d2fb16b8c0 = p_new;
  Ptr v1d2fb16b9b0 = p_ref;
  Ptr v1d2fb16c2c0 = ((Struct_1d2fb1600a0*)v1d2fb16b9b0)->next;
  Ptr v1d2fb16c220 = &((Struct_1d2fb1600a0*)v1d2fb16c2c0)->prev;
  *(Ptr*)v1d2fb16c220 = v1d2fb16b8c0;
  Ptr v1d2fb16c040 = p_new;
  Ptr v1d2fb16ba50 = p_ref;
  Ptr v1d2fb16beb0 = &((Struct_1d2fb1600a0*)v1d2fb16ba50)->next;
  *(Ptr*)v1d2fb16beb0 = v1d2fb16c040;
  Ptr v1d2fb16c4a0 = p_new;
  return v1d2fb16c4a0;
  v1d2fb1690e0_break:
}

Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint16_t p_type) {
  Ptr v1d2fb16ccc0 = p_f;
  uint64_t v1d2fb16cc20 = (uint64_t)24llu;
  Fun_1d2fb165f50 v1d2fb16d6c0 = fe_malloc;
  Ptr v1d2fb16d530 = v1d2fb16d6c0(v1d2fb16ccc0, v1d2fb16cc20);
  Ptr l_inst = v1d2fb16d530;
  uint16_t v1d2fb16ca90 = p_type;
  Ptr v1d2fb16d440 = l_inst;
  Ptr v1d2fb16c9a0 = &((Struct_1d2fb1600a0*)v1d2fb16d440)->type;
  *(uint16_t*)v1d2fb16c9a0 = v1d2fb16ca90;
  uint16_t v1d2fb16cc70 = p_kind;
  Ptr v1d2fb16d580 = l_inst;
  Ptr v1d2fb16d490 = &((Struct_1d2fb1600a0*)v1d2fb16d580)->kind;
  *(uint16_t*)v1d2fb16d490 = v1d2fb16cc70;
  Ptr v1d2fb16cea0 = l_inst;
  return v1d2fb16cea0;
  v1d2fb16c590_break:
}

Ptr fe_from_extra(Ptr p_extra) {
  Ptr v1d2fb16d170 = p_extra;
  Ptr v1d2fb16d300 = (Ptr)24llu;
  Ptr v1d2fb16d710 = (Ptr)(v1d2fb16d170 - v1d2fb16d300);
  Ptr v1d2fb16d5d0 = (Ptr)v1d2fb16d710;
  return v1d2fb16d5d0;
  v1d2fb16d120_break:
}

Ptr fe_inst_bookend(Ptr p_f, Ptr p_block) {
  Ptr v1d2fb16c8b0 = p_f;
  uint64_t v1d2fb16c900 = (uint64_t)32llu;
  Fun_1d2fb165f50 v1d2fb16d1c0 = fe_malloc;
  Ptr v1d2fb16d210 = v1d2fb16d1c0(v1d2fb16c8b0, v1d2fb16c900);
  Ptr l_inst = v1d2fb16d210;
  uint16_t v1d2fb16cfe0 = (uint16_t)1llu;
  Ptr v1d2fb16cf90 = l_inst;
  Ptr v1d2fb16c7c0 = &((Struct_1d2fb1600a0*)v1d2fb16cf90)->kind;
  *(uint16_t*)v1d2fb16c7c0 = v1d2fb16cfe0;
  Ptr v1d2fb16e1e0 = l_inst;
  Fun_1d2fb165a10 v1d2fb16dec0 = fe_to_bookend;
  Ptr v1d2fb16e050 = v1d2fb16dec0(v1d2fb16e1e0);
  Ptr l_bookend = v1d2fb16e050;
  Ptr v1d2fb16e230 = p_block;
  Ptr v1d2fb16e4b0 = l_bookend;
  Ptr v1d2fb16da60 = &((Struct_1d2fb1653d0*)v1d2fb16e4b0)->block;
  *(Ptr*)v1d2fb16da60 = v1d2fb16e230;
  Ptr v1d2fb16e3c0 = l_inst;
  return v1d2fb16e3c0;
  v1d2fb16d620_break:
}

Ptr fe_to_bookend(Ptr p_inst) {
  Ptr v1d2fb16d9c0 = p_inst;
  Ptr v1d2fb16e410 = (Ptr)v1d2fb16d9c0;
  int64_t v1d2fb16dd80 = (int64_t)24ll;
  Ptr v1d2fb16e5f0 = (Ptr)(v1d2fb16e410 + v1d2fb16dd80);
  return v1d2fb16e5f0;
  v1d2fb16e5a0_break:
}

Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_sub) {
  Ptr v1d2fb16e460 = p_f;
  uint64_t v1d2fb16e820 = (uint64_t)32llu;
  Fun_1d2fb165f50 v1d2fb16dc40 = fe_malloc;
  Ptr v1d2fb16e320 = v1d2fb16dc40(v1d2fb16e460, v1d2fb16e820);
  Ptr l_inst = v1d2fb16e320;
  uint16_t v1d2fb16e000 = p_type;
  Ptr v1d2fb16dd30 = l_inst;
  Ptr v1d2fb16dba0 = &((Struct_1d2fb1600a0*)v1d2fb16dd30)->type;
  *(uint16_t*)v1d2fb16dba0 = v1d2fb16e000;
  uint16_t v1d2fb16f700 = p_kind;
  Ptr v1d2fb16ee40 = l_inst;
  Ptr v1d2fb16ef80 = &((Struct_1d2fb1600a0*)v1d2fb16ee40)->kind;
  *(uint16_t*)v1d2fb16ef80 = v1d2fb16f700;
  Ptr v1d2fb16f890 = l_inst;
  Fun_1d2fb165a90 v1d2fb16f480 = fe_to_unop;
  Ptr v1d2fb16f390 = v1d2fb16f480(v1d2fb16f890);
  Ptr l_unop = v1d2fb16f390;
  Ptr v1d2fb16f930 = p_sub;
  Ptr v1d2fb16f8e0 = l_unop;
  Ptr v1d2fb16f2f0 = &((Struct_1d2fb165990*)v1d2fb16f8e0)->sub;
  *(Ptr*)v1d2fb16f2f0 = v1d2fb16f930;
  Ptr v1d2fb16f570 = l_inst;
  return v1d2fb16f570;
  v1d2fb16e6e0_break:
}

Ptr fe_to_unop(Ptr p_inst) {
  Ptr v1d2fb16f840 = p_inst;
  Ptr v1d2fb16f1b0 = (Ptr)v1d2fb16f840;
  int64_t v1d2fb16eb70 = (int64_t)24ll;
  Ptr v1d2fb16f430 = (Ptr)(v1d2fb16f1b0 + v1d2fb16eb70);
  return v1d2fb16f430;
  v1d2fb16ee90_break:
}

Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_lhs, Ptr p_rhs) {
  Ptr v1d2fb16f340 = p_f;
  uint64_t v1d2fb1715e0 = (uint64_t)40llu;
  Fun_1d2fb165f50 v1d2fb16f160 = fe_malloc;
  Ptr v1d2fb16ed00 = v1d2fb16f160(v1d2fb16f340, v1d2fb1715e0);
  Ptr l_inst = v1d2fb16ed00;
  uint16_t v1d2fb1710e0 = p_type;
  Ptr v1d2fb171540 = l_inst;
  Ptr v1d2fb171770 = &((Struct_1d2fb1600a0*)v1d2fb171540)->type;
  *(uint16_t*)v1d2fb171770 = v1d2fb1710e0;
  uint16_t v1d2fb171130 = p_kind;
  Ptr v1d2fb171180 = l_inst;
  Ptr v1d2fb1719f0 = &((Struct_1d2fb1600a0*)v1d2fb171180)->kind;
  *(uint16_t*)v1d2fb1719f0 = v1d2fb171130;
  Ptr v1d2fb1712c0 = l_inst;
  Fun_1d2fb165690 v1d2fb171a40 = fe_to_binop;
  Ptr v1d2fb171900 = v1d2fb171a40(v1d2fb1712c0);
  Ptr l_binop = v1d2fb171900;
  Ptr v1d2fb171b80 = p_lhs;
  Ptr v1d2fb171bd0 = l_binop;
  Ptr v1d2fb171310 = &((Struct_1d2fb165c50*)v1d2fb171bd0)->lhs;
  *(Ptr*)v1d2fb171310 = v1d2fb171b80;
  Ptr v1d2fb171400 = p_rhs;
  Ptr v1d2fb1713b0 = l_binop;
  Ptr v1d2fb171360 = &((Struct_1d2fb165c50*)v1d2fb1713b0)->rhs;
  *(Ptr*)v1d2fb171360 = v1d2fb171400;
  Ptr v1d2fb1714a0 = l_inst;
  return v1d2fb1714a0;
  v1d2fb16f250_break:
}

Ptr fe_to_binop(Ptr p_inst) {
  Ptr v1d2fb171b30 = p_inst;
  Ptr v1d2fb1716d0 = (Ptr)v1d2fb171b30;
  int64_t v1d2fb171860 = (int64_t)24ll;
  Ptr v1d2fb171950 = (Ptr)(v1d2fb1716d0 + v1d2fb171860);
  return v1d2fb171950;
  v1d2fb1719a0_break:
}

Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr) {
  Ptr v1d2fb170b90 = p_f;
  uint64_t v1d2fb170be0 = (uint64_t)32llu;
  Fun_1d2fb165f50 v1d2fb170d20 = fe_malloc;
  Ptr v1d2fb170b40 = v1d2fb170d20(v1d2fb170b90, v1d2fb170be0);
  Ptr l_inst = v1d2fb170b40;
  uint16_t v1d2fb170d70 = p_type;
  Ptr v1d2fb1709b0 = l_inst;
  Ptr v1d2fb170cd0 = &((Struct_1d2fb1600a0*)v1d2fb1709b0)->type;
  *(uint16_t*)v1d2fb170cd0 = v1d2fb170d70;
  uint16_t v1d2fb170820 = p_kind;
  Ptr v1d2fb1703c0 = l_inst;
  Ptr v1d2fb16fe70 = &((Struct_1d2fb1600a0*)v1d2fb1703c0)->kind;
  *(uint16_t*)v1d2fb16fe70 = v1d2fb170820;
  Ptr v1d2fb170eb0 = l_inst;
  Fun_1d2fb165490 v1d2fb1708c0 = fe_to_load;
  Ptr v1d2fb16fec0 = v1d2fb1708c0(v1d2fb170eb0);
  Ptr l_load = v1d2fb16fec0;
  Ptr v1d2fb16fe20 = p_ptr;
  Ptr v1d2fb170ff0 = l_load;
  Ptr v1d2fb170f50 = &((Struct_1d2fb165310*)v1d2fb170ff0)->ptr;
  *(Ptr*)v1d2fb170f50 = v1d2fb16fe20;
  Ptr v1d2fb170690 = l_inst;
  return v1d2fb170690;
  v1d2fb170960_break:
}

Ptr fe_to_load(Ptr p_inst) {
  Ptr v1d2fb170640 = p_inst;
  Ptr v1d2fb170a50 = (Ptr)v1d2fb170640;
  int64_t v1d2fb1700a0 = (int64_t)24ll;
  Ptr v1d2fb170e60 = (Ptr)(v1d2fb170a50 + v1d2fb1700a0);
  return v1d2fb170e60;
  v1d2fb171040_break:
}

Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr, Ptr p_val) {
  Ptr v1d2fb174050 = p_f;
  uint64_t v1d2fb174190 = (uint64_t)40llu;
  Fun_1d2fb165f50 v1d2fb174410 = fe_malloc;
  Ptr v1d2fb1743c0 = v1d2fb174410(v1d2fb174050, v1d2fb174190);
  Ptr l_inst = v1d2fb1743c0;
  uint16_t v1d2fb174780 = p_type;
  Ptr v1d2fb173d80 = l_inst;
  Ptr v1d2fb174460 = &((Struct_1d2fb1600a0*)v1d2fb173d80)->type;
  *(uint16_t*)v1d2fb174460 = v1d2fb174780;
  uint16_t v1d2fb174320 = p_kind;
  Ptr v1d2fb174690 = l_inst;
  Ptr v1d2fb174640 = &((Struct_1d2fb1600a0*)v1d2fb174690)->kind;
  *(uint16_t*)v1d2fb174640 = v1d2fb174320;
  Ptr v1d2fb172e30 = l_inst;
  Fun_1d2fb165550 v1d2fb173ec0 = fe_to_store;
  Ptr v1d2fb173c40 = v1d2fb173ec0(v1d2fb172e30);
  Ptr l_store = v1d2fb173c40;
  Ptr v1d2fb172d90 = p_ptr;
  Ptr v1d2fb172f70 = l_store;
  Ptr v1d2fb172a20 = &((Struct_1d2fb1660d0*)v1d2fb172f70)->ptr;
  *(Ptr*)v1d2fb172a20 = v1d2fb172d90;
  Ptr v1d2fb173880 = p_val;
  Ptr v1d2fb1730b0 = l_store;
  Ptr v1d2fb172890 = &((Struct_1d2fb1660d0*)v1d2fb1730b0)->val;
  *(Ptr*)v1d2fb172890 = v1d2fb173880;
  Ptr v1d2fb173b50 = l_inst;
  return v1d2fb173b50;
  v1d2fb1745a0_break:
}

Ptr fe_to_store(Ptr p_inst) {
  Ptr v1d2fb1741e0 = p_inst;
  Ptr v1d2fb1740f0 = (Ptr)v1d2fb1741e0;
  int64_t v1d2fb174230 = (int64_t)24ll;
  Ptr v1d2fb173330 = (Ptr)(v1d2fb1740f0 + v1d2fb174230);
  return v1d2fb173330;
  v1d2fb173e70_break:
}

Ptr fe_new_block(Ptr p_f) {
  Ptr v1d2fb173830 = p_f;
  uint64_t v1d2fb173650 = (uint64_t)32llu;
  Fun_1d2fb165f50 v1d2fb173150 = fe_malloc;
  Ptr v1d2fb172b60 = v1d2fb173150(v1d2fb173830, v1d2fb173650);
  Ptr l_block = v1d2fb172b60;
  Ptr v1d2fb173420 = p_f;
  Ptr v1d2fb1736a0 = l_block;
  Fun_1d2fb165cd0 v1d2fb173b00 = fe_inst_bookend;
  Ptr v1d2fb173470 = v1d2fb173b00(v1d2fb173420, v1d2fb1736a0);
  Ptr l_bookend = v1d2fb173470;
  Ptr v1d2fb1732e0 = l_bookend;
  Ptr v1d2fb173ab0 = l_block;
  Ptr v1d2fb172bb0 = &((Struct_1d2fb166150*)v1d2fb173ab0)->start;
  *(Ptr*)v1d2fb172bb0 = v1d2fb1732e0;
  Ptr v1d2fb173290 = l_bookend;
  Ptr v1d2fb172fc0 = l_block;
  Ptr v1d2fb172c00 = &((Struct_1d2fb166150*)v1d2fb172fc0)->end;
  *(Ptr*)v1d2fb172c00 = v1d2fb173290;
  Ptr v1d2fb1728e0 = p_f;
  Ptr v1d2fb172930 = l_block;
  Ptr v1d2fb173010 = &((Struct_1d2fb166150*)v1d2fb172930)->function;
  *(Ptr*)v1d2fb173010 = v1d2fb1728e0;
  Ptr v1d2fb173510 = l_block;
  return v1d2fb173510;
  v1d2fb173bf0_break:
}

