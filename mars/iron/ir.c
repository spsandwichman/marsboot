#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_563c16ebc720 {
  uint16_t kind;
  uint8_t type;
  Ptr next;
  Ptr prev;
} Struct_563c16ebc720;
typedef struct Struct_563c16ebc960 {
  Ptr block;
} Struct_563c16ebc960;
typedef struct Struct_563c16ebc9a0 {
  Ptr start;
  Ptr end;
  Ptr next_block;
  Ptr function;
} Struct_563c16ebc9a0;
typedef struct Struct_563c16ebcb40 {
  Ptr symbol;
  Ptr first_block;
} Struct_563c16ebcb40;
typedef struct Struct_563c16ebcbd0 {
  Slice name;
} Struct_563c16ebcbd0;
typedef struct Struct_563c16ebd220 {
  Ptr sub;
} Struct_563c16ebd220;
typedef struct Struct_563c16ebd3c0 {
  Ptr lhs;
  Ptr rhs;
} Struct_563c16ebd3c0;
typedef struct Struct_563c16ebd5c0 {
  uint64_t val;
} Struct_563c16ebd5c0;
typedef struct Struct_563c16ebd6e0 {
  Ptr ptr;
} Struct_563c16ebd6e0;
typedef struct Struct_563c16ebd580 {
  Ptr ptr;
  Ptr val;
} Struct_563c16ebd580;
typedef Ptr (*Fun_563c16ebddb0)(Ptr, uint64_t);
typedef Ptr (*Fun_563c16ebe1d0)(Ptr, Ptr);
typedef Ptr (*Fun_563c16ebfc70)(Ptr, Ptr);
typedef Ptr (*Fun_563c16ec0590)(Ptr);
typedef Ptr (*Fun_563c16ec1540)(Ptr, Ptr);
typedef Ptr (*Fun_563c16ec28b0)(Ptr, uint16_t, uint8_t);
typedef Ptr (*Fun_563c16ec2da0)(Ptr, uint64_t);
typedef Ptr (*Fun_563c16ec3560)(Ptr);
typedef Ptr (*Fun_563c16ec3c80)(Ptr, Ptr);
typedef Ptr (*Fun_563c16ec41b0)(Ptr, uint64_t);
typedef Ptr (*Fun_563c16ec4f20)(Ptr, uint16_t, uint8_t, Ptr);
typedef Ptr (*Fun_563c16ec5450)(Ptr, uint64_t);
typedef Ptr (*Fun_563c16ec5e20)(Ptr);
typedef Ptr (*Fun_563c16ec6a30)(Ptr, uint16_t, uint8_t, Ptr, Ptr);
typedef Ptr (*Fun_563c16ec6f60)(Ptr, uint64_t);
typedef Ptr (*Fun_563c16ec7930)(Ptr);
typedef Ptr (*Fun_563c16ec8640)(Ptr, uint16_t, uint8_t, Ptr);
typedef Ptr (*Fun_563c16ec8b70)(Ptr, uint64_t);
typedef Ptr (*Fun_563c16ec9540)(Ptr);
typedef Ptr (*Fun_563c16eca170)(Ptr, uint16_t, uint8_t, Ptr, Ptr);
typedef Ptr (*Fun_563c16eca6a0)(Ptr, uint64_t);
typedef Ptr (*Fun_563c16ecb070)(Ptr);
typedef Ptr (*Fun_563c16ecbbc0)(Ptr);
typedef Ptr (*Fun_563c16ecc0f0)(Ptr, uint64_t);

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
  Ptr v563c16ebe3a0 = p_b;
  Ptr v563c16ebe350 = ((Struct_563c16ebc9a0*)v563c16ebe3a0)->start;
  uint16_t v563c16ebe300 = ((Struct_563c16ebc720*)v563c16ebe350)->kind;
  uint16_t v563c16ebe3f0 = (uint16_t)1llu;
  _Bool v563c16ebe440 = (_Bool)(v563c16ebe300 == v563c16ebe3f0);
  if (v563c16ebe440)
  {
    Ptr v563c16ebe750 = p_b;
    Ptr v563c16ebe700 = ((Struct_563c16ebc9a0*)v563c16ebe750)->start;
    Ptr l_bookend = v563c16ebe700;
    Ptr v563c16ebe890 = l_bookend;
    Ptr v563c16ebe840 = p_inst;
    Ptr v563c16ebe7f0 = &((Struct_563c16ebc720*)v563c16ebe840)->prev;
    *(Ptr*)v563c16ebe7f0 = v563c16ebe890;
    Ptr v563c16ebe9d0 = l_bookend;
    Ptr v563c16ebe980 = p_inst;
    Ptr v563c16ebe930 = &((Struct_563c16ebc720*)v563c16ebe980)->next;
    *(Ptr*)v563c16ebe930 = v563c16ebe9d0;
    Ptr v563c16ebeb10 = p_inst;
    Ptr v563c16ebeac0 = l_bookend;
    Ptr v563c16ebea70 = &((Struct_563c16ebc720*)v563c16ebeac0)->next;
    *(Ptr*)v563c16ebea70 = v563c16ebeb10;
    Ptr v563c16ebec80 = p_inst;
    Ptr v563c16ebec30 = l_bookend;
    Ptr v563c16ebebe0 = &((Struct_563c16ebc720*)v563c16ebec30)->prev;
    *(Ptr*)v563c16ebebe0 = v563c16ebec80;
    Ptr v563c16ebedc0 = p_inst;
    Ptr v563c16ebed70 = p_b;
    Ptr v563c16ebed20 = &((Struct_563c16ebc9a0*)v563c16ebed70)->start;
    *(Ptr*)v563c16ebed20 = v563c16ebedc0;
    Ptr v563c16ebef40 = p_inst;
    Ptr v563c16ebeef0 = p_b;
    Ptr v563c16ebeea0 = &((Struct_563c16ebc9a0*)v563c16ebeef0)->end;
    *(Ptr*)v563c16ebeea0 = v563c16ebef40;
    v563c16ebe5c0_break:
  }
  else {
    {
      Ptr v563c16ebf280 = p_b;
      Ptr v563c16ebf230 = ((Struct_563c16ebc9a0*)v563c16ebf280)->end;
      Ptr v563c16ebf1e0 = p_inst;
      Ptr v563c16ebf190 = &((Struct_563c16ebc720*)v563c16ebf1e0)->prev;
      *(Ptr*)v563c16ebf190 = v563c16ebf230;
      Ptr v563c16ebf460 = p_b;
      Ptr v563c16ebf410 = ((Struct_563c16ebc9a0*)v563c16ebf460)->end;
      Ptr v563c16ebf3c0 = ((Struct_563c16ebc720*)v563c16ebf410)->next;
      Ptr v563c16ebf370 = p_inst;
      Ptr v563c16ebf320 = &((Struct_563c16ebc720*)v563c16ebf370)->next;
      *(Ptr*)v563c16ebf320 = v563c16ebf3c0;
      Ptr v563c16ebf640 = p_inst;
      Ptr v563c16ebf5f0 = p_b;
      Ptr v563c16ebf5a0 = ((Struct_563c16ebc9a0*)v563c16ebf5f0)->end;
      Ptr v563c16ebf550 = ((Struct_563c16ebc720*)v563c16ebf5a0)->next;
      Ptr v563c16ebf500 = &((Struct_563c16ebc720*)v563c16ebf550)->prev;
      *(Ptr*)v563c16ebf500 = v563c16ebf640;
      Ptr v563c16ebf7d0 = p_inst;
      Ptr v563c16ebf780 = p_b;
      Ptr v563c16ebf730 = ((Struct_563c16ebc9a0*)v563c16ebf780)->end;
      Ptr v563c16ebf6e0 = &((Struct_563c16ebc720*)v563c16ebf730)->next;
      *(Ptr*)v563c16ebf6e0 = v563c16ebf7d0;
      v563c16ebf0a0_break:
    }
  }
  v563c16ebe2b0_break:
  Ptr v563c16ebf8a0 = p_inst;
  return v563c16ebf8a0;
  v563c16ebe210_break:
}

