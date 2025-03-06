#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_2578a050440 {
  uint16_t kind;
  uint16_t type;
  Ptr next;
  Ptr prev;
} Struct_2578a050440;
typedef struct Struct_2578a0553e0 {
  Ptr block;
} Struct_2578a0553e0;
typedef struct Struct_2578a0555a0 {
  Ptr start;
  Ptr end;
  Ptr next_block;
  Ptr function;
} Struct_2578a0555a0;
typedef struct Struct_2578a0555e0 {
  Ptr symbol;
  Ptr first_block;
} Struct_2578a0555e0;
typedef struct Struct_2578a055de0 {
  Slice name;
} Struct_2578a055de0;
typedef struct Struct_2578a0556a0 {
  Ptr sub;
} Struct_2578a0556a0;
typedef struct Struct_2578a055620 {
  Ptr lhs;
  Ptr rhs;
} Struct_2578a055620;
typedef struct Struct_2578a055ce0 {
  uint64_t val;
} Struct_2578a055ce0;
typedef struct Struct_2578a0556e0 {
  Ptr ptr;
} Struct_2578a0556e0;
typedef struct Struct_2578a055e20 {
  Ptr ptr;
  Ptr val;
} Struct_2578a055e20;
typedef Ptr (*Fun_2578a055c60)(Ptr, uint64_t);
typedef Ptr (*Fun_2578a055a60)(Ptr, Ptr);
typedef Ptr (*Fun_2578a055d20)(Ptr, Ptr);
typedef Ptr (*Fun_2578a0554e0)(Ptr);
typedef Ptr (*Fun_2578a055ea0)(Ptr, Ptr);
typedef Ptr (*Fun_2578a055720)(Ptr, uint16_t, uint16_t);
typedef Ptr (*Fun_2578a055f60)(Ptr, uint64_t);
typedef Ptr (*Fun_2578a055fa0)(Ptr);
typedef Ptr (*Fun_2578a055820)(Ptr, Ptr);
typedef Ptr (*Fun_2578a055860)(Ptr, uint64_t);
typedef Ptr (*Fun_2578a055460)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_2578a055760)(Ptr, uint64_t);
typedef Ptr (*Fun_2578a056060)(Ptr);
typedef Ptr (*Fun_2578a0558a0)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_2578a0558e0)(Ptr, uint64_t);
typedef Ptr (*Fun_2578a0560a0)(Ptr);
typedef Ptr (*Fun_2578a055960)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_2578a0559a0)(Ptr, uint64_t);
typedef Ptr (*Fun_2578a055ae0)(Ptr);
typedef Ptr (*Fun_2578a055a20)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_2578a056120)(Ptr, uint64_t);
typedef Ptr (*Fun_2578a055b60)(Ptr);
typedef Ptr (*Fun_2578a056160)(Ptr);
typedef Ptr (*Fun_2578a054da0)(Ptr, uint64_t);

Ptr malloc(Ptr p_f, uint64_t p_size);
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

