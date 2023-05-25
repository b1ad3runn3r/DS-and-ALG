#include "table.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

static inline int compare_keys(const KeyType *k1, const KeyType *k2) {
    return strcmp(k1, k2);
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
    table->csize = 0;
    table->ks = NULL;

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
            element->par, 
            *(element->info->info)
        );
}

void print_table(const Table *table) {
    printf("Busy\tKey\tPar\tInfo\n");
    for (IndexType i = 0; i < table->csize; ++i) {
        print_element(table->ks + i);
    }
}

void free_element(KeySpace *element) {
    if (!element || !element->key){
        return ;
    }
        
    free(element->info->info);
    free(element->info);
    free(element->key);
    free(element->par);
}

void free_table(Table *table) {
    if (!table) {
        return ;
    }

    for (IndexType i = 0; i < table->csize; ++i) {
        free_element(table->ks + i);
    }

    if (table->csize > 0) {
        free(table->ks);
    }
    free(table);
}

int remove_garbage(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    for (IndexType i = 0; i < table->csize; ++i) {
        if ((table->ks + i)->busy == 0) {
            free_element(table->ks + i);
            memmove(table->ks + i, table->ks + i + 1, (table->csize - i - 1) * sizeof(KeySpace));
            --table->csize;
            --i;
        }
    }

    if (table->msize == table->csize) {
        return E_OVERFLOW;
    }
    else {
        KeySpace *tmp_ptr = NULL;
        tmp_ptr = realloc(table->ks, table->csize * sizeof(KeySpace));

        if (!tmp_ptr) {
            return E_ALLOC;
        }

        table->ks = tmp_ptr;
        return E_OK;
    }
}

IndexType search(const Table *table, const KeyType *key, const KeyType *par, int parent) {
    if (!table || !key) {
        return E_ALLOC;
    }

    if (parent && !par) {
        return E_NULLPTR;
    }

    for (IndexType i = 0; i < table->csize; ++i) {
        if (parent) {
            if ((table->ks + i)->busy && !compare_keys((table->ks + i)->key, par)) {
                return i;
            }
        }
        else {
            if ((table->ks + i)->busy && !compare_keys((table->ks + i)->key, key)) {
                return i;
            }
        }
    }

    return E_NOTFOUND;
}

int remove_element(Table *table, const KeyType *key) {
    if (!table || !key) {
        return E_NULLPTR;
    }

    int idx = search(table, key, NULL, 0);
    if (idx == E_NOTFOUND) {
        return E_NOTFOUND;
    }
    KeySpace *cur_elem = table->ks + idx;
    cur_elem->busy = 0;

    for (IndexType i = 0; i < table->csize; ++i) {
        if ((table->ks + i)->par) {
            if (!compare_keys(cur_elem->key, (table->ks + i)->par)) {
                remove_element(table, (table->ks + i)->key);
            }
        }
    }

    return E_OK;
}

int insert(Table *table, KeyType *key, KeyType *par, InfoType* info) {
    if (!table || !key || !info) {
        return E_NULLPTR;
    }

    if (par) {
        if (search(table, key, par, 1) == E_NOTFOUND) {
           return E_INSERT;
        }
    }

    if (table->csize == table->msize) {
        if (remove_garbage(table) != E_OK) {
            if (table->csize > 0) {
                return E_OVERFLOW;
            }
        }
    }

    if (search(table, key, par, 0) != E_NOTFOUND) {
        return E_INSERT;
    }

    KeySpace *tmp_ptr = NULL;
    if (table->csize == 0) {
        tmp_ptr = calloc(1, sizeof(KeySpace));
    }
    else {
        tmp_ptr = realloc(table->ks, (table->csize + 1) * sizeof(KeySpace));
    }

    if (!tmp_ptr) {
        return E_ALLOC;
    }

    Item *data = calloc(1, sizeof(Item));
    if (!data) {
        free(tmp_ptr);
        return E_ALLOC;
    }

    data->info = info;

    table->ks = tmp_ptr;
    (table->ks + table->csize)->key = key;
    (table->ks + table->csize)->par = par;
    (table->ks + table->csize)->busy = 1;
    (table->ks + table->csize)->info = data;
    table->csize += 1;

    return E_OK;
}
