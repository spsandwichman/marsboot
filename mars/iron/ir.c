#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_2a4183d3940 {
  uint16_t kind;
  uint8_t type;
  Ptr next;
  Ptr prev;
} Struct_2a4183d3940;
typedef struct Struct_2a4183d3c00 {
  Ptr block;
} Struct_2a4183d3c00;
typedef struct Struct_2a4183d4100 {
  Ptr start;
  Ptr end;
  Ptr next_block;
  Ptr function;
} Struct_2a4183d4100;
typedef struct Struct_2a4183d4180 {
  Ptr symbol;
  Ptr first_block;
} Struct_2a4183d4180;
typedef struct Struct_2a4183d4040 {
  Slice name;
} Struct_2a4183d4040;
typedef struct Struct_2a4183d4200 {
  Ptr sub;
} Struct_2a4183d4200;
typedef struct Struct_2a4183d3c40 {
  Ptr lhs;
  Ptr rhs;
} Struct_2a4183d3c40;
typedef struct Struct_2a4183d4540 {
  uint64_t val;
} Struct_2a4183d4540;
typedef struct Struct_2a4183d3c80 {
  Ptr ptr;
} Struct_2a4183d3c80;
typedef struct Struct_2a4183d3f40 {
  Ptr ptr;
  Ptr val;
} Struct_2a4183d3f40;
typedef Ptr (*Fun_2a4183d3d80)(Ptr, uint64_t);
typedef Ptr (*Fun_2a4183d44c0)(Ptr, Ptr);
typedef Ptr (*Fun_2a4183d3dc0)(Ptr, Ptr);
typedef Ptr (*Fun_2a4183d3ec0)(Ptr);
typedef Ptr (*Fun_2a4183d4800)(Ptr, Ptr);
typedef Ptr (*Fun_2a4183d3e80)(Ptr, uint16_t, uint8_t);
typedef Ptr (*Fun_2a4183d3a00)(Ptr, uint64_t);
typedef Ptr (*Fun_2a4183d38c0)(Ptr);
typedef Ptr (*Fun_2a4183d3f00)(Ptr, Ptr);
typedef Ptr (*Fun_2a4183d3ac0)(Ptr, uint64_t);
typedef Ptr (*Fun_2a4183d3fc0)(Ptr, uint16_t, uint8_t, Ptr);
typedef Ptr (*Fun_2a4183d42c0)(Ptr, uint64_t);
typedef Ptr (*Fun_2a4183d4000)(Ptr);
typedef Ptr (*Fun_2a4183d4300)(Ptr, uint16_t, uint8_t, Ptr, Ptr);
typedef Ptr (*Fun_2a4183d4380)(Ptr, uint64_t);
typedef Ptr (*Fun_2a4183d3b80)(Ptr);
typedef Ptr (*Fun_2a4183d4a80)(Ptr, uint16_t, uint8_t, Ptr);
typedef Ptr (*Fun_2a4183d5380)(Ptr, uint64_t);
typedef Ptr (*Fun_2a4183d4ac0)(Ptr);
typedef Ptr (*Fun_2a4183d5740)(Ptr, uint16_t, uint8_t, Ptr, Ptr);
typedef Ptr (*Fun_2a4183d4c00)(Ptr, uint64_t);
typedef Ptr (*Fun_2a4183d54c0)(Ptr);
typedef Ptr (*Fun_2a4183d5340)(Ptr);
typedef Ptr (*Fun_2a4183d5400)(Ptr, uint64_t);

Ptr malloc(Ptr p_f, uint64_t p_size);
Ptr fe_append(Ptr p_b, Ptr p_inst);
Ptr fe_insert_before(Ptr p_new, Ptr p_ref);
Ptr fe_insert_after(Ptr p_new, Ptr p_ref);
Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint8_t p_type);
Ptr fe_from_extra(Ptr p_extra);
Ptr fe_inst_bookend(Ptr p_f, Ptr p_block);
Ptr fe_to_bookend(Ptr p_inst);
Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_sub);
Ptr fe_to_unop(Ptr p_inst);
Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_lhs, Ptr p_rhs);
Ptr fe_to_binop(Ptr p_inst);
Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_ptr);
Ptr fe_to_load(Ptr p_inst);
Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_ptr, Ptr p_val);
Ptr fe_to_store(Ptr p_inst);
Ptr fe_new_block(Ptr p_f);

