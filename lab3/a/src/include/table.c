#include "table.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

void swap(void *p1, void *p2) {
    void *tmp = p1;
    p1 = p2;
    p2 = tmp;
}

int compare_keys(KeyType k1, KeyType k2) {
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
    table->ks = calloc(msize, sizeof(KeySpace));
    if (!table->ks) {
        free(table);
        return NULL;
    }

    return table;
}

void print_element(KeySpace *element) {
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

void print_table(Table *table) {
    printf("Busy\tKey\tPar\tInfo\n");
    for (IndexType i = 0; i < table->msize; i++) {
        print_element(&table->ks[i]);
    }
}

void free_element(KeySpace *element) {
    if (!element || !element->key){
        return ;
    }
        
    free(element->info->info);
    free(element->info);
    free(element->key);
}

void free_table(Table *table) {
    if (!table) {
        return ;
    }

    for (IndexType i = 0; i < table->csize; i++) {
        free_element(table->ks + i);
    }
    free(table->ks);
    free(table);
}

int remove_garbage(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    IndexType idx = 0;
    for (IndexType i = 0; i < table->msize; i++) {
        if (table->ks[i].busy) {
            swap(table->ks + idx, table->ks + i);
            ++idx;
        }
    }

    table->csize = idx;
    return table->csize == table->msize; //1 if table is full
}

IndexType search(Table *table, KeySpace *element) {
    if (!table || !element) {
        return 1;
    }

    for (int i = 0; i < table->msize; i++) {
        if ((table->ks + i)->busy && !compare_keys((table->ks + i)->key, element->key)) {
            return i; // TODO: Make comparator a separate function
        }
    }

    return E_NOTFOUND;
}

void remove_element(Table *table, KeySpace *element) {
    if (!table || !element) {
        return ;
    }
    
    //TODO: make removal recursive

    IndexType idx = search(table, element);
    if (idx != E_NOTFOUND) {
        (table->ks + idx)->busy = 0;
    }
}

int insert(Table *table, KeySpace *element) {
    if (!table || !element) {
        return E_NULLPTR;
    }
    

    if (table->csize == table->msize) {
        if (remove_garbage(table)) { //TODO: fix reinserting to not busy elements
            return E_TABLEOVERFLOW;
        }
    }

    if (search(table, element) != E_NOTFOUND) {
        return E_DUPLICATE;
    }

    table->ks[table->csize] = *element;
    (table->ks + table->csize)->busy = 1;
    ++(table->csize);
    return E_OK;
}
