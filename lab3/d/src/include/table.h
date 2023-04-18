#ifndef TABLE_H
#define TABLE_H
#define ELEM_FMT "%ld\t%zu\t%ld\t%zu\n"

#include <stdio.h>
#include "types.h"

typedef struct Item {
    long offset;
} Item;

typedef struct KeySpace {
    long busy;
    long key_offset;
    RelType release;
} KeySpace;

typedef struct Table {
    FILE *fp;
    char *filename;
    int msize;
} Table;

int load_table(Table *, const char *filename);
int save_table(Table *);

Table *init_table();

static inline int hash1(const Table *table, KeyType key);
static inline int hash2(const Table *table, KeyType key);
static inline int hash(const Table *table, KeyType key, int i);

static inline int compare_keys(KeyType k1, KeyType k2, RelType r1, RelType r2, int release);

void free_table(Table *table);

int f_print_element(FILE *fp, const KeySpace *element, const Item *item);
int f_print_table(Table *table);

int f_search(const Table *table, KeyType key, RelType elem_release, int has_release, int *last_idx);
int f_remove_element(Table *table, KeyType key, RelType elem_release, int has_release);
int f_insert(Table *table, KeyType key, InfoType data);

#endif // TABLE_H