Ptr fe_append(Ptr p_b, Ptr p_inst) {
  Ptr v2a4183d64f0 = p_b;
  Ptr v2a4183d7440 = ((Struct_2a4183d4100*)v2a4183d64f0)->start;
  uint16_t v2a4183d73f0 = ((Struct_2a4183d3940*)v2a4183d7440)->kind;
  uint16_t v2a4183d65e0 = (uint16_t)1llu;
  _Bool v2a4183d6630 = (_Bool)(v2a4183d73f0 == v2a4183d65e0);
  if (v2a4183d6630)
  {
    Ptr v2a4183d7c30 = p_b;
    Ptr v2a4183d7cd0 = ((Struct_2a4183d4100*)v2a4183d7c30)->start;
    Ptr l_bookend = v2a4183d7cd0;
    Ptr v2a4183d7a00 = l_bookend;
    Ptr v2a4183d79b0 = p_inst;
    Ptr v2a4183d7960 = &((Struct_2a4183d3940*)v2a4183d79b0)->prev;
    *(Ptr*)v2a4183d7960 = v2a4183d7a00;
    Ptr v2a4183d8270 = l_bookend;
    Ptr v2a4183d7a50 = p_inst;
    Ptr v2a4183d8590 = &((Struct_2a4183d3940*)v2a4183d7a50)->next;
    *(Ptr*)v2a4183d8590 = v2a4183d8270;
    Ptr v2a4183d80e0 = p_inst;
    Ptr v2a4183d7aa0 = l_bookend;
    Ptr v2a4183d7820 = &((Struct_2a4183d3940*)v2a4183d7aa0)->next;
    *(Ptr*)v2a4183d7820 = v2a4183d80e0;
    Ptr v2a4183d78c0 = p_inst;
    Ptr v2a4183d85e0 = l_bookend;
    Ptr v2a4183d7870 = &((Struct_2a4183d3940*)v2a4183d85e0)->prev;
    *(Ptr*)v2a4183d7870 = v2a4183d78c0;
    Ptr v2a4183d82c0 = p_inst;
    Ptr v2a4183d8360 = p_b;
    Ptr v2a4183d81d0 = &((Struct_2a4183d4100*)v2a4183d8360)->start;
    *(Ptr*)v2a4183d81d0 = v2a4183d82c0;
    Ptr v2a4183d8450 = p_inst;
    Ptr v2a4183d7b40 = p_b;
    Ptr v2a4183d7780 = &((Struct_2a4183d4100*)v2a4183d7b40)->end;
    *(Ptr*)v2a4183d7780 = v2a4183d8450;
    v2a4183d6720_break:
  }
  else {
    {
      Ptr v2a4183d7be0 = p_b;
      Ptr v2a4183d7d70 = ((Struct_2a4183d4100*)v2a4183d7be0)->end;
      Ptr v2a4183d8400 = p_inst;
      Ptr v2a4183d7b90 = &((Struct_2a4183d3940*)v2a4183d8400)->prev;
      *(Ptr*)v2a4183d7b90 = v2a4183d7d70;
      Ptr v2a4183d8040 = p_b;
      Ptr v2a4183d7f00 = ((Struct_2a4183d4100*)v2a4183d8040)->end;
      Ptr v2a4183d7eb0 = ((Struct_2a4183d3940*)v2a4183d7f00)->next;
      Ptr v2a4183d7e60 = p_inst;
      Ptr v2a4183d7dc0 = &((Struct_2a4183d3940*)v2a4183d7e60)->next;
      *(Ptr*)v2a4183d7dc0 = v2a4183d7eb0;
      Ptr v2a4183d8130 = p_inst;
      Ptr v2a4183d8090 = p_b;
      Ptr v2a4183d7ff0 = ((Struct_2a4183d4100*)v2a4183d8090)->end;
      Ptr v2a4183d7fa0 = ((Struct_2a4183d3940*)v2a4183d7ff0)->next;
      Ptr v2a4183d7f50 = &((Struct_2a4183d3940*)v2a4183d7fa0)->prev;
      *(Ptr*)v2a4183d7f50 = v2a4183d8130;
      Ptr v2a4183d9000 = p_inst;
      Ptr v2a4183d7730 = p_b;
      Ptr v2a4183d8680 = ((Struct_2a4183d4100*)v2a4183d7730)->end;
      Ptr v2a4183d8630 = &((Struct_2a4183d3940*)v2a4183d8680)->next;
      *(Ptr*)v2a4183d8630 = v2a4183d9000;
      v2a4183d7910_break:
    }
  }
  v2a4183d70d0_break:
  Ptr v2a4183d9280 = p_inst;
  return v2a4183d9280;
  v2a4183d7080_break:
}

