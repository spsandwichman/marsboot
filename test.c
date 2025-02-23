#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef struct Struct_5617ea8a3c10 {
  int64_t data;
  Ptr next;
  Ptr prev;
} Struct_5617ea8a3c10;
typedef void (*Fun_5617ea8a41a0)(Ptr, Ptr);
typedef void (*Fun_5617ea8a4fd0)(Ptr, Ptr);
typedef void (*Fun_5617ea8a5bf0)(Ptr, Ptr);

void test_append_back(Ptr list, Ptr new);
void test_insert_before(Ptr node, Ptr new);
void test_insert_after(Ptr node, Ptr new);

void test_append_back(Ptr list, Ptr new) {
  Ptr v5617ea8a42b0 = list;
  _Bool v5617ea8a42f0 = (_Bool)v5617ea8a42b0;
  _Bool v5617ea8a4330 = !v5617ea8a42f0;
  _Bool v5617ea8a4430 = v5617ea8a4330;
  if (!v5617ea8a4330) {
    Ptr v5617ea8a4370 = new;
    _Bool v5617ea8a43b0 = (_Bool)v5617ea8a4370;
    _Bool v5617ea8a43f0 = !v5617ea8a43b0;
    v5617ea8a4430 = v5617ea8a43f0;
  }
  if (v5617ea8a4430)
  {
    return ;
  }
  Ptr v5617ea8a4670 = list;
  Ptr end = v5617ea8a4670;
  while (1) {
    Ptr v5617ea8a4730 = end;
    Ptr v5617ea8a46f0 = ((Struct_5617ea8a3c10*)v5617ea8a4730)->next;
    _Bool v5617ea8a4770 = (_Bool)v5617ea8a46f0;
    if (!v5617ea8a4770) break;
    {
      Ptr v5617ea8a4a30 = end;
      Ptr v5617ea8a49f0 = ((Struct_5617ea8a3c10*)v5617ea8a4a30)->next;
      Ptr v5617ea8a49b0 = &end;
      *(Ptr*)v5617ea8a49b0 = v5617ea8a49f0;
    }
  }
  Ptr v5617ea8a4b30 = new;
  Ptr v5617ea8a4af0 = end;
  Ptr v5617ea8a4ab0 = &((Struct_5617ea8a3c10*)v5617ea8a4af0)->next;
  *(Ptr*)v5617ea8a4ab0 = v5617ea8a4b30;
  Ptr v5617ea8a4c30 = end;
  Ptr v5617ea8a4bf0 = new;
  Ptr v5617ea8a4bb0 = &((Struct_5617ea8a3c10*)v5617ea8a4bf0)->prev;
  *(Ptr*)v5617ea8a4bb0 = v5617ea8a4c30;
}

void test_insert_before(Ptr node, Ptr new) {
  Ptr v5617ea8a50e0 = node;
  _Bool v5617ea8a5120 = (_Bool)v5617ea8a50e0;
  _Bool v5617ea8a5160 = !v5617ea8a5120;
  _Bool v5617ea8a5260 = v5617ea8a5160;
  if (!v5617ea8a5160) {
    Ptr v5617ea8a51a0 = new;
    _Bool v5617ea8a51e0 = (_Bool)v5617ea8a51a0;
    _Bool v5617ea8a5220 = !v5617ea8a51e0;
    v5617ea8a5260 = v5617ea8a5220;
  }
  if (v5617ea8a5260)
  {
    return ;
  }
  Ptr v5617ea8a5510 = node;
  Ptr v5617ea8a54d0 = ((Struct_5617ea8a3c10*)v5617ea8a5510)->prev;
  Ptr v5617ea8a5490 = new;
  Ptr v5617ea8a5450 = &((Struct_5617ea8a3c10*)v5617ea8a5490)->prev;
  *(Ptr*)v5617ea8a5450 = v5617ea8a54d0;
  Ptr v5617ea8a5610 = node;
  Ptr v5617ea8a55d0 = new;
  Ptr v5617ea8a5590 = &((Struct_5617ea8a3c10*)v5617ea8a55d0)->next;
  *(Ptr*)v5617ea8a5590 = v5617ea8a5610;
  Ptr v5617ea8a5750 = new;
  Ptr v5617ea8a5710 = node;
  Ptr v5617ea8a56d0 = ((Struct_5617ea8a3c10*)v5617ea8a5710)->prev;
  Ptr v5617ea8a5690 = &((Struct_5617ea8a3c10*)v5617ea8a56d0)->next;
  *(Ptr*)v5617ea8a5690 = v5617ea8a5750;
  Ptr v5617ea8a5850 = new;
  Ptr v5617ea8a5810 = node;
  Ptr v5617ea8a57d0 = &((Struct_5617ea8a3c10*)v5617ea8a5810)->prev;
  *(Ptr*)v5617ea8a57d0 = v5617ea8a5850;
}

void test_insert_after(Ptr node, Ptr new) {
  Ptr v5617ea8a5d00 = node;
  _Bool v5617ea8a5d40 = (_Bool)v5617ea8a5d00;
  _Bool v5617ea8a5d80 = !v5617ea8a5d40;
  _Bool v5617ea8a5e80 = v5617ea8a5d80;
  if (!v5617ea8a5d80) {
    Ptr v5617ea8a5dc0 = new;
    _Bool v5617ea8a5e00 = (_Bool)v5617ea8a5dc0;
    _Bool v5617ea8a5e40 = !v5617ea8a5e00;
    v5617ea8a5e80 = v5617ea8a5e40;
  }
  if (v5617ea8a5e80)
  {
    return ;
  }
  Ptr v5617ea8a6130 = node;
  Ptr v5617ea8a60f0 = ((Struct_5617ea8a3c10*)v5617ea8a6130)->next;
  Ptr v5617ea8a60b0 = new;
  Ptr v5617ea8a6070 = &((Struct_5617ea8a3c10*)v5617ea8a60b0)->next;
  *(Ptr*)v5617ea8a6070 = v5617ea8a60f0;
  Ptr v5617ea8a6230 = node;
  Ptr v5617ea8a61f0 = new;
  Ptr v5617ea8a61b0 = &((Struct_5617ea8a3c10*)v5617ea8a61f0)->prev;
  *(Ptr*)v5617ea8a61b0 = v5617ea8a6230;
  Ptr v5617ea8a63b0 = new;
  Ptr v5617ea8a6370 = node;
  Ptr v5617ea8a6330 = ((Struct_5617ea8a3c10*)v5617ea8a6370)->next;
  Ptr v5617ea8a62f0 = &((Struct_5617ea8a3c10*)v5617ea8a6330)->prev;
  *(Ptr*)v5617ea8a62f0 = v5617ea8a63b0;
  Ptr v5617ea8a64f0 = new;
  Ptr v5617ea8a64b0 = node;
  Ptr v5617ea8a6470 = &((Struct_5617ea8a3c10*)v5617ea8a64b0)->next;
  *(Ptr*)v5617ea8a6470 = v5617ea8a64f0;
}