Ptr fe_insert_before(Ptr p_new, Ptr p_ref) {
  Ptr v563c16ebfe40 = p_ref;
  Ptr v563c16ebfdf0 = ((Struct_563c16ebc720*)v563c16ebfe40)->prev;
  uint16_t v563c16ebfda0 = ((Struct_563c16ebc720*)v563c16ebfdf0)->kind;
  uint16_t v563c16ebfe90 = (uint16_t)1llu;
  _Bool v563c16ebfee0 = (_Bool)(v563c16ebfda0 == v563c16ebfe90);
  if (v563c16ebfee0)
  {
    Ptr v563c16ec0940 = p_ref;
    Ptr v563c16ec08f0 = ((Struct_563c16ebc720*)v563c16ec0940)->prev;
    Fun_563c16ec0590 v563c16ec08a0 = fe_to_bookend;
    Ptr v563c16ec01a0 = v563c16ec08a0(v563c16ec08f0);
    Ptr l_bookend = v563c16ec01a0;
    Ptr v563c16ec0ad0 = p_new;
    Ptr v563c16ec0a80 = l_bookend;
    Ptr v563c16ec0a30 = ((Struct_563c16ebc960*)v563c16ec0a80)->block;
    Ptr v563c16ec09e0 = &((Struct_563c16ebc9a0*)v563c16ec0a30)->start;
    *(Ptr*)v563c16ec09e0 = v563c16ec0ad0;
    v563c16ec0060_break:
  }
  v563c16ebfd50_break:
  Ptr v563c16ec0c10 = p_ref;
  Ptr v563c16ec0bc0 = p_new;
  Ptr v563c16ec0b70 = &((Struct_563c16ebc720*)v563c16ec0bc0)->next;
  *(Ptr*)v563c16ec0b70 = v563c16ec0c10;
  Ptr v563c16ec0da0 = p_ref;
  Ptr v563c16ec0d50 = ((Struct_563c16ebc720*)v563c16ec0da0)->prev;
  Ptr v563c16ec0d00 = p_new;
  Ptr v563c16ec0cb0 = &((Struct_563c16ebc720*)v563c16ec0d00)->prev;
  *(Ptr*)v563c16ec0cb0 = v563c16ec0d50;
  Ptr v563c16ec0f30 = p_new;
  Ptr v563c16ec0ee0 = p_ref;
  Ptr v563c16ec0e90 = ((Struct_563c16ebc720*)v563c16ec0ee0)->prev;
  Ptr v563c16ec0e40 = &((Struct_563c16ebc720*)v563c16ec0e90)->next;
  *(Ptr*)v563c16ec0e40 = v563c16ec0f30;
  Ptr v563c16ec10a0 = p_new;
  Ptr v563c16ec1050 = p_ref;
  Ptr v563c16ec1000 = &((Struct_563c16ebc720*)v563c16ec1050)->prev;
  *(Ptr*)v563c16ec1000 = v563c16ec10a0;
  Ptr v563c16ec1140 = p_new;
  return v563c16ec1140;
  v563c16ebfcb0_break:
}

