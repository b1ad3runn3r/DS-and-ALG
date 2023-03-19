#include "table.h"
#include "types.h"
#include <stdio.h>

Table *init_table(IndexType msize) {
    if (msize <= 0) {
        return NULL;
    }

    Table *table = calloc(1, sizeof(Table));
    if (!table) {
        return NULL;
    }

    table->msize = msize;
    table->ks = calloc(msize, sizeof(KeySpace));
    if (!table->ks) {
        free(table);
        return NULL;
    }

    return table;
}

void print_element(KeySpace *element) {
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

int garbage_collector(Table *table) {
    if (!table) {
        return 1;
    }

    IndexType idx = 0;
    for (IndexType i = 0; i < table->csize; i++) {
        if (table->ks[i].busy) {
            table->ks[idx++] = table->ks[i];
        }
    }

    table->csize = idx;
    return table->csize == table->msize;
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
        free_element(&table->ks[i]);
    }
    free(table->ks);
    free(table);
}