Ptr fe_insert_before(Ptr p_new, Ptr p_ref) {
  Ptr v2a4183d9370 = p_ref;
  Ptr v2a4183d8a60 = ((Struct_2a4183d3940*)v2a4183d9370)->prev;
  uint16_t v2a4183d95f0 = ((Struct_2a4183d3940*)v2a4183d8a60)->kind;
  uint16_t v2a4183d9190 = (uint16_t)1llu;
  _Bool v2a4183d8ab0 = (_Bool)(v2a4183d95f0 == v2a4183d9190);
  if (v2a4183d8ab0)
  {
    Ptr v2a4183d94b0 = p_ref;
    Ptr v2a4183d8970 = ((Struct_2a4183d3940*)v2a4183d94b0)->prev;
    Fun_2a4183d3ec0 v2a4183d8c90 = fe_to_bookend;
    Ptr v2a4183d8b00 = v2a4183d8c90(v2a4183d8970);
    Ptr l_bookend = v2a4183d8b00;
    Ptr v2a4183d90f0 = p_new;
    Ptr v2a4183d9500 = l_bookend;
    Ptr v2a4183d93c0 = ((Struct_2a4183d3c00*)v2a4183d9500)->block;
    Ptr v2a4183d95a0 = &((Struct_2a4183d4100*)v2a4183d93c0)->start;
    *(Ptr*)v2a4183d95a0 = v2a4183d90f0;
    v2a4183d9320_break:
  }
  v2a4183d8dd0_break:
  Ptr v2a4183d9640 = p_ref;
  Ptr v2a4183d9550 = p_new;
  Ptr v2a4183d9140 = &((Struct_2a4183d3940*)v2a4183d9550)->next;
  *(Ptr*)v2a4183d9140 = v2a4183d9640;
  Ptr v2a4183d8a10 = p_ref;
  Ptr v2a4183d89c0 = ((Struct_2a4183d3940*)v2a4183d8a10)->prev;
  Ptr v2a4183d8f10 = p_new;
  Ptr v2a4183d88d0 = &((Struct_2a4183d3940*)v2a4183d8f10)->prev;
  *(Ptr*)v2a4183d88d0 = v2a4183d89c0;
  Ptr v2a4183d9bd0 = p_new;
  Ptr v2a4183d6cc0 = p_ref;
  Ptr v2a4183d73a0 = ((Struct_2a4183d3940*)v2a4183d6cc0)->prev;
  Ptr v2a4183d8fb0 = &((Struct_2a4183d3940*)v2a4183d73a0)->next;
  *(Ptr*)v2a4183d8fb0 = v2a4183d9bd0;
  Ptr v2a4183da210 = p_new;
  Ptr v2a4183d9f40 = p_ref;
  Ptr v2a4183d9ef0 = &((Struct_2a4183d3940*)v2a4183d9f40)->prev;
  *(Ptr*)v2a4183d9ef0 = v2a4183da210;
  Ptr v2a4183d9d60 = p_new;
  return v2a4183d9d60;
  v2a4183d8ba0_break:
}