Ptr fe_insert_after(Ptr p_new, Ptr p_ref) {
  Ptr v563c16ec1710 = p_ref;
  Ptr v563c16ec16c0 = ((Struct_563c16ebc720*)v563c16ec1710)->next;
  uint16_t v563c16ec1670 = ((Struct_563c16ebc720*)v563c16ec16c0)->kind;
  uint16_t v563c16ec1760 = (uint16_t)1llu;
  _Bool v563c16ec17b0 = (_Bool)(v563c16ec1670 == v563c16ec1760);
  if (v563c16ec17b0)
  {
    Ptr v563c16ec1b80 = p_ref;
    Ptr v563c16ec1b30 = ((Struct_563c16ebc720*)v563c16ec1b80)->next;
    Fun_563c16ec0590 v563c16ec1ae0 = fe_to_bookend;
    Ptr v563c16ec1a70 = v563c16ec1ae0(v563c16ec1b30);
    Ptr l_bookend = v563c16ec1a70;
    Ptr v563c16ec1d10 = p_new;
    Ptr v563c16ec1cc0 = l_bookend;
    Ptr v563c16ec1c70 = ((Struct_563c16ebc960*)v563c16ec1cc0)->block;
    Ptr v563c16ec1c20 = &((Struct_563c16ebc9a0*)v563c16ec1c70)->end;
    *(Ptr*)v563c16ec1c20 = v563c16ec1d10;
    v563c16ec1930_break:
  }
  v563c16ec1620_break:
  Ptr v563c16ec1e50 = p_ref;
  Ptr v563c16ec1e00 = p_new;
  Ptr v563c16ec1db0 = &((Struct_563c16ebc720*)v563c16ec1e00)->prev;
  *(Ptr*)v563c16ec1db0 = v563c16ec1e50;
  Ptr v563c16ec1fe0 = p_ref;
  Ptr v563c16ec1f90 = ((Struct_563c16ebc720*)v563c16ec1fe0)->next;
  Ptr v563c16ec1f40 = p_new;
  Ptr v563c16ec1ef0 = &((Struct_563c16ebc720*)v563c16ec1f40)->next;
  *(Ptr*)v563c16ec1ef0 = v563c16ec1f90;
  Ptr v563c16ec2170 = p_new;
  Ptr v563c16ec2120 = p_ref;
  Ptr v563c16ec20d0 = ((Struct_563c16ebc720*)v563c16ec2120)->next;
  Ptr v563c16ec2080 = &((Struct_563c16ebc720*)v563c16ec20d0)->prev;
  *(Ptr*)v563c16ec2080 = v563c16ec2170;
  Ptr v563c16ec22b0 = p_new;
  Ptr v563c16ec2260 = p_ref;
  Ptr v563c16ec2210 = &((Struct_563c16ebc720*)v563c16ec2260)->next;
  *(Ptr*)v563c16ec2210 = v563c16ec22b0;
  Ptr v563c16ec2350 = p_new;
  return v563c16ec2350;
  v563c16ec1580_break:
}

