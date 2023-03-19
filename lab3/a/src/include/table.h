#ifndef TABLE_H
#define TABLE_H
#define ELEM_FMT "%d\t%s\t%s\t%zu\n"

#include "types.h"

typedef struct Item {
    InfoType *info;
} Item;

typedef struct KeySpace {
    BusyType busy;
    KeyType key;
    KeyType par;
    Item *info;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    IndexType msize;
    IndexType csize;
} Table;

Table *init_table(IndexType msize);

void swap(void *p1, void *p2);
int compare_keys(KeyType k1, KeyType k2);

void print_element(KeySpace *element);
void print_table(Table *table);

void free_element(KeySpace *element);
void free_table(Table *table);

int remove_garbage(Table *table);
IndexType search(Table *table, KeySpace *element);
void remove_element(Table *table, KeySpace *element);
int insert(Table *table, KeySpace *element);

#endif // TABLE_H