Ptr fe_insert_after(Ptr p_new, Ptr p_ref) {
  Ptr v2a4183da2b0 = p_ref;
  Ptr v2a4183da3a0 = ((Struct_2a4183d3940*)v2a4183da2b0)->next;
  uint16_t v2a4183d9cc0 = ((Struct_2a4183d3940*)v2a4183da3a0)->kind;
  uint16_t v2a4183d9fe0 = (uint16_t)1llu;
  _Bool v2a4183da170 = (_Bool)(v2a4183d9cc0 == v2a4183d9fe0);
  if (v2a4183da170)
  {
    Ptr v2a4183da1c0 = p_ref;
    Ptr v2a4183d9e50 = ((Struct_2a4183d3940*)v2a4183da1c0)->next;
    Fun_2a4183d3ec0 v2a4183d9db0 = fe_to_bookend;
    Ptr v2a4183da300 = v2a4183d9db0(v2a4183d9e50);
    Ptr l_bookend = v2a4183da300;
    Ptr v2a4183da3f0 = p_new;
    Ptr v2a4183da350 = l_bookend;
    Ptr v2a4183d9b80 = ((Struct_2a4183d3c00*)v2a4183da350)->block;
    Ptr v2a4183d9e00 = &((Struct_2a4183d4100*)v2a4183d9b80)->end;
    *(Ptr*)v2a4183d9e00 = v2a4183da3f0;
    v2a4183da580_break:
  }
  v2a4183d9d10_break:
  Ptr v2a4183da4e0 = p_ref;
  Ptr v2a4183da490 = p_new;
  Ptr v2a4183d9a40 = &((Struct_2a4183d3940*)v2a4183da490)->prev;
  *(Ptr*)v2a4183d9a40 = v2a4183da4e0;
  Ptr v2a4183da670 = p_ref;
  Ptr v2a4183da5d0 = ((Struct_2a4183d3940*)v2a4183da670)->next;
  Ptr v2a4183da620 = p_new;
  Ptr v2a4183d9c20 = &((Struct_2a4183d3940*)v2a4183da620)->next;
  *(Ptr*)v2a4183d9c20 = v2a4183da5d0;
  Ptr v2a4183da800 = p_new;
  Ptr v2a4183da7b0 = p_ref;
  Ptr v2a4183da710 = ((Struct_2a4183d3940*)v2a4183da7b0)->next;
  Ptr v2a4183da6c0 = &((Struct_2a4183d3940*)v2a4183da710)->prev;
  *(Ptr*)v2a4183da6c0 = v2a4183da800;
  Ptr v2a4183d99f0 = p_new;
  Ptr v2a4183d99a0 = p_ref;
  Ptr v2a4183d9950 = &((Struct_2a4183d3940*)v2a4183d99a0)->next;
  *(Ptr*)v2a4183d9950 = v2a4183d99f0;
  Ptr v2a4183dcd70 = p_new;
  return v2a4183dcd70;
  v2a4183da850_break:
}

Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint8_t p_type) {
  Ptr v2a4183dd450 = p_f;
  uint64_t v2a4183dcf00 = (uint64_t)24llu;
  Fun_2a4183d3a00 v2a4183dceb0 = malloc;
  Ptr v2a4183dd9a0 = v2a4183dceb0(v2a4183dd450, v2a4183dcf00);
  Ptr l_inst = v2a4183dd9a0;
  uint8_t v2a4183dd540 = p_type;
  Ptr v2a4183dcb40 = l_inst;
  Ptr v2a4183dd770 = &((Struct_2a4183d3940*)v2a4183dcb40)->type;
  *(uint8_t*)v2a4183dd770 = v2a4183dd540;
  uint16_t v2a4183dcaf0 = p_kind;
  Ptr v2a4183dd0e0 = l_inst;
  Ptr v2a4183dd8b0 = &((Struct_2a4183d3940*)v2a4183dd0e0)->kind;
  *(uint16_t*)v2a4183dd8b0 = v2a4183dcaf0;
  Ptr v2a4183dd630 = l_inst;
  return v2a4183dd630;
  v2a4183dcdc0_break:
}

Ptr fe_from_extra(Ptr p_extra) {
  Ptr v2a4183dd680 = p_extra;
  Ptr v2a4183dcff0 = (Ptr)24llu;
  Ptr v2a4183dd310 = (Ptr)(v2a4183dd680 - v2a4183dcff0);
  Ptr v2a4183dd360 = (Ptr)v2a4183dd310;
  return v2a4183dd360;
  v2a4183dd220_break:
}

