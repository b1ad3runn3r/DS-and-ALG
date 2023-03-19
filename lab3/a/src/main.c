#include <stdio.h>
#include "include/types.h"
#include "include/table.h"

int main() {
    IndexType msize = 10;
    Table *table = init_table(msize);

    Item *item = calloc(1, sizeof(Item));
    item->info = calloc(1, sizeof(size_t));
    *(item->info) = 5;

    KeySpace elem = {0};
    elem.key = calloc(5, sizeof(char));
    (elem.key)[0] = 'a';
    elem.par = NULL;
    elem.busy = 0;
    elem.info = item;

    insert(table, &elem);
    print_table(table);
    if (insert(table, &elem)) {
        printf("Insertion error\n");
    }

    remove_element(table, &elem);
    print_table(table);

    if (!table) {
        return 1;
    }

    free_table(table);
    return 0;
}