#include <stdint.h>

typedef void* Ptr;
typedef struct Slice { Ptr raw; uint64_t len; } Slice;
typedef struct Dyn { Ptr raw; uint64_t id; } Dyn;
typedef Ptr (*Fun_558d84d33290)(uint64_t);
typedef void (*Fun_558d84d33520)(Ptr);
typedef Ptr (*Fun_558d84d33800)(Ptr, uint64_t);

Ptr malloc(uint64_t p_size);
void free(Ptr p_ptr);
Ptr realloc(Ptr p_ptr, uint64_t p_size);

