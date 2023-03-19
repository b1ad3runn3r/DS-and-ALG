#ifndef TABLE_H
#define TABLE_H

#include "types.h"

#define ELEM_FMT "%d\t%s\t%s\t%zu\n"

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

void print_element(KeySpace *element);
void print_table(Table *table);

void free_element(KeySpace *element);
void free_table(Table *table);

#endif // TABLE_H
