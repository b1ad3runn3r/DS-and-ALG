#ifndef TABLE_H
#define TABLE_H
#define ELEM_FMT "%d\t%s\t%s\t%d\n"

#include "types.h"

typedef struct Item {
    InfoType *info;
} Item;

typedef struct KeySpace {
    BusyType busy;
    KeyType *key;
    KeyType *par;
    Item *info;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    IndexType msize;
    IndexType csize;
} Table;

Table *init_table(IndexType msize);

static inline void swap(KeySpace *p1, KeySpace *p2);
static inline int compare_keys(const KeyType *k1, const KeyType *k2);

void print_element(const KeySpace *element);
void print_table(const Table *table);

void free_element(KeySpace *element);
void free_table(Table *table);

int remove_garbage(Table *table);
IndexType search(const Table *table, const KeySpace *element, int parent);
void remove_element(Table *table, KeySpace *element);
int insert(Table *table, KeySpace *element);

#endif // TABLE_H
