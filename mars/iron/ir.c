#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_562a896b4080 {
  uint16_t kind;
  uint16_t type;
  Ptr next;
  Ptr prev;
} Struct_562a896b4080;
typedef struct Struct_562a896b42c0 {
  Ptr block;
} Struct_562a896b42c0;
typedef struct Struct_562a896b4330 {
  Ptr start;
  Ptr end;
  Ptr next_block;
  Ptr function;
} Struct_562a896b4330;
typedef struct Struct_562a896b44d0 {
  Ptr symbol;
  Ptr first_block;
} Struct_562a896b44d0;
typedef struct Struct_562a896b4560 {
  Slice name;
} Struct_562a896b4560;
typedef struct Struct_562a896b4b80 {
  Ptr sub;
} Struct_562a896b4b80;
typedef struct Struct_562a896b4d20 {
  Ptr lhs;
  Ptr rhs;
} Struct_562a896b4d20;
typedef struct Struct_562a896b4ee0 {
  uint64_t val;
} Struct_562a896b4ee0;
typedef struct Struct_562a896b5030 {
  Ptr ptr;
} Struct_562a896b5030;
typedef struct Struct_562a896b51d0 {
  Ptr ptr;
  Ptr val;
} Struct_562a896b51d0;
typedef Ptr (*Fun_562a896b5760)(Ptr, uint64_t);
typedef Ptr (*Fun_562a896b5cc0)(Ptr, Ptr);
typedef Ptr (*Fun_562a896b7750)(Ptr, Ptr);
typedef Ptr (*Fun_562a896b89b0)(Ptr, Ptr);
typedef Ptr (*Fun_562a896b9c90)(Ptr, uint16_t, uint16_t);
typedef Ptr (*Fun_562a896ba650)(Ptr);
typedef Ptr (*Fun_562a896bacd0)(Ptr, Ptr);
typedef Ptr (*Fun_562a896bb6e0)(Ptr);
typedef Ptr (*Fun_562a896bc1c0)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_562a896bcd90)(Ptr);
typedef Ptr (*Fun_562a896bd900)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_562a896be4d0)(Ptr);
typedef Ptr (*Fun_562a896bf130)(Ptr, uint16_t, uint16_t, Ptr);
typedef Ptr (*Fun_562a896bfd00)(Ptr);
typedef Ptr (*Fun_562a896c0870)(Ptr, uint16_t, uint16_t, Ptr, Ptr);
typedef Ptr (*Fun_562a896c1440)(Ptr);
typedef Ptr (*Fun_562a896c1ec0)(Ptr);

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
  Ptr v562a896b5890 = (Ptr)0llu;
  return v562a896b5890;
  v562a896b57a0_break:
}