Ptr fe_inst_bookend(Ptr p_f, Ptr p_block) {
  Ptr v2a4183deb30 = p_f;
  uint64_t v2a4183de5e0 = (uint64_t)32llu;
  Fun_2a4183d3ac0 v2a4183de680 = malloc;
  Ptr v2a4183dee50 = v2a4183de680(v2a4183deb30, v2a4183de5e0);
  Ptr l_inst = v2a4183dee50;
  uint16_t v2a4183de040 = (uint16_t)1llu;
  Ptr v2a4183de2c0 = l_inst;
  Ptr v2a4183de9a0 = &((Struct_2a4183d3940*)v2a4183de2c0)->kind;
  *(uint16_t*)v2a4183de9a0 = v2a4183de040;
  Ptr v2a4183decc0 = l_inst;
  Fun_2a4183d3ec0 v2a4183de310 = fe_to_bookend;
  Ptr v2a4183de090 = v2a4183de310(v2a4183decc0);
  Ptr l_bookend = v2a4183de090;
  Ptr v2a4183de770 = p_block;
  Ptr v2a4183de450 = l_bookend;
  Ptr v2a4183dec70 = &((Struct_2a4183d3c00*)v2a4183de450)->block;
  *(Ptr*)v2a4183dec70 = v2a4183de770;
  Ptr v2a4183de590 = l_inst;
  return v2a4183de590;
  v2a4183dd950_break:
}

Ptr fe_to_bookend(Ptr p_inst) {
  Ptr v2a4183d90a0 = p_inst;
  Ptr v2a4183d9460 = (Ptr)v2a4183d90a0;
  int64_t v2a4183d8d30 = (int64_t)24ll;
  Ptr v2a4183d8e20 = (Ptr)(v2a4183d9460 + v2a4183d8d30);
  return v2a4183d8e20;
  v2a4183d8b50_break:
}

Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_sub) {
  Ptr v2a4183dfbc0 = p_f;
  uint64_t v2a4183df850 = (uint64_t)32llu;
  Fun_2a4183d42c0 v2a4183dcaa0 = malloc;
  Ptr v2a4183ddf50 = v2a4183dcaa0(v2a4183dfbc0, v2a4183df850);
  Ptr l_inst = v2a4183ddf50;
  uint8_t v2a4183dfc10 = p_type;
  Ptr v2a4183df530 = l_inst;
  Ptr v2a4183df5d0 = &((Struct_2a4183d3940*)v2a4183df530)->type;
  *(uint8_t*)v2a4183df5d0 = v2a4183dfc10;
  uint16_t v2a4183dfa30 = p_kind;
  Ptr v2a4183dfd50 = l_inst;
  Ptr v2a4183dfc60 = &((Struct_2a4183d3940*)v2a4183dfd50)->kind;
  *(uint16_t*)v2a4183dfc60 = v2a4183dfa30;
  Ptr v2a4183dfe90 = l_inst;
  Fun_2a4183d4000 v2a4183df940 = fe_to_unop;
  Ptr v2a4183df350 = v2a4183df940(v2a4183dfe90);
  Ptr l_unop = v2a4183df350;
  Ptr v2a4183dfb20 = p_sub;
  Ptr v2a4183df7b0 = l_unop;
  Ptr v2a4183df710 = &((Struct_2a4183d4200*)v2a4183df7b0)->sub;
  *(Ptr*)v2a4183df710 = v2a4183dfb20;
  Ptr v2a4183df760 = l_inst;
  return v2a4183df760;
  v2a4183de4f0_break:
}