Ptr fe_inst_base(Ptr p_f, uint16_t p_kind, uint8_t p_type) {
  Ptr v563c16ec2e30 = p_f;
  uint64_t v563c16ec2e80 = (uint64_t)24llu;
  Fun_563c16ec2da0 v563c16ec2de0 = malloc;
  Ptr v563c16ec2a80 = v563c16ec2de0(v563c16ec2e30, v563c16ec2e80);
  Ptr l_inst = v563c16ec2a80;
  uint8_t v563c16ec3060 = p_type;
  Ptr v563c16ec3010 = l_inst;
  Ptr v563c16ec2fc0 = &((Struct_563c16ebc720*)v563c16ec3010)->type;
  *(uint8_t*)v563c16ec2fc0 = v563c16ec3060;
  uint16_t v563c16ec31a0 = p_kind;
  Ptr v563c16ec3150 = l_inst;
  Ptr v563c16ec3100 = &((Struct_563c16ebc720*)v563c16ec3150)->kind;
  *(uint16_t*)v563c16ec3100 = v563c16ec31a0;
  Ptr v563c16ec3240 = l_inst;
  return v563c16ec3240;
  v563c16ec28f0_break:
}

Ptr fe_from_extra(Ptr p_extra) {
  Ptr v563c16ec3690 = p_extra;
  Ptr v563c16ec36e0 = (Ptr)24llu;
  Ptr v563c16ec3780 = (Ptr)(v563c16ec3690 - v563c16ec36e0);
  Ptr v563c16ec37d0 = (Ptr)v563c16ec3780;
  return v563c16ec37d0;
  v563c16ec35a0_break:
}