Ptr fe_append(Ptr p_b, Ptr p_inst) {
  Ptr v562a896b5e90 = p_b;
  Ptr v562a896b5e40 = ((Struct_562a896b4330*)v562a896b5e90)->start;
  uint16_t v562a896b5df0 = ((Struct_562a896b4080*)v562a896b5e40)->kind;
  uint16_t v562a896b5ee0 = (uint16_t)1llu;
  _Bool v562a896b5f30 = (_Bool)(v562a896b5df0 == v562a896b5ee0);
  if (v562a896b5f30)
  {
    Ptr v562a896b6240 = p_b;
    Ptr v562a896b61f0 = ((Struct_562a896b4330*)v562a896b6240)->start;
    Ptr l_bookend = v562a896b61f0;
    Ptr v562a896b6380 = l_bookend;
    Ptr v562a896b6330 = p_inst;
    Ptr v562a896b62e0 = &((Struct_562a896b4080*)v562a896b6330)->prev;
    *(Ptr*)v562a896b62e0 = v562a896b6380;
    Ptr v562a896b64c0 = l_bookend;
    Ptr v562a896b6470 = p_inst;
    Ptr v562a896b6420 = &((Struct_562a896b4080*)v562a896b6470)->next;
    *(Ptr*)v562a896b6420 = v562a896b64c0;
    Ptr v562a896b6600 = p_inst;
    Ptr v562a896b65b0 = l_bookend;
    Ptr v562a896b6560 = &((Struct_562a896b4080*)v562a896b65b0)->next;
    *(Ptr*)v562a896b6560 = v562a896b6600;
    Ptr v562a896b6770 = p_inst;
    Ptr v562a896b6720 = l_bookend;
    Ptr v562a896b66d0 = &((Struct_562a896b4080*)v562a896b6720)->prev;
    *(Ptr*)v562a896b66d0 = v562a896b6770;
    Ptr v562a896b68b0 = p_inst;
    Ptr v562a896b6860 = p_b;
    Ptr v562a896b6810 = &((Struct_562a896b4330*)v562a896b6860)->start;
    *(Ptr*)v562a896b6810 = v562a896b68b0;
    Ptr v562a896b6a30 = p_inst;
    Ptr v562a896b69e0 = p_b;
    Ptr v562a896b6990 = &((Struct_562a896b4330*)v562a896b69e0)->end;
    *(Ptr*)v562a896b6990 = v562a896b6a30;
    v562a896b60b0_break:
  }
  else {
    {
      Ptr v562a896b6d70 = p_b;
      Ptr v562a896b6d20 = ((Struct_562a896b4330*)v562a896b6d70)->end;
      Ptr v562a896b6cd0 = p_inst;
      Ptr v562a896b6c80 = &((Struct_562a896b4080*)v562a896b6cd0)->prev;
      *(Ptr*)v562a896b6c80 = v562a896b6d20;
      Ptr v562a896b6f50 = p_b;
      Ptr v562a896b6f00 = ((Struct_562a896b4330*)v562a896b6f50)->end;
      Ptr v562a896b6eb0 = ((Struct_562a896b4080*)v562a896b6f00)->next;
      Ptr v562a896b6e60 = p_inst;
      Ptr v562a896b6e10 = &((Struct_562a896b4080*)v562a896b6e60)->next;
      *(Ptr*)v562a896b6e10 = v562a896b6eb0;
      Ptr v562a896b7130 = p_inst;
      Ptr v562a896b70e0 = p_b;
      Ptr v562a896b7090 = ((Struct_562a896b4330*)v562a896b70e0)->end;
      Ptr v562a896b7040 = ((Struct_562a896b4080*)v562a896b7090)->next;
      Ptr v562a896b6ff0 = &((Struct_562a896b4080*)v562a896b7040)->prev;
      *(Ptr*)v562a896b6ff0 = v562a896b7130;
      Ptr v562a896b72c0 = p_inst;
      Ptr v562a896b7270 = p_b;
      Ptr v562a896b7220 = ((Struct_562a896b4330*)v562a896b7270)->end;
      Ptr v562a896b71d0 = &((Struct_562a896b4080*)v562a896b7220)->next;
      *(Ptr*)v562a896b71d0 = v562a896b72c0;
      v562a896b6b90_break:
    }
  }
  v562a896b5da0_break:
  Ptr v562a896b7390 = p_inst;
  return v562a896b7390;
  v562a896b5d00_break:
}

Ptr fe_insert_before(Ptr p_new, Ptr p_ref) {
  Ptr v562a896b7920 = p_ref;
  Ptr v562a896b78d0 = ((Struct_562a896b4080*)v562a896b7920)->prev;
  uint16_t v562a896b7880 = ((Struct_562a896b4080*)v562a896b78d0)->kind;
  uint16_t v562a896b7970 = (uint16_t)1llu;
  _Bool v562a896b79c0 = (_Bool)(v562a896b7880 == v562a896b7970);
  if (v562a896b79c0)
  {
    Ptr v562a896b7db0 = p_ref;
    Ptr v562a896b7d60 = ((Struct_562a896b4080*)v562a896b7db0)->prev;
    Ptr v562a896b7c80 = (Ptr)v562a896b7d60;
    Ptr l_bookend = v562a896b7c80;
    Ptr v562a896b7f40 = p_new;
    Ptr v562a896b7ef0 = l_bookend;
    Ptr v562a896b7ea0 = ((Struct_562a896b42c0*)v562a896b7ef0)->block;
    Ptr v562a896b7e50 = &((Struct_562a896b4330*)v562a896b7ea0)->start;
    *(Ptr*)v562a896b7e50 = v562a896b7f40;
    v562a896b7b40_break:
  }
  v562a896b7830_break:
  Ptr v562a896b8080 = p_ref;
  Ptr v562a896b8030 = p_new;
  Ptr v562a896b7fe0 = &((Struct_562a896b4080*)v562a896b8030)->next;
  *(Ptr*)v562a896b7fe0 = v562a896b8080;
  Ptr v562a896b8210 = p_ref;
  Ptr v562a896b81c0 = ((Struct_562a896b4080*)v562a896b8210)->prev;
  Ptr v562a896b8170 = p_new;
  Ptr v562a896b8120 = &((Struct_562a896b4080*)v562a896b8170)->prev;
  *(Ptr*)v562a896b8120 = v562a896b81c0;
  Ptr v562a896b83a0 = p_new;
  Ptr v562a896b8350 = p_ref;
  Ptr v562a896b8300 = ((Struct_562a896b4080*)v562a896b8350)->prev;
  Ptr v562a896b82b0 = &((Struct_562a896b4080*)v562a896b8300)->next;
  *(Ptr*)v562a896b82b0 = v562a896b83a0;
  Ptr v562a896b8510 = p_new;
  Ptr v562a896b84c0 = p_ref;
  Ptr v562a896b8470 = &((Struct_562a896b4080*)v562a896b84c0)->prev;
  *(Ptr*)v562a896b8470 = v562a896b8510;
  Ptr v562a896b85b0 = p_new;
  return v562a896b85b0;
  v562a896b7790_break:
}

