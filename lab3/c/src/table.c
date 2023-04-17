#include "include/table.h"
#include "include/types.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

static inline int hash1(const Table *table, KeyType key) {
    int hash = INT_MAX;
    for (int i = 0; i < sizeof(key); ++i) {
        hash = 37 * hash + ((char *)&key)[i];
    }

    return abs(hash) % table->msize;
}

static inline int hash2(const Table *table, KeyType key) {
    return key % (table->msize - 1) + 1;
}

static inline int hash(const Table *table, KeyType key, int i) {
    return (hash1(table, key) + i * hash2(table, key)) % table->msize;
}

static inline int compare_keys(const KeySpace *k1, const KeySpace *k2, int release) {
    if (release) {
        return (k1->key == k2->key) && (k1->release == k2->release);
    }
    else {
        return k1->key == k2->key;
    }
}

Table *init_table(IndexType msize) {
    if (msize <= 0) {
        return NULL;
    }

    Table *table = calloc(1, sizeof(Table));
    if (!table) {
        return NULL;
    }


    table->msize = msize;
    table->ks = calloc(table->msize, sizeof(KeySpace));

    return table;
}

void print_element(const KeySpace *element) {
    if (!element || !element->key) {
        return ;
    }

    printf( 
            ELEM_FMT,
            element->busy,
            element->key,
            element->release,
            *(element->info->info)
        );
}

void print_table(const Table *table) {
    printf("Busy\tKey\tRelease\tInfo\n");
    for (IndexType i = 0; i < table->msize; ++i) {
        print_element(table->ks + i);
    }
}

void free_element(KeySpace *element) {
    if (!element){
        return ;
    }

    if (!element->info) {
        return ;
    }
        
    free(element->info->info);
    free(element->info);
}

void free_table(Table *table) {
    if (!table) {
        return ;
    }

    for (IndexType i = 0; i < table->msize; ++i) {
        free_element(table->ks + i);
    }

    free(table->ks);
    free(table);
}

IndexType search(const Table *table, const KeySpace *element, int release, int *last_idx) {
    if (!table || !element) {
        return E_ALLOC;
    }

    for (int i = *last_idx; i < table->msize; ++i) {
        IndexType idx = hash(table, element->key, i);
        if (table->ks[idx].busy && compare_keys(table->ks + idx, element, release)) {
            *last_idx = i + 1;
            return idx;
        }
    }

    return E_NOTFOUND;
}

int remove_element(Table *table, const KeySpace *element, int release) {
    if (!table || !element) {
        return E_NULLPTR;
    }

    for(int i = 0; i < table->msize; ++i) {
        IndexType idx = hash(table, element->key, i);
        if (table->ks[idx].busy && compare_keys(table->ks + idx, element, release)) {
            table->ks[idx].busy = 0;
            return E_OK;
        }
    }

    return E_OK;
}

int insert(Table *table, const KeySpace *element) {
    if (!table || !element) {
        return E_NULLPTR;
    }

    int release = 0;
    for (int i = 0; i < table->msize; ++i) {
        IndexType idx = hash(table, element->key, i);
        if (table->ks[idx].busy && compare_keys(table->ks + idx, element, 0)) {
            release += 1;
            continue;
        }

        if (!table->ks[idx].busy) {
            table->ks[idx].busy = 1;
            table->ks[idx].key = element->key;
            table->ks[idx].release = release;
            if (table->ks[idx].info) {
                free(table->ks[idx].info->info);
                free(table->ks[idx].info);
            }
            table->ks[idx].info = element->info;
            return E_OK;
        }
    }

    return E_INSERT;
}