Ptr fe_to_unop(Ptr p_inst) {
  Ptr v2a4183dfa80 = p_inst;
  Ptr v2a4183dfcb0 = (Ptr)v2a4183dfa80;
  int64_t v2a4183df490 = (int64_t)24ll;
  Ptr v2a4183dfee0 = (Ptr)(v2a4183dfcb0 + v2a4183df490);
  return v2a4183dfee0;
  v2a4183df670_break:
}

Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_lhs, Ptr p_rhs) {
  Ptr v2a4183e1ea0 = p_f;
  uint64_t v2a4183e15e0 = (uint64_t)40llu;
  Fun_2a4183d4380 v2a4183e1770 = malloc;
  Ptr v2a4183e1810 = v2a4183e1770(v2a4183e1ea0, v2a4183e15e0);
  Ptr l_inst = v2a4183e1810;
  uint8_t v2a4183e1130 = p_type;
  Ptr v2a4183e1360 = l_inst;
  Ptr v2a4183e1400 = &((Struct_2a4183d3940*)v2a4183e1360)->type;
  *(uint8_t*)v2a4183e1400 = v2a4183e1130;
  uint16_t v2a4183e2030 = p_kind;
  Ptr v2a4183e1040 = l_inst;
  Ptr v2a4183e0ff0 = &((Struct_2a4183d3940*)v2a4183e1040)->kind;
  *(uint16_t*)v2a4183e0ff0 = v2a4183e2030;
  Ptr v2a4183e1b80 = l_inst;
  Fun_2a4183d3b80 v2a4183e1ef0 = fe_to_binop;
  Ptr v2a4183e1b30 = v2a4183e1ef0(v2a4183e1b80);
  Ptr l_binop = v2a4183e1b30;
  Ptr v2a4183e1630 = p_lhs;
  Ptr v2a4183e18b0 = l_binop;
  Ptr v2a4183e0eb0 = &((Struct_2a4183d3c40*)v2a4183e18b0)->lhs;
  *(Ptr*)v2a4183e0eb0 = v2a4183e1630;
  Ptr v2a4183e1540 = p_rhs;
  Ptr v2a4183e10e0 = l_binop;
  Ptr v2a4183e14f0 = &((Struct_2a4183d3c40*)v2a4183e10e0)->rhs;
  *(Ptr*)v2a4183e14f0 = v2a4183e1540;
  Ptr v2a4183e1680 = l_inst;
  return v2a4183e1680;
  v2a4183de8b0_break:
}

Ptr fe_to_binop(Ptr p_inst) {
  Ptr v2a4183e1a40 = p_inst;
  Ptr v2a4183e1a90 = (Ptr)v2a4183e1a40;
  int64_t v2a4183e1f40 = (int64_t)24ll;
  Ptr v2a4183e0d70 = (Ptr)(v2a4183e1a90 + v2a4183e1f40);
  return v2a4183e0d70;
  v2a4183e0e60_break:
}

Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_ptr) {
  Ptr v2a4183e2670 = p_f;
  uint64_t v2a4183e28a0 = (uint64_t)32llu;
  Fun_2a4183d5380 v2a4183e23f0 = malloc;
  Ptr v2a4183e2210 = v2a4183e23f0(v2a4183e2670, v2a4183e28a0);
  Ptr l_inst = v2a4183e2210;
  uint8_t v2a4183e2170 = p_type;
  Ptr v2a4183e2a30 = l_inst;
  Ptr v2a4183e2990 = &((Struct_2a4183d3940*)v2a4183e2a30)->type;
  *(uint8_t*)v2a4183e2990 = v2a4183e2170;
  uint16_t v2a4183e2a80 = p_kind;
  Ptr v2a4183e2940 = l_inst;
  Ptr v2a4183e2ad0 = &((Struct_2a4183d3940*)v2a4183e2940)->kind;
  *(uint16_t*)v2a4183e2ad0 = v2a4183e2a80;
  Ptr v2a4183e5dc0 = l_inst;
  Fun_2a4183d4ac0 v2a4183e2760 = fe_to_load;
  Ptr v2a4183e2800 = v2a4183e2760(v2a4183e5dc0);
  Ptr l_load = v2a4183e2800;
  Ptr v2a4183e5820 = p_ptr;
  Ptr v2a4183e5960 = l_load;
  Ptr v2a4183e5b40 = &((Struct_2a4183d3c80*)v2a4183e5960)->ptr;
  *(Ptr*)v2a4183e5b40 = v2a4183e5820;
  Ptr v2a4183e5f00 = l_inst;
  return v2a4183e5f00;
  v2a4183e1310_break:
}