Ptr fe_inst_bookend(Ptr p_f, Ptr p_block) {
  Ptr v563c16ec4240 = p_f;
  uint64_t v563c16ec43d0 = (uint64_t)32llu;
  Fun_563c16ec41b0 v563c16ec41f0 = malloc;
  Ptr v563c16ec3e50 = v563c16ec41f0(v563c16ec4240, v563c16ec43d0);
  Ptr l_inst = v563c16ec3e50;
  uint16_t v563c16ec4560 = (uint16_t)1llu;
  Ptr v563c16ec4510 = l_inst;
  Ptr v563c16ec44c0 = &((Struct_563c16ebc720*)v563c16ec4510)->kind;
  *(uint16_t*)v563c16ec44c0 = v563c16ec4560;
  Ptr v563c16ec4710 = l_inst;
  Fun_563c16ec0590 v563c16ec46c0 = fe_to_bookend;
  Ptr v563c16ec4650 = v563c16ec46c0(v563c16ec4710);
  Ptr l_bookend = v563c16ec4650;
  Ptr v563c16ec4850 = p_block;
  Ptr v563c16ec4800 = l_bookend;
  Ptr v563c16ec47b0 = &((Struct_563c16ebc960*)v563c16ec4800)->block;
  *(Ptr*)v563c16ec47b0 = v563c16ec4850;
  Ptr v563c16ec48f0 = l_inst;
  return v563c16ec48f0;
  v563c16ec3cc0_break:
}

Ptr fe_to_bookend(Ptr p_inst) {
  Ptr v563c16ec0760 = p_inst;
  Ptr v563c16ec06c0 = (Ptr)v563c16ec0760;
  int64_t v563c16ec07b0 = (int64_t)24ll;
  Ptr v563c16ec0850 = (Ptr)(v563c16ec06c0 + v563c16ec07b0);
  return v563c16ec0850;
  v563c16ec05d0_break:
}

Ptr fe_inst_unop(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_sub) {
  Ptr v563c16ec54e0 = p_f;
  uint64_t v563c16ec5670 = (uint64_t)32llu;
  Fun_563c16ec5450 v563c16ec5490 = malloc;
  Ptr v563c16ec50f0 = v563c16ec5490(v563c16ec54e0, v563c16ec5670);
  Ptr l_inst = v563c16ec50f0;
  uint8_t v563c16ec5800 = p_type;
  Ptr v563c16ec57b0 = l_inst;
  Ptr v563c16ec5760 = &((Struct_563c16ebc720*)v563c16ec57b0)->type;
  *(uint8_t*)v563c16ec5760 = v563c16ec5800;
  uint16_t v563c16ec5940 = p_kind;
  Ptr v563c16ec58f0 = l_inst;
  Ptr v563c16ec58a0 = &((Struct_563c16ebc720*)v563c16ec58f0)->kind;
  *(uint16_t*)v563c16ec58a0 = v563c16ec5940;
  Ptr v563c16ec6180 = l_inst;
  Fun_563c16ec5e20 v563c16ec6130 = fe_to_unop;
  Ptr v563c16ec5a30 = v563c16ec6130(v563c16ec6180);
  Ptr l_unop = v563c16ec5a30;
  Ptr v563c16ec62c0 = p_sub;
  Ptr v563c16ec6270 = l_unop;
  Ptr v563c16ec6220 = &((Struct_563c16ebd220*)v563c16ec6270)->sub;
  *(Ptr*)v563c16ec6220 = v563c16ec62c0;
  Ptr v563c16ec6360 = l_inst;
  return v563c16ec6360;
  v563c16ec4f60_break:
}

Ptr fe_to_unop(Ptr p_inst) {
  Ptr v563c16ec5ff0 = p_inst;
  Ptr v563c16ec5f50 = (Ptr)v563c16ec5ff0;
  int64_t v563c16ec6040 = (int64_t)24ll;
  Ptr v563c16ec60e0 = (Ptr)(v563c16ec5f50 + v563c16ec6040);
  return v563c16ec60e0;
  v563c16ec5e60_break:
}