Ptr fe_append(Ptr p_b, Ptr p_inst) {
  Ptr v2578a0567f0 = p_b;
  Ptr v2578a057290 = ((Struct_2578a0555a0*)v2578a0567f0)->start;
  uint16_t v2578a057240 = ((Struct_2578a050440*)v2578a057290)->kind;
  uint16_t v2578a056d40 = (uint16_t)1llu;
  _Bool v2578a057330 = (_Bool)(v2578a057240 == v2578a056d40);
  if (v2578a057330)
  {
    Ptr v2578a057380 = p_b;
    Ptr v2578a056ed0 = ((Struct_2578a0555a0*)v2578a057380)->start;
    Ptr l_bookend = v2578a056ed0;
    Ptr v2578a057d10 = l_bookend;
    Ptr v2578a058490 = p_inst;
    Ptr v2578a056cf0 = &((Struct_2578a050440*)v2578a058490)->prev;
    *(Ptr*)v2578a056cf0 = v2578a057d10;
    Ptr v2578a0584e0 = l_bookend;
    Ptr v2578a057ef0 = p_inst;
    Ptr v2578a057e00 = &((Struct_2578a050440*)v2578a057ef0)->next;
    *(Ptr*)v2578a057e00 = v2578a0584e0;
    Ptr v2578a058440 = p_inst;
    Ptr v2578a057ea0 = l_bookend;
    Ptr v2578a058030 = &((Struct_2578a050440*)v2578a057ea0)->next;
    *(Ptr*)v2578a058030 = v2578a058440;
    Ptr v2578a0579a0 = p_inst;
    Ptr v2578a057720 = l_bookend;
    Ptr v2578a058080 = &((Struct_2578a050440*)v2578a057720)->prev;
    *(Ptr*)v2578a058080 = v2578a0579a0;
    Ptr v2578a0577c0 = p_inst;
    Ptr v2578a057fe0 = p_b;
    Ptr v2578a058530 = &((Struct_2578a0555a0*)v2578a057fe0)->start;
    *(Ptr*)v2578a058530 = v2578a0577c0;
    Ptr v2578a057bd0 = p_inst;
    Ptr v2578a057630 = p_b;
    Ptr v2578a0582b0 = &((Struct_2578a0555a0*)v2578a057630)->end;
    *(Ptr*)v2578a0582b0 = v2578a057bd0;
    v2578a056840_break:
  }
  else {
    {
      Ptr v2578a057cc0 = p_b;
      Ptr v2578a057c70 = ((Struct_2578a0555a0*)v2578a057cc0)->end;
      Ptr v2578a0580d0 = p_inst;
      Ptr v2578a057b30 = &((Struct_2578a050440*)v2578a0580d0)->prev;
      *(Ptr*)v2578a057b30 = v2578a057c70;
      Ptr v2578a058300 = p_b;
      Ptr v2578a057680 = ((Struct_2578a0555a0*)v2578a058300)->end;
      Ptr v2578a057b80 = ((Struct_2578a050440*)v2578a057680)->next;
      Ptr v2578a0583f0 = p_inst;
      Ptr v2578a057e50 = &((Struct_2578a050440*)v2578a0583f0)->next;
      *(Ptr*)v2578a057e50 = v2578a057b80;
      Ptr v2578a058120 = p_inst;
      Ptr v2578a0581c0 = p_b;
      Ptr v2578a057f40 = ((Struct_2578a0555a0*)v2578a0581c0)->end;
      Ptr v2578a057810 = ((Struct_2578a050440*)v2578a057f40)->next;
      Ptr v2578a058260 = &((Struct_2578a050440*)v2578a057810)->prev;
      *(Ptr*)v2578a058260 = v2578a058120;
      Ptr v2578a057950 = p_inst;
      Ptr v2578a057900 = p_b;
      Ptr v2578a0578b0 = ((Struct_2578a0555a0*)v2578a057900)->end;
      Ptr v2578a058210 = &((Struct_2578a050440*)v2578a0578b0)->next;
      *(Ptr*)v2578a058210 = v2578a057950;
      v2578a057f90_break:
    }
  }
  v2578a056700_break:
  Ptr v2578a0579f0 = p_inst;
  return v2578a0579f0;
  v2578a056d90_break:
}

Ptr fe_insert_before(Ptr p_new, Ptr p_ref) {
  Ptr v2578a058f30 = p_ref;
  Ptr v2578a058990 = ((Struct_2578a050440*)v2578a058f30)->prev;
  uint16_t v2578a058ee0 = ((Struct_2578a050440*)v2578a058990)->kind;
  uint16_t v2578a058a80 = (uint16_t)1llu;
  _Bool v2578a058e90 = (_Bool)(v2578a058ee0 == v2578a058a80);
  if (v2578a058e90)
  {
    Ptr v2578a058f80 = p_ref;
    Ptr v2578a058cb0 = ((Struct_2578a050440*)v2578a058f80)->prev;
    Fun_2578a0554e0 v2578a058fd0 = fe_to_bookend;
    Ptr v2578a058df0 = v2578a058fd0(v2578a058cb0);
    Ptr l_bookend = v2578a058df0;
    Ptr v2578a059390 = p_new;
    Ptr v2578a0590c0 = l_bookend;
    Ptr v2578a059070 = ((Struct_2578a0553e0*)v2578a0590c0)->block;
    Ptr v2578a058bc0 = &((Struct_2578a0555a0*)v2578a059070)->start;
    *(Ptr*)v2578a058bc0 = v2578a059390;
    v2578a059110_break:
  }
  v2578a059020_break:
  Ptr v2578a0593e0 = p_ref;
  Ptr v2578a0592f0 = p_new;
  Ptr v2578a0592a0 = &((Struct_2578a050440*)v2578a0592f0)->next;
  *(Ptr*)v2578a0592a0 = v2578a0593e0;
  Ptr v2578a059480 = p_ref;
  Ptr v2578a058e40 = ((Struct_2578a050440*)v2578a059480)->prev;
  Ptr v2578a059430 = p_new;
  Ptr v2578a0589e0 = &((Struct_2578a050440*)v2578a059430)->prev;
  *(Ptr*)v2578a0589e0 = v2578a058e40;
  Ptr v2578a0587b0 = p_new;
  Ptr v2578a058760 = p_ref;
  Ptr v2578a0588a0 = ((Struct_2578a050440*)v2578a058760)->prev;
  Ptr v2578a059570 = &((Struct_2578a050440*)v2578a0588a0)->next;
  *(Ptr*)v2578a059570 = v2578a0587b0;
  Ptr v2578a058b20 = p_new;
  Ptr v2578a058a30 = p_ref;
  Ptr v2578a058850 = &((Struct_2578a050440*)v2578a058a30)->prev;
  *(Ptr*)v2578a058850 = v2578a058b20;
  Ptr v2578a05c340 = p_new;
  return v2578a05c340;
  v2578a0588f0_break:
}

