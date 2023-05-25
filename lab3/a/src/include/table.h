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

static inline int compare_keys(const KeyType *k1, const KeyType *k2);

void print_element(const KeySpace *element);
void print_table(const Table *table);

void free_element(KeySpace *element);
void free_table(Table *table);

int remove_garbage(Table *table);
IndexType search(const Table *table, const KeyType *key, const KeyType *par, int parent);
int remove_element(Table *table, const KeyType *key);
int insert(Table *table, KeyType *key, KeyType *par, InfoType* info);

#endif // TABLE_H
