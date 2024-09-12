#include "parse.h"

#define EXTRA_INIT_CAP 64
#define NODES_INIT_CAP 128

forceinline ParseNode* get_parse_node(ParseTree* tree, ParseNodeIndex index) {
    return &tree->nodes[index];
}

forceinline u8 get_parse_node_kind(ParseTree* tree, ParseNodeIndex index) {
    return tree->node_kinds[index];
}

forceinline ParseNodeIndex get_extra_index(ParseTree* tree, ParseExtraIndex index) {
    return tree->extra.at[index];
}

forceinline ParseExtraVarDecl* get_extra_var_decl(ParseTree* tree, ParseExtraIndex index) {
    return (ParseExtraVarDecl*) &tree->extra.at[index];
}

forceinline ParseExtraFnDecl* get_extra_fn_decl(ParseTree* tree, ParseExtraIndex index) {
    return (ParseExtraFnDecl*) &tree->extra.at[index];
}

static ParseExtraIndex reserve_extra_slots(ParseTree* tree, u32 slots) {
    if (tree->extra.len + slots >= tree->extra.cap) {
        tree->extra.cap *= 2;
        tree->extra.at = realloc(tree->extra.at, sizeof(tree->extra.at[0]) * tree->extra.cap);
    }
    ParseExtraIndex i = tree->extra.len;
    tree->extra.len += slots;
    return i;
}

static ParseExtraIndex new_extra(ParseTree* tree, u8 kind) {
    u64 size_requested;
    switch (kind) {
    case PN_FN_DECL:
        size_requested = sizeof(ParseExtraFnDecl);
    case PN_MUT_DECL:
    case PN_LET_DECL:
        size_requested = sizeof(ParseExtraVarDecl);
    default:
        CRASH("invalid new_extra kind");
    }

    return reserve_extra_slots(tree, sizeof(ParseExtraFnDecl) / sizeof(ParseNodeIndex));
}

static ParseExtraIndex new_extra_list(ParseTree* tree, u32 len) {
    return reserve_extra_slots(tree, len);
}

// does NOT create extra
ParseNodeIndex new_parse_node(ParseTree* tree, u8 type) {
    if (tree->len == tree->cap) {
        tree->cap *= 2;
        tree->nodes = realloc(tree->nodes, sizeof(tree->nodes[0]) * tree->cap);
        tree->node_kinds = realloc(tree->node_kinds, sizeof(tree->node_kinds[0]) * tree->cap);
    }

    ParseNodeIndex index = tree->len;
    tree->len++;
    
    // memset empty
    *get_parse_node(tree, index) = (ParseNode){0};

    return index;
}

ParseTree new_parse_tree(TokenBuf tb) {
    ParseTree tree = {0};
    tree.tokens = tb;
    
    da_init(&tree.extra, EXTRA_INIT_CAP);
    
    tree.len = 0;
    tree.cap = NODES_INIT_CAP;
    tree.nodes = malloc(sizeof(tree.nodes[0]) * tree.cap);
    tree.node_kinds = malloc(sizeof(tree.node_kinds[0]) * tree.cap);

    return tree;
}