Ptr fe_insert_after(Ptr p_new, Ptr p_ref) {
  Ptr v562a896b8b80 = p_ref;
  Ptr v562a896b8b30 = ((Struct_562a896b4080*)v562a896b8b80)->prev;
  uint16_t v562a896b8ae0 = ((Struct_562a896b4080*)v562a896b8b30)->kind;
  uint16_t v562a896b8bd0 = (uint16_t)1llu;
  _Bool v562a896b8c20 = (_Bool)(v562a896b8ae0 == v562a896b8bd0);
  if (v562a896b8c20)
  {
    Ptr v562a896b8fd0 = p_ref;
    Ptr v562a896b8f80 = ((Struct_562a896b4080*)v562a896b8fd0)->next;
    Ptr v562a896b8ee0 = (Ptr)v562a896b8f80;
    Ptr l_bookend = v562a896b8ee0;
    Ptr v562a896b9160 = p_new;
    Ptr v562a896b9110 = l_bookend;
    Ptr v562a896b90c0 = ((Struct_562a896b42c0*)v562a896b9110)->block;
    Ptr v562a896b9070 = &((Struct_562a896b4330*)v562a896b90c0)->end;
    *(Ptr*)v562a896b9070 = v562a896b9160;
    v562a896b8da0_break:
  }
  v562a896b8a90_break:
  Ptr v562a896b92a0 = p_ref;
  Ptr v562a896b9250 = p_new;
  Ptr v562a896b9200 = &((Struct_562a896b4080*)v562a896b9250)->prev;
  *(Ptr*)v562a896b9200 = v562a896b92a0;
  Ptr v562a896b9430 = p_ref;
  Ptr v562a896b93e0 = ((Struct_562a896b4080*)v562a896b9430)->next;
  Ptr v562a896b9390 = p_new;
  Ptr v562a896b9340 = &((Struct_562a896b4080*)v562a896b9390)->next;
  *(Ptr*)v562a896b9340 = v562a896b93e0;
  Ptr v562a896b95c0 = p_new;
  Ptr v562a896b9570 = p_ref;
  Ptr v562a896b9520 = ((Struct_562a896b4080*)v562a896b9570)->next;
  Ptr v562a896b94d0 = &((Struct_562a896b4080*)v562a896b9520)->prev;
  *(Ptr*)v562a896b94d0 = v562a896b95c0;
  Ptr v562a896b9700 = p_new;
  Ptr v562a896b96b0 = p_ref;
  Ptr v562a896b9660 = &((Struct_562a896b4080*)v562a896b96b0)->next;
  *(Ptr*)v562a896b9660 = v562a896b9700;
  Ptr v562a896b97a0 = p_new;
  return v562a896b97a0;
  v562a896b89f0_break:
}

Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint16_t p_type) {
  Ptr v562a896b9f20 = p_f;
  uint64_t v562a896b9f70 = (uint64_t)24llu;
  Fun_562a896b5760 v562a896b9ed0 = fe_malloc;
  Ptr v562a896b9e60 = v562a896b9ed0(v562a896b9f20, v562a896b9f70);
  Ptr l_inst = v562a896b9e60;
  uint16_t v562a896ba150 = p_type;
  Ptr v562a896ba100 = l_inst;
  Ptr v562a896ba0b0 = &((Struct_562a896b4080*)v562a896ba100)->type;
  *(uint16_t*)v562a896ba0b0 = v562a896ba150;
  uint16_t v562a896ba290 = p_kind;
  Ptr v562a896ba240 = l_inst;
  Ptr v562a896ba1f0 = &((Struct_562a896b4080*)v562a896ba240)->kind;
  *(uint16_t*)v562a896ba1f0 = v562a896ba290;
  Ptr v562a896ba330 = l_inst;
  return v562a896ba330;
  v562a896b9cd0_break:
}

