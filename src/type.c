#include "sema.h"

TypeGraph tg;

void type_init() {
    
}

TypeNode* tnode(Type t) {
    return tg.handle2node.at[t];
}