Ptr fe_insert_after(Ptr p_new, Ptr p_ref) {
  Ptr v2578a05c520 = p_ref;
  Ptr v2578a05bb70 = ((Struct_2578a050440*)v2578a05c520)->next;
  uint16_t v2578a05b850 = ((Struct_2578a050440*)v2578a05bb70)->kind;
  uint16_t v2578a05c390 = (uint16_t)1llu;
  _Bool v2578a05bdf0 = (_Bool)(v2578a05b850 == v2578a05c390);
  if (v2578a05bdf0)
  {
    Ptr v2578a05ba30 = p_ref;
    Ptr v2578a05b8f0 = ((Struct_2578a050440*)v2578a05ba30)->next;
    Fun_2578a0554e0 v2578a05be40 = fe_to_bookend;
    Ptr v2578a05c020 = v2578a05be40(v2578a05b8f0);
    Ptr l_bookend = v2578a05c020;
    Ptr v2578a05b990 = p_new;
    Ptr v2578a05b940 = l_bookend;
    Ptr v2578a05c110 = ((Struct_2578a0553e0*)v2578a05b940)->block;
    Ptr v2578a05bee0 = &((Struct_2578a0555a0*)v2578a05c110)->end;
    *(Ptr*)v2578a05bee0 = v2578a05b990;
    v2578a05c3e0_break:
  }
  v2578a05bad0_break:
  Ptr v2578a05c4d0 = p_ref;
  Ptr v2578a05c430 = p_new;
  Ptr v2578a05c160 = &((Struct_2578a050440*)v2578a05c430)->prev;
  *(Ptr*)v2578a05c160 = v2578a05c4d0;
  Ptr v2578a05bc60 = p_ref;
  Ptr v2578a05bb20 = ((Struct_2578a050440*)v2578a05bc60)->next;
  Ptr v2578a05bbc0 = p_new;
  Ptr v2578a05bda0 = &((Struct_2578a050440*)v2578a05bbc0)->next;
  *(Ptr*)v2578a05bda0 = v2578a05bb20;
  Ptr v2578a05c700 = p_new;
  Ptr v2578a05c1b0 = p_ref;
  Ptr v2578a05bc10 = ((Struct_2578a050440*)v2578a05c1b0)->next;
  Ptr v2578a05c6b0 = &((Struct_2578a050440*)v2578a05bc10)->prev;
  *(Ptr*)v2578a05c6b0 = v2578a05c700;
  Ptr v2578a05c250 = p_new;
  Ptr v2578a05c200 = p_ref;
  Ptr v2578a05bcb0 = &((Struct_2578a050440*)v2578a05c200)->next;
  *(Ptr*)v2578a05bcb0 = v2578a05c250;
  Ptr v2578a05b8a0 = p_new;
  return v2578a05b8a0;
  v2578a05c660_break:
}

Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint16_t p_type) {
  Ptr v2578a05d040 = p_f;
  uint64_t v2578a05cdc0 = (uint64_t)24llu;
  Fun_2578a055f60 v2578a05cff0 = malloc;
  Ptr v2578a05d810 = v2578a05cff0(v2578a05d040, v2578a05cdc0);
  Ptr l_inst = v2578a05d810;
  uint16_t v2578a05d590 = p_type;
  Ptr v2578a05d540 = l_inst;
  Ptr v2578a05d860 = &((Struct_2578a050440*)v2578a05d540)->type;
  *(uint16_t*)v2578a05d860 = v2578a05d590;
  uint16_t v2578a05d3b0 = p_kind;
  Ptr v2578a05d0e0 = l_inst;
  Ptr v2578a05d6d0 = &((Struct_2578a050440*)v2578a05d0e0)->kind;
  *(uint16_t*)v2578a05d6d0 = v2578a05d3b0;
  Ptr v2578a05d8b0 = l_inst;
  return v2578a05d8b0;
  v2578a05d680_break:
}