Ptr fe_from_extra(Ptr p_extra) {
  Ptr v562a896ba780 = p_extra;
  Ptr v562a896ba7d0 = (Ptr)24llu;
  Ptr v562a896ba870 = (Ptr)(v562a896ba780 - v562a896ba7d0);
  Ptr v562a896ba8c0 = (Ptr)v562a896ba870;
  return v562a896ba8c0;
  v562a896ba690_break:
}

Ptr fe_inst_bookend(Ptr p_f, Ptr p_block) {
  Ptr v562a896baf40 = p_f;
  uint64_t v562a896bb0d0 = (uint64_t)32llu;
  Fun_562a896b5760 v562a896baef0 = fe_malloc;
  Ptr v562a896baea0 = v562a896baef0(v562a896baf40, v562a896bb0d0);
  Ptr l_inst = v562a896baea0;
  uint16_t v562a896bb260 = (uint16_t)1llu;
  Ptr v562a896bb210 = l_inst;
  Ptr v562a896bb1c0 = &((Struct_562a896b4080*)v562a896bb210)->kind;
  *(uint16_t*)v562a896bb1c0 = v562a896bb260;
  Ptr v562a896bba40 = l_inst;
  Fun_562a896bb6e0 v562a896bb9f0 = fe_to_bookend;
  Ptr v562a896bb350 = v562a896bb9f0(v562a896bba40);
  Ptr l_bookend = v562a896bb350;
  Ptr v562a896bbb80 = p_block;
  Ptr v562a896bbb30 = l_bookend;
  Ptr v562a896bbae0 = &((Struct_562a896b42c0*)v562a896bbb30)->block;
  *(Ptr*)v562a896bbae0 = v562a896bbb80;
  Ptr v562a896bbc20 = l_inst;
  return v562a896bbc20;
  v562a896bad10_break:
}

Ptr fe_to_bookend(Ptr p_inst) {
  Ptr v562a896bb8b0 = p_inst;
  Ptr v562a896bb810 = (Ptr)v562a896bb8b0;
  int64_t v562a896bb900 = (int64_t)24ll;
  Ptr v562a896bb9a0 = (Ptr)(v562a896bb810 + v562a896bb900);
  return v562a896bb9a0;
  v562a896bb720_break:
}

Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_sub) {
  Ptr v562a896bc450 = p_f;
  uint64_t v562a896bc5e0 = (uint64_t)32llu;
  Fun_562a896b5760 v562a896bc400 = fe_malloc;
  Ptr v562a896bc390 = v562a896bc400(v562a896bc450, v562a896bc5e0);
  Ptr l_inst = v562a896bc390;
  uint16_t v562a896bc770 = p_type;
  Ptr v562a896bc720 = l_inst;
  Ptr v562a896bc6d0 = &((Struct_562a896b4080*)v562a896bc720)->type;
  *(uint16_t*)v562a896bc6d0 = v562a896bc770;
  uint16_t v562a896bc8b0 = p_kind;
  Ptr v562a896bc860 = l_inst;
  Ptr v562a896bc810 = &((Struct_562a896b4080*)v562a896bc860)->kind;
  *(uint16_t*)v562a896bc810 = v562a896bc8b0;
  Ptr v562a896bd0f0 = l_inst;
  Fun_562a896bcd90 v562a896bd0a0 = fe_to_unop;
  Ptr v562a896bc9a0 = v562a896bd0a0(v562a896bd0f0);
  Ptr l_unop = v562a896bc9a0;
  Ptr v562a896bd230 = p_sub;
  Ptr v562a896bd1e0 = l_unop;
  Ptr v562a896bd190 = &((Struct_562a896b4b80*)v562a896bd1e0)->sub;
  *(Ptr*)v562a896bd190 = v562a896bd230;
  Ptr v562a896bd2d0 = l_inst;
  return v562a896bd2d0;
  v562a896bc200_break:
}

