#include <stdio.h>
#include <string.h>
#include "include/table.h"
#include "include/types.h"

int load_table(Table *table, const char *filename) {
    if (!table) {
        return E_NULLPTR;
    }

    table->filename = strdup(filename);
    if (!table->filename) {
        return E_ALLOC;
    }

    table->fp = fopen(filename, "rb+");
    if (!table->fp) {
        return E_NOFILE;
    }

    fseek(table->fp, 0, SEEK_SET);
    fread(&table->msize, sizeof(int), 1, table->fp);
    // fread(&table->csize, sizeof(int), 1, table->fp);

    return E_OK;
}

int save_table(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    fseek(table->fp, 0, SEEK_SET);
    fwrite(&table->msize, sizeof(int), 1, table->fp);
    fwrite(&table->csize, sizeof(int), 1, table->fp);

    return E_OK;
}

static inline int compare_keys(const char *k1, const char *k2) {
    return strcmp(k1, k2);
}

Table *init_table() {
    return calloc(1, sizeof(Table));
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
    for (int i = 0; i < table->csize; ++i) {
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

    for (int i = 0; i < table->csize; ++i) {
        free_element(table->ks + i);
    }

    if (table->csize > 0) {
        free(table->ks);
    }

    free(table->filename);
    free(table);
}

int remove_garbage(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->ks) {
        return E_NULLPTR;
    }

    for (int i = 0; i < table->csize; ++i) {
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

int search(const Table *table, const KeySpace *element, int parent) {
    if (!table || !element) {
        return E_ALLOC;
    }

    if (parent && !(element->par)) {
        return E_NULLPTR;
    }

    for (int i = 0; i < table->csize; ++i) {
        if (parent) {
            if ((table->ks + i)->busy && !compare_keys((table->ks + i)->key, element->par)) {
                return i;
            }
        }
        else {
            if ((table->ks + i)->busy && !compare_keys((table->ks + i)->key, element->key)) {
                return i;
            }
        }
    }

    return E_NOTFOUND;
}

int remove_element(Table *table, const KeySpace *element) {
    if (!table || !element) {
        return E_NULLPTR;
    }

    int idx = search(table, element, 0);
    if (idx == E_NOTFOUND) {
        return E_NOTFOUND;
    }
    KeySpace *cur_elem = table->ks + idx;
    cur_elem->busy = 0;

    for (int i = 0; i < table->csize; ++i) {
        if ((table->ks + i)->par) {
            if (!compare_keys(cur_elem->key, (table->ks + i)->par)) {
                remove_element(table, table->ks + i);
            }
        }
    }

    return E_OK;
}

int insert(Table *table, const KeySpace *element) {
    if (!table || !element) {
        return E_NULLPTR;
    }

    if (element->par) {
        if (search(table, element, 1) == E_NOTFOUND) {
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

    if (search(table, element, 0) != E_NOTFOUND) {
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

    table->ks = tmp_ptr;
    *(table->ks + table->csize) = *element;
    (table->ks + table->csize)->busy = 1;
    table->csize += 1;

    return E_OK;
}