Ptr fe_inst_binop(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_lhs, Ptr p_rhs) {
  Ptr v563c16ec6ff0 = p_f;
  uint64_t v563c16ec7180 = (uint64_t)40llu;
  Fun_563c16ec6f60 v563c16ec6fa0 = malloc;
  Ptr v563c16ec6c00 = v563c16ec6fa0(v563c16ec6ff0, v563c16ec7180);
  Ptr l_inst = v563c16ec6c00;
  uint8_t v563c16ec7310 = p_type;
  Ptr v563c16ec72c0 = l_inst;
  Ptr v563c16ec7270 = &((Struct_563c16ebc720*)v563c16ec72c0)->type;
  *(uint8_t*)v563c16ec7270 = v563c16ec7310;
  uint16_t v563c16ec7450 = p_kind;
  Ptr v563c16ec7400 = l_inst;
  Ptr v563c16ec73b0 = &((Struct_563c16ebc720*)v563c16ec7400)->kind;
  *(uint16_t*)v563c16ec73b0 = v563c16ec7450;
  Ptr v563c16ec7c90 = l_inst;
  Fun_563c16ec7930 v563c16ec7c40 = fe_to_binop;
  Ptr v563c16ec7540 = v563c16ec7c40(v563c16ec7c90);
  Ptr l_binop = v563c16ec7540;
  Ptr v563c16ec7dd0 = p_lhs;
  Ptr v563c16ec7d80 = l_binop;
  Ptr v563c16ec7d30 = &((Struct_563c16ebd3c0*)v563c16ec7d80)->lhs;
  *(Ptr*)v563c16ec7d30 = v563c16ec7dd0;
  Ptr v563c16ec7f10 = p_rhs;
  Ptr v563c16ec7ec0 = l_binop;
  Ptr v563c16ec7e70 = &((Struct_563c16ebd3c0*)v563c16ec7ec0)->rhs;
  *(Ptr*)v563c16ec7e70 = v563c16ec7f10;
  Ptr v563c16ec7ff0 = l_inst;
  return v563c16ec7ff0;
  v563c16ec6a70_break:
}

Ptr fe_to_binop(Ptr p_inst) {
  Ptr v563c16ec7b00 = p_inst;
  Ptr v563c16ec7a60 = (Ptr)v563c16ec7b00;
  int64_t v563c16ec7b50 = (int64_t)24ll;
  Ptr v563c16ec7bf0 = (Ptr)(v563c16ec7a60 + v563c16ec7b50);
  return v563c16ec7bf0;
  v563c16ec7970_break:
}

Ptr fe_inst_load(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_ptr) {
  Ptr v563c16ec8c00 = p_f;
  uint64_t v563c16ec8d90 = (uint64_t)32llu;
  Fun_563c16ec8b70 v563c16ec8bb0 = malloc;
  Ptr v563c16ec8810 = v563c16ec8bb0(v563c16ec8c00, v563c16ec8d90);
  Ptr l_inst = v563c16ec8810;
  uint8_t v563c16ec8f20 = p_type;
  Ptr v563c16ec8ed0 = l_inst;
  Ptr v563c16ec8e80 = &((Struct_563c16ebc720*)v563c16ec8ed0)->type;
  *(uint8_t*)v563c16ec8e80 = v563c16ec8f20;
  uint16_t v563c16ec9060 = p_kind;
  Ptr v563c16ec9010 = l_inst;
  Ptr v563c16ec8fc0 = &((Struct_563c16ebc720*)v563c16ec9010)->kind;
  *(uint16_t*)v563c16ec8fc0 = v563c16ec9060;
  Ptr v563c16ec98a0 = l_inst;
  Fun_563c16ec9540 v563c16ec9850 = fe_to_load;
  Ptr v563c16ec9150 = v563c16ec9850(v563c16ec98a0);
  Ptr l_load = v563c16ec9150;
  Ptr v563c16ec99e0 = p_ptr;
  Ptr v563c16ec9990 = l_load;
  Ptr v563c16ec9940 = &((Struct_563c16ebd6e0*)v563c16ec9990)->ptr;
  *(Ptr*)v563c16ec9940 = v563c16ec99e0;
  Ptr v563c16ec9a80 = l_inst;
  return v563c16ec9a80;
  v563c16ec8680_break:
}

Ptr fe_to_load(Ptr p_inst) {
  Ptr v563c16ec9710 = p_inst;
  Ptr v563c16ec9670 = (Ptr)v563c16ec9710;
  int64_t v563c16ec9760 = (int64_t)24ll;
  Ptr v563c16ec9800 = (Ptr)(v563c16ec9670 + v563c16ec9760);
  return v563c16ec9800;
  v563c16ec9580_break:
}

