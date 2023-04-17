#ifndef TABLE_H
#define TABLE_H
#define ELEM_FMT "%d\t%zu\t%zu\t%zu\n"

#include "types.h"

typedef struct Item {
    InfoType *info;
} Item;

typedef struct KeySpace {
    BusyType busy;
    KeyType key;
    RelType release;
    Item *info;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    IndexType msize;
    IndexType csize;
} Table;

Table *init_table(IndexType msize);

static inline int compare_keys(const KeySpace *k1, const KeySpace *k2, int release);

void print_element(const KeySpace *element);
void print_table(const Table *table);

void free_element(KeySpace *element);
void free_table(Table *table);

int remove_garbage(Table *table);
IndexType search(const Table *table, const KeySpace *element, int release, IndexType *last_idx);
int remove_element(Table *table, const KeySpace *element, int release);
int insert(Table *table, const KeySpace *element);

#endif // TABLE_H
