#ifndef TREE_H
#define TREE_H

#include "types.h"

typedef struct Tree {
    KeyType key;
    struct Tree *left;
    struct Tree *right;
    struct Tree *parent;
    DataType *info;
} Tree;

static inline int is_equal(KeyType k1, KeyType k2);
static inline int greater_than(KeyType k1, KeyType k2);
static inline DataType *copy(DataType *info);

void print_element(Tree *element);
void print_tree(Tree *ptr, int level);

static void free_element(Tree *element);
void free_tree(Tree **root);

void traverse(Tree **root, KeyType k1);

Tree** find(Tree *ptr, KeyType key, size_t *result_size);
Tree** find_min(Tree *ptr, size_t *result_size);

static Tree* find_last(Tree *ptr, KeyType key);
static Tree *find_last_min(Tree *ptr);

int insert(Tree **root, KeyType key, DataType *info);
int delete(Tree **root, KeyType key);

#endif // TREE_H