Ptr fe_inst_store(Ptr p_f, uint16_t p_kind, uint8_t p_type, Ptr p_ptr, Ptr p_val) {
  Ptr v563c16eca730 = p_f;
  uint64_t v563c16eca8c0 = (uint64_t)40llu;
  Fun_563c16eca6a0 v563c16eca6e0 = malloc;
  Ptr v563c16eca340 = v563c16eca6e0(v563c16eca730, v563c16eca8c0);
  Ptr l_inst = v563c16eca340;
  uint8_t v563c16ecaa50 = p_type;
  Ptr v563c16ecaa00 = l_inst;
  Ptr v563c16eca9b0 = &((Struct_563c16ebc720*)v563c16ecaa00)->type;
  *(uint8_t*)v563c16eca9b0 = v563c16ecaa50;
  uint16_t v563c16ecab90 = p_kind;
  Ptr v563c16ecab40 = l_inst;
  Ptr v563c16ecaaf0 = &((Struct_563c16ebc720*)v563c16ecab40)->kind;
  *(uint16_t*)v563c16ecaaf0 = v563c16ecab90;
  Ptr v563c16ecb3d0 = l_inst;
  Fun_563c16ecb070 v563c16ecb380 = fe_to_store;
  Ptr v563c16ecac80 = v563c16ecb380(v563c16ecb3d0);
  Ptr l_store = v563c16ecac80;
  Ptr v563c16ecb510 = p_ptr;
  Ptr v563c16ecb4c0 = l_store;
  Ptr v563c16ecb470 = &((Struct_563c16ebd580*)v563c16ecb4c0)->ptr;
  *(Ptr*)v563c16ecb470 = v563c16ecb510;
  Ptr v563c16ecb650 = p_val;
  Ptr v563c16ecb600 = l_store;
  Ptr v563c16ecb5b0 = &((Struct_563c16ebd580*)v563c16ecb600)->val;
  *(Ptr*)v563c16ecb5b0 = v563c16ecb650;
  Ptr v563c16ecb730 = l_inst;
  return v563c16ecb730;
  v563c16eca1b0_break:
}

Ptr fe_to_store(Ptr p_inst) {
  Ptr v563c16ecb240 = p_inst;
  Ptr v563c16ecb1a0 = (Ptr)v563c16ecb240;
  int64_t v563c16ecb290 = (int64_t)24ll;
  Ptr v563c16ecb330 = (Ptr)(v563c16ecb1a0 + v563c16ecb290);
  return v563c16ecb330;
  v563c16ecb0b0_break:
}

Ptr fe_new_block(Ptr p_f) {
  Ptr v563c16ecc180 = p_f;
  uint64_t v563c16ecc1d0 = (uint64_t)32llu;
  Fun_563c16ecc0f0 v563c16ecc130 = malloc;
  Ptr v563c16ecbd90 = v563c16ecc130(v563c16ecc180, v563c16ecc1d0);
  Ptr l_block = v563c16ecbd90;
  Ptr v563c16ecc420 = p_f;
  Ptr v563c16ecc470 = l_block;
  Fun_563c16ec3c80 v563c16ecc3d0 = fe_inst_bookend;
  Ptr v563c16ecc360 = v563c16ecc3d0(v563c16ecc420, v563c16ecc470);
  Ptr l_bookend = v563c16ecc360;
  Ptr v563c16ecc5b0 = l_bookend;
  Ptr v563c16ecc560 = l_block;
  Ptr v563c16ecc510 = &((Struct_563c16ebc9a0*)v563c16ecc560)->start;
  *(Ptr*)v563c16ecc510 = v563c16ecc5b0;
  Ptr v563c16ecc6f0 = l_bookend;
  Ptr v563c16ecc6a0 = l_block;
  Ptr v563c16ecc650 = &((Struct_563c16ebc9a0*)v563c16ecc6a0)->end;
  *(Ptr*)v563c16ecc650 = v563c16ecc6f0;
  Ptr v563c16ecc830 = p_f;
  Ptr v563c16ecc7e0 = l_block;
  Ptr v563c16ecc790 = &((Struct_563c16ebc9a0*)v563c16ecc7e0)->function;
  *(Ptr*)v563c16ecc790 = v563c16ecc830;
  Ptr v563c16ecc8d0 = l_block;
  return v563c16ecc8d0;
  v563c16ecbc00_break:
}