Ptr fe_from_extra(Ptr p_extra) {
  Ptr v2578a05caf0 = p_extra;
  Ptr v2578a05d400 = (Ptr)24llu;
  Ptr v2578a05d220 = (Ptr)(v2578a05caf0 - v2578a05d400);
  Ptr v2578a05cbe0 = (Ptr)v2578a05d220;
  return v2578a05cbe0;
  v2578a05ca50_break:
}

Ptr fe_inst_bookend(Ptr p_f, Ptr p_block) {
  Ptr v2578a05dfa0 = p_f;
  uint64_t v2578a05e310 = (uint64_t)32llu;
  Fun_2578a055860 v2578a05e810 = malloc;
  Ptr v2578a05ce60 = v2578a05e810(v2578a05dfa0, v2578a05e310);
  Ptr l_inst = v2578a05ce60;
  uint16_t v2578a05e220 = (uint16_t)1llu;
  Ptr v2578a05e8b0 = l_inst;
  Ptr v2578a05ddc0 = &((Struct_2578a050440*)v2578a05e8b0)->kind;
  *(uint16_t*)v2578a05ddc0 = v2578a05e220;
  Ptr v2578a05e270 = l_inst;
  Fun_2578a0554e0 v2578a05ec70 = fe_to_bookend;
  Ptr v2578a05e5e0 = v2578a05ec70(v2578a05e270);
  Ptr l_bookend = v2578a05e5e0;
  Ptr v2578a05deb0 = p_block;
  Ptr v2578a05e180 = l_bookend;
  Ptr v2578a05ea40 = &((Struct_2578a0553e0*)v2578a05e180)->block;
  *(Ptr*)v2578a05ea40 = v2578a05deb0;
  Ptr v2578a05e950 = l_inst;
  return v2578a05e950;
  v2578a05d270_break:
}

Ptr fe_to_bookend(Ptr p_inst) {
  Ptr v2578a059520 = p_inst;
  Ptr v2578a0591b0 = (Ptr)v2578a059520;
  int64_t v2578a058710 = (int64_t)24ll;
  Ptr v2578a058b70 = (Ptr)(v2578a0591b0 + v2578a058710);
  return v2578a058b70;
  v2578a059340_break:
}

Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_sub) {
  Ptr v2578a05e130 = p_f;
  uint64_t v2578a05fe30 = (uint64_t)32llu;
  Fun_2578a055760 v2578a05eb30 = malloc;
  Ptr v2578a05e9a0 = v2578a05eb30(v2578a05e130, v2578a05fe30);
  Ptr l_inst = v2578a05e9a0;
  uint16_t v2578a05f520 = p_type;
  Ptr v2578a05f980 = l_inst;
  Ptr v2578a05fe80 = &((Struct_2578a050440*)v2578a05f980)->type;
  *(uint16_t*)v2578a05fe80 = v2578a05f520;
  uint16_t v2578a05fd90 = p_kind;
  Ptr v2578a05fed0 = l_inst;
  Ptr v2578a05f2f0 = &((Struct_2578a050440*)v2578a05fed0)->kind;
  *(uint16_t*)v2578a05f2f0 = v2578a05fd90;
  Ptr v2578a05f1b0 = l_inst;
  Fun_2578a056060 v2578a05f020 = fe_to_unop;
  Ptr v2578a05ff20 = v2578a05f020(v2578a05f1b0);
  Ptr l_unop = v2578a05ff20;
  Ptr v2578a05f340 = p_sub;
  Ptr v2578a05fa70 = l_unop;
  Ptr v2578a05fc50 = &((Struct_2578a0556a0*)v2578a05fa70)->sub;
  *(Ptr*)v2578a05fc50 = v2578a05f340;
  Ptr v2578a05f9d0 = l_inst;
  return v2578a05f9d0;
  v2578a05e3b0_break:
}

