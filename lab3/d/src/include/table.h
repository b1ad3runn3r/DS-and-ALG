#ifndef TABLE_H
#define TABLE_H
#define ELEM_FMT "%ld\t%s\t%s\t%d\n"

#include <stdio.h>
#include "types.h"

typedef struct Item {
    long offset;
} Item;

typedef struct KeySpace {
    long busy;
    long key_len;
    long key_offset;
    long par_len;
    long par_offset;
} KeySpace;

typedef struct Table {
    FILE *fp;
    char *filename;
    int msize;
    int csize;
} Table;

int load_table(Table *, const char *filename);
int save_table(Table *);

Table *init_table();

static inline int compare_keys(const char *k1, const char *k2);

void free_table(Table *table);

int f_print_element(FILE *fp, const KeySpace *element, const Item *item);
int f_print_table(Table *table);

int f_remove_garbage(Table *table);
int f_search(const Table *table, const char *key);
int f_remove_element(Table *table, char *key, int idx);
int f_insert(Table *table, const char *key, const char *par, int data);

#endif // TABLE_H
