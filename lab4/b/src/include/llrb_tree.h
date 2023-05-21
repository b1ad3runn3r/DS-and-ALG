#ifndef LLRB_TREE_H
#define LLRB_TREE_H

#include <stdint.h>
#include <stdio.h>

typedef char key_tt;
typedef uint64_t value_t;

typedef enum {
    RED = 0,
    BLACK = 1
} color_t;

typedef struct Node {
    key_tt *key;
    value_t *value;
    uint64_t value_size;
    color_t color;
    struct Node *left;
    struct Node *right;
} Node;

value_t *search(Node **root, key_tt *key, uint64_t position);
value_t *search_min(Node **root, uint64_t position);

int insert(Node **root, key_tt *key, value_t value);

int delete(Node **root, key_tt *key, uint64_t position);

void traverse(Node *node, key_tt *start, key_tt *end);
void print_tree(Node *node, uint64_t level);

void free_tree(Node **root);

int load_tree(FILE *fp, Node **root);

#endif // LLRB_TREE_H