Ptr fe_to_unop(Ptr p_inst) {
  Ptr v2578a05fca0 = p_inst;
  Ptr v2578a05f700 = (Ptr)v2578a05fca0;
  int64_t v2578a05fa20 = (int64_t)24ll;
  Ptr v2578a05f200 = (Ptr)(v2578a05f700 + v2578a05fa20);
  return v2578a05f200;
  v2578a05f0c0_break:
}

Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_lhs, Ptr p_rhs) {
  Ptr v2578a060ee0 = p_f;
  uint64_t v2578a0612a0 = (uint64_t)40llu;
  Fun_2578a0558e0 v2578a061d40 = malloc;
  Ptr v2578a05dff0 = v2578a061d40(v2578a060ee0, v2578a0612a0);
  Ptr l_inst = v2578a05dff0;
  uint16_t v2578a060c10 = p_type;
  Ptr v2578a061980 = l_inst;
  Ptr v2578a061b10 = &((Struct_2578a050440*)v2578a061980)->type;
  *(uint16_t*)v2578a061b10 = v2578a060c10;
  uint16_t v2578a060c60 = p_kind;
  Ptr v2578a061200 = l_inst;
  Ptr v2578a061570 = &((Struct_2578a050440*)v2578a061200)->kind;
  *(uint16_t*)v2578a061570 = v2578a060c60;
  Ptr v2578a061390 = l_inst;
  Fun_2578a0560a0 v2578a061890 = fe_to_binop;
  Ptr v2578a061340 = v2578a061890(v2578a061390);
  Ptr l_binop = v2578a061340;
  Ptr v2578a061a70 = p_lhs;
  Ptr v2578a0617a0 = l_binop;
  Ptr v2578a0613e0 = &((Struct_2578a055620*)v2578a0617a0)->lhs;
  *(Ptr*)v2578a0613e0 = v2578a061a70;
  Ptr v2578a060fd0 = p_rhs;
  Ptr v2578a060cb0 = l_binop;
  Ptr v2578a061f20 = &((Struct_2578a055620*)v2578a060cb0)->rhs;
  *(Ptr*)v2578a061f20 = v2578a060fd0;
  Ptr v2578a061de0 = l_inst;
  return v2578a061de0;
  v2578a05f750_break:
}

Ptr fe_to_binop(Ptr p_inst) {
  Ptr v2578a0616b0 = p_inst;
  Ptr v2578a061a20 = (Ptr)v2578a0616b0;
  int64_t v2578a061e80 = (int64_t)24ll;
  Ptr v2578a060f30 = (Ptr)(v2578a061a20 + v2578a061e80);
  return v2578a060f30;
  v2578a0615c0_break:
}

Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr) {
  Ptr v2578a062830 = p_f;
  uint64_t v2578a062a10 = (uint64_t)32llu;
  Fun_2578a0559a0 v2578a0625b0 = malloc;
  Ptr v2578a061070 = v2578a0625b0(v2578a062830, v2578a062a10);
  Ptr l_inst = v2578a061070;
  uint16_t v2578a062ab0 = p_type;
  Ptr v2578a0626a0 = l_inst;
  Ptr v2578a062650 = &((Struct_2578a050440*)v2578a0626a0)->type;
  *(uint16_t*)v2578a062650 = v2578a062ab0;
  uint16_t v2578a0621f0 = p_kind;
  Ptr v2578a062740 = l_inst;
  Ptr v2578a0626f0 = &((Struct_2578a050440*)v2578a062740)->kind;
  *(uint16_t*)v2578a0626f0 = v2578a0621f0;
  Ptr v2578a0622e0 = l_inst;
  Fun_2578a055ae0 v2578a062470 = fe_to_load;
  Ptr v2578a062880 = v2578a062470(v2578a0622e0);
  Ptr l_load = v2578a062880;
  Ptr v2578a05fcf0 = p_ptr;
  Ptr v2578a062380 = l_load;
  Ptr v2578a062330 = &((Struct_2578a0556e0*)v2578a062380)->ptr;
  *(Ptr*)v2578a062330 = v2578a05fcf0;
  Ptr v2578a065740 = l_inst;
  return v2578a065740;
  v2578a061c00_break:
}