Ptr fe_to_load(Ptr p_inst) {
  Ptr v2a4183e25d0 = p_inst;
  Ptr v2a4183e24e0 = (Ptr)v2a4183e25d0;
  int64_t v2a4183e2120 = (int64_t)24ll;
  Ptr v2a4183e2490 = (Ptr)(v2a4183e24e0 + v2a4183e2120);
  return v2a4183e2490;
  v2a4183e20d0_break:
}

Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_ptr, Ptr p_val) {
  Ptr v2a4183e5b90 = p_f;
  uint64_t v2a4183e5910 = (uint64_t)40llu;
  Fun_2a4183d4c00 v2a4183e5a50 = malloc;
  Ptr v2a4183e5d70 = v2a4183e5a50(v2a4183e5b90, v2a4183e5910);
  Ptr l_inst = v2a4183e5d70;
  uint8_t v2a4183e4b50 = p_type;
  Ptr v2a4183e5230 = l_inst;
  Ptr v2a4183e57d0 = &((Struct_2a4183d3940*)v2a4183e5230)->type;
  *(uint8_t*)v2a4183e57d0 = v2a4183e4b50;
  uint16_t v2a4183e41f0 = p_kind;
  Ptr v2a4183e5140 = l_inst;
  Ptr v2a4183e4560 = &((Struct_2a4183d3940*)v2a4183e5140)->kind;
  *(uint16_t*)v2a4183e4560 = v2a4183e41f0;
  Ptr v2a4183e4880 = l_inst;
  Fun_2a4183d54c0 v2a4183e4380 = fe_to_store;
  Ptr v2a4183e4100 = v2a4183e4380(v2a4183e4880);
  Ptr l_store = v2a4183e4100;
  Ptr v2a4183e5000 = p_ptr;
  Ptr v2a4183e4f60 = l_store;
  Ptr v2a4183e4ba0 = &((Struct_2a4183d3f40*)v2a4183e4f60)->ptr;
  *(Ptr*)v2a4183e4ba0 = v2a4183e5000;
  Ptr v2a4183e4c40 = p_val;
  Ptr v2a4183e45b0 = l_store;
  Ptr v2a4183e4240 = &((Struct_2a4183d3f40*)v2a4183e45b0)->val;
  *(Ptr*)v2a4183e4240 = v2a4183e4c40;
  Ptr v2a4183e4d30 = l_inst;
  return v2a4183e4d30;
  v2a4183e56e0_break:
}

Ptr fe_to_store(Ptr p_inst) {
  Ptr v2a4183e50a0 = p_inst;
  Ptr v2a4183e48d0 = (Ptr)v2a4183e50a0;
  int64_t v2a4183e4830 = (int64_t)24ll;
  Ptr v2a4183e4740 = (Ptr)(v2a4183e48d0 + v2a4183e4830);
  return v2a4183e4740;
  v2a4183e5280_break:
}

Ptr fe_new_block(Ptr p_f) {
  Ptr v2a4183e46f0 = p_f;
  uint64_t v2a4183e53c0 = (uint64_t)32llu;
  Fun_2a4183d5400 v2a4183e46a0 = malloc;
  Ptr v2a4183e4b00 = v2a4183e46a0(v2a4183e46f0, v2a4183e53c0);
  Ptr l_block = v2a4183e4b00;
  Ptr v2a4183e4920 = p_f;
  Ptr v2a4183e4a10 = l_block;
  Fun_2a4183d3f00 v2a4183e4d80 = fe_inst_bookend;
  Ptr v2a4183e47e0 = v2a4183e4d80(v2a4183e4920, v2a4183e4a10);
  Ptr l_bookend = v2a4183e47e0;
  Ptr v2a4183e4c90 = l_bookend;
  Ptr v2a4183e4ab0 = l_block;
  Ptr v2a4183e4a60 = &((Struct_2a4183d4100*)v2a4183e4ab0)->start;
  *(Ptr*)v2a4183e4a60 = v2a4183e4c90;
  Ptr v2a4183e6820 = l_bookend;
  Ptr v2a4183e4ec0 = l_block;
  Ptr v2a4183e4e20 = &((Struct_2a4183d4100*)v2a4183e4ec0)->end;
  *(Ptr*)v2a4183e4e20 = v2a4183e6820;
  Ptr v2a4183e6b90 = p_f;
  Ptr v2a4183e6e60 = l_block;
  Ptr v2a4183e6f00 = &((Struct_2a4183d4100*)v2a4183e6e60)->function;
  *(Ptr*)v2a4183e6f00 = v2a4183e6b90;
  Ptr v2a4183e6be0 = l_block;
  return v2a4183e6be0;
  v2a4183e4470_break:
}

