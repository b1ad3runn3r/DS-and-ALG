#ifndef TABLE_H
#define TABLE_H
#define ELEM_FMT "%d\t%s\t%s\t%d\n"

#include <stdio.h>
#include "types.h"

typedef struct Item {
    int *info;
} Item;

typedef struct KeySpace {
    int busy;
    char *key;
    char *par;
    Item *info;
} KeySpace;

typedef struct Table {
    FILE *fp;
    char *filename;
    KeySpace *ks;
    int msize;
    int csize;
} Table;

int load_table(Table *, const char *filename);
int save_table(Table *);

Table *init_table();

static inline int compare_keys(const char *k1, const char *k2);

void print_element(const KeySpace *element);
void print_table(const Table *table);

void free_element(KeySpace *element);
void free_table(Table *table);

int remove_garbage(Table *table);
int search(const Table *table, const KeySpace *element, int parent);
int remove_element(Table *table, const KeySpace *element);
int insert(Table *table, const KeySpace *element);

#endif // TABLE_H