Ptr fe_to_unop(Ptr p_inst) {
  Ptr v562a896bcf60 = p_inst;
  Ptr v562a896bcec0 = (Ptr)v562a896bcf60;
  int64_t v562a896bcfb0 = (int64_t)24ll;
  Ptr v562a896bd050 = (Ptr)(v562a896bcec0 + v562a896bcfb0);
  return v562a896bd050;
  v562a896bcdd0_break:
}

Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_lhs, Ptr p_rhs) {
  Ptr v562a896bdb90 = p_f;
  uint64_t v562a896bdd20 = (uint64_t)40llu;
  Fun_562a896b5760 v562a896bdb40 = fe_malloc;
  Ptr v562a896bdad0 = v562a896bdb40(v562a896bdb90, v562a896bdd20);
  Ptr l_inst = v562a896bdad0;
  uint16_t v562a896bdeb0 = p_type;
  Ptr v562a896bde60 = l_inst;
  Ptr v562a896bde10 = &((Struct_562a896b4080*)v562a896bde60)->type;
  *(uint16_t*)v562a896bde10 = v562a896bdeb0;
  uint16_t v562a896bdff0 = p_kind;
  Ptr v562a896bdfa0 = l_inst;
  Ptr v562a896bdf50 = &((Struct_562a896b4080*)v562a896bdfa0)->kind;
  *(uint16_t*)v562a896bdf50 = v562a896bdff0;
  Ptr v562a896be830 = l_inst;
  Fun_562a896be4d0 v562a896be7e0 = fe_to_binop;
  Ptr v562a896be0e0 = v562a896be7e0(v562a896be830);
  Ptr l_binop = v562a896be0e0;
  Ptr v562a896be970 = p_lhs;
  Ptr v562a896be920 = l_binop;
  Ptr v562a896be8d0 = &((Struct_562a896b4d20*)v562a896be920)->lhs;
  *(Ptr*)v562a896be8d0 = v562a896be970;
  Ptr v562a896beab0 = p_rhs;
  Ptr v562a896bea60 = l_binop;
  Ptr v562a896bea10 = &((Struct_562a896b4d20*)v562a896bea60)->rhs;
  *(Ptr*)v562a896bea10 = v562a896beab0;
  Ptr v562a896beb90 = l_inst;
  return v562a896beb90;
  v562a896bd940_break:
}

Ptr fe_to_binop(Ptr p_inst) {
  Ptr v562a896be6a0 = p_inst;
  Ptr v562a896be600 = (Ptr)v562a896be6a0;
  int64_t v562a896be6f0 = (int64_t)24ll;
  Ptr v562a896be790 = (Ptr)(v562a896be600 + v562a896be6f0);
  return v562a896be790;
  v562a896be510_break:
}

Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr) {
  Ptr v562a896bf3c0 = p_f;
  uint64_t v562a896bf550 = (uint64_t)32llu;
  Fun_562a896b5760 v562a896bf370 = fe_malloc;
  Ptr v562a896bf300 = v562a896bf370(v562a896bf3c0, v562a896bf550);
  Ptr l_inst = v562a896bf300;
  uint16_t v562a896bf6e0 = p_type;
  Ptr v562a896bf690 = l_inst;
  Ptr v562a896bf640 = &((Struct_562a896b4080*)v562a896bf690)->type;
  *(uint16_t*)v562a896bf640 = v562a896bf6e0;
  uint16_t v562a896bf820 = p_kind;
  Ptr v562a896bf7d0 = l_inst;
  Ptr v562a896bf780 = &((Struct_562a896b4080*)v562a896bf7d0)->kind;
  *(uint16_t*)v562a896bf780 = v562a896bf820;
  Ptr v562a896c0060 = l_inst;
  Fun_562a896bfd00 v562a896c0010 = fe_to_load;
  Ptr v562a896bf910 = v562a896c0010(v562a896c0060);
  Ptr l_load = v562a896bf910;
  Ptr v562a896c01a0 = p_ptr;
  Ptr v562a896c0150 = l_load;
  Ptr v562a896c0100 = &((Struct_562a896b5030*)v562a896c0150)->ptr;
  *(Ptr*)v562a896c0100 = v562a896c01a0;
  Ptr v562a896c0240 = l_inst;
  return v562a896c0240;
  v562a896bf170_break:
}