Ptr fe_to_load(Ptr p_inst) {
  Ptr v2578a0629c0 = p_inst;
  Ptr v2578a0621a0 = (Ptr)v2578a0629c0;
  int64_t v2578a061fc0 = (int64_t)24ll;
  Ptr v2578a062290 = (Ptr)(v2578a0621a0 + v2578a061fc0);
  return v2578a062290;
  v2578a062420_break:
}

Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr, Ptr p_val) {
  Ptr v2578a065b50 = p_f;
  uint64_t v2578a0654c0 = (uint64_t)40llu;
  Fun_2578a056120 v2578a065b00 = malloc;
  Ptr v2578a065880 = v2578a065b00(v2578a065b50, v2578a0654c0);
  Ptr l_inst = v2578a065880;
  uint16_t v2578a0656f0 = p_type;
  Ptr v2578a0656a0 = l_inst;
  Ptr v2578a065380 = &((Struct_2578a050440*)v2578a0656a0)->type;
  *(uint16_t*)v2578a065380 = v2578a0656f0;
  uint16_t v2578a063ee0 = p_kind;
  Ptr v2578a065920 = l_inst;
  Ptr v2578a0658d0 = &((Struct_2578a050440*)v2578a065920)->kind;
  *(uint16_t*)v2578a0658d0 = v2578a063ee0;
  Ptr v2578a0645c0 = l_inst;
  Fun_2578a055b60 v2578a064cf0 = fe_to_store;
  Ptr v2578a064b10 = v2578a064cf0(v2578a0645c0);
  Ptr l_store = v2578a064b10;
  Ptr v2578a064110 = p_ptr;
  Ptr v2578a0647a0 = l_store;
  Ptr v2578a064570 = &((Struct_2578a055e20*)v2578a0647a0)->ptr;
  *(Ptr*)v2578a064570 = v2578a064110;
  Ptr v2578a064890 = p_val;
  Ptr v2578a0642a0 = l_store;
  Ptr v2578a064840 = &((Struct_2578a055e20*)v2578a0642a0)->val;
  *(Ptr*)v2578a064840 = v2578a064890;
  Ptr v2578a064ac0 = l_inst;
  return v2578a064ac0;
  v2578a065a10_break:
}

Ptr fe_to_store(Ptr p_inst) {
  Ptr v2578a064250 = p_inst;
  Ptr v2578a064750 = (Ptr)v2578a064250;
  int64_t v2578a063f30 = (int64_t)24ll;
  Ptr v2578a065150 = (Ptr)(v2578a064750 + v2578a063f30);
  return v2578a065150;
  v2578a065060_break:
}

Ptr fe_new_block(Ptr p_f) {
  Ptr v2578a064390 = p_f;
  uint64_t v2578a064980 = (uint64_t)32llu;
  Fun_2578a054da0 v2578a064340 = malloc;
  Ptr v2578a063f80 = v2578a064340(v2578a064390, v2578a064980);
  Ptr l_block = v2578a063f80;
  Ptr v2578a0642f0 = p_f;
  Ptr v2578a063fd0 = l_block;
  Fun_2578a055820 v2578a064e30 = fe_inst_bookend;
  Ptr v2578a064a70 = v2578a064e30(v2578a0642f0, v2578a063fd0);
  Ptr l_bookend = v2578a064a70;
  Ptr v2578a063e90 = l_bookend;
  Ptr v2578a064d90 = l_block;
  Ptr v2578a064d40 = &((Struct_2578a0555a0*)v2578a064d90)->start;
  *(Ptr*)v2578a064d40 = v2578a063e90;
  Ptr v2578a064430 = l_bookend;
  Ptr v2578a064020 = l_block;
  Ptr v2578a064f20 = &((Struct_2578a0555a0*)v2578a064020)->end;
  *(Ptr*)v2578a064f20 = v2578a064430;
  Ptr v2578a064700 = p_f;
  Ptr v2578a0646b0 = l_block;
  Ptr v2578a064660 = &((Struct_2578a0555a0*)v2578a0646b0)->function;
  *(Ptr*)v2578a064660 = v2578a064700;
  Ptr v2578a066660 = l_block;
  return v2578a066660;
  v2578a064f70_break:
}

