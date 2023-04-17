#include "include/table.h"
#include "include/types.h"
#include <stdio.h>
#include <string.h>

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
            element->release,
            *(element->info->info)
        );
}

void print_table(const Table *table) {
    printf("Busy\tKey\tRelease\tInfo\n");
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

IndexType search(const Table *table, const KeySpace *element, int release, IndexType *last_idx) {
    if (!table || !element) {
        return E_ALLOC;
    }

    for (IndexType i = *last_idx; i < table->csize; ++i) {
        if ((table->ks + i)->busy && compare_keys((table->ks + i), element, release)) {
            *last_idx = i + 1;
            return i;
        }
    }

    return E_NOTFOUND;
}

int remove_element(Table *table, const KeySpace *element, int release) {
    if (!table || !element) {
        return E_NULLPTR;
    }

    IndexType last_idx = 0, idx = search(table, element, release, &last_idx);
    while (idx != E_NOTFOUND) {
        (table->ks + idx)->busy = 0;
        idx = search(table, element, release, &last_idx);
    }

    return E_OK;
}

int insert(Table *table, const KeySpace *element) {
    if (!table || !element) {
        return E_NULLPTR;
    }

    if (table->csize == table->msize) {
        if (remove_garbage(table) != E_OK) {
            if (table->csize > 0) {
                return E_OVERFLOW;
            }
        }
    }

    IndexType last_idx = 0, tmp_idx = -1;
    IndexType idx = search(table, element, 0, &last_idx);
    RelType release = 0;

    while (idx >= 0) {
        tmp_idx = idx;
        idx = search(table, element, 0, &last_idx);
    }

    if (table->csize && tmp_idx != E_NOTFOUND) {
        release = (table->ks + tmp_idx)->release + 1;
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

    table->ks = tmp_ptr;
    *(table->ks + table->csize) = *element;
    (table->ks + table->csize)->busy = 1;
    (table->ks + table->csize)->release = release;
    table->csize += 1;

    return E_OK;
}