Ptr fe_to_load(Ptr p_inst) {
  Ptr v562a896bfed0 = p_inst;
  Ptr v562a896bfe30 = (Ptr)v562a896bfed0;
  int64_t v562a896bff20 = (int64_t)24ll;
  Ptr v562a896bffc0 = (Ptr)(v562a896bfe30 + v562a896bff20);
  return v562a896bffc0;
  v562a896bfd40_break:
}

Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint16_t p_type, Ptr p_ptr, Ptr p_val) {
  Ptr v562a896c0b00 = p_f;
  uint64_t v562a896c0c90 = (uint64_t)40llu;
  Fun_562a896b5760 v562a896c0ab0 = fe_malloc;
  Ptr v562a896c0a40 = v562a896c0ab0(v562a896c0b00, v562a896c0c90);
  Ptr l_inst = v562a896c0a40;
  uint16_t v562a896c0e20 = p_type;
  Ptr v562a896c0dd0 = l_inst;
  Ptr v562a896c0d80 = &((Struct_562a896b4080*)v562a896c0dd0)->type;
  *(uint16_t*)v562a896c0d80 = v562a896c0e20;
  uint16_t v562a896c0f60 = p_kind;
  Ptr v562a896c0f10 = l_inst;
  Ptr v562a896c0ec0 = &((Struct_562a896b4080*)v562a896c0f10)->kind;
  *(uint16_t*)v562a896c0ec0 = v562a896c0f60;
  Ptr v562a896c17a0 = l_inst;
  Fun_562a896c1440 v562a896c1750 = fe_to_store;
  Ptr v562a896c1050 = v562a896c1750(v562a896c17a0);
  Ptr l_store = v562a896c1050;
  Ptr v562a896c18e0 = p_ptr;
  Ptr v562a896c1890 = l_store;
  Ptr v562a896c1840 = &((Struct_562a896b51d0*)v562a896c1890)->ptr;
  *(Ptr*)v562a896c1840 = v562a896c18e0;
  Ptr v562a896c1a20 = p_val;
  Ptr v562a896c19d0 = l_store;
  Ptr v562a896c1980 = &((Struct_562a896b51d0*)v562a896c19d0)->val;
  *(Ptr*)v562a896c1980 = v562a896c1a20;
  Ptr v562a896c1b00 = l_inst;
  return v562a896c1b00;
  v562a896c08b0_break:
}

Ptr fe_to_store(Ptr p_inst) {
  Ptr v562a896c1610 = p_inst;
  Ptr v562a896c1570 = (Ptr)v562a896c1610;
  int64_t v562a896c1660 = (int64_t)24ll;
  Ptr v562a896c1700 = (Ptr)(v562a896c1570 + v562a896c1660);
  return v562a896c1700;
  v562a896c1480_break:
}

Ptr fe_new_block(Ptr p_f) {
  Ptr v562a896c2150 = p_f;
  uint64_t v562a896c21a0 = (uint64_t)32llu;
  Fun_562a896b5760 v562a896c2100 = fe_malloc;
  Ptr v562a896c2090 = v562a896c2100(v562a896c2150, v562a896c21a0);
  Ptr l_block = v562a896c2090;
  Ptr v562a896c23f0 = p_f;
  Ptr v562a896c2440 = l_block;
  Fun_562a896bacd0 v562a896c23a0 = fe_inst_bookend;
  Ptr v562a896c2330 = v562a896c23a0(v562a896c23f0, v562a896c2440);
  Ptr l_bookend = v562a896c2330;
  Ptr v562a896c2580 = l_bookend;
  Ptr v562a896c2530 = l_block;
  Ptr v562a896c24e0 = &((Struct_562a896b4330*)v562a896c2530)->start;
  *(Ptr*)v562a896c24e0 = v562a896c2580;
  Ptr v562a896c26c0 = l_bookend;
  Ptr v562a896c2670 = l_block;
  Ptr v562a896c2620 = &((Struct_562a896b4330*)v562a896c2670)->end;
  *(Ptr*)v562a896c2620 = v562a896c26c0;
  Ptr v562a896c2800 = p_f;
  Ptr v562a896c27b0 = l_block;
  Ptr v562a896c2760 = &((Struct_562a896b4330*)v562a896c27b0)->function;
  *(Ptr*)v562a896c2760 = v562a896c2800;
  Ptr v562a896c28a0 = l_block;
  return v562a896c28a0;
  v562a896c1f00_break:
}

