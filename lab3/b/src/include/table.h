#ifndef TABLE_H
#define TABLE_H
#define ELEM_FMT "%d\t%s\t%s\t%d\n"

#include <stdio.h>
#include "types.h"

typedef struct Item {
    int *info;
    long offset;
} Item;

typedef struct KeySpace {
    int busy;
    char *key;
    char *par;
    Item *info;
    long key_len;
    long key_offset;
    long par_len;
    long par_offset;
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

int f_print_element(FILE *fp, KeySpace *element, Item *item);
int f_print_table(Table *table);

int f_remove_garbage(Table *table);
int f_search(const Table *table, const char *key);
int f_remove_element(Table *table, const char *key);
int f_insert(Table *table, const char *key, const char *par, int data);

#endif // TABLE_H
