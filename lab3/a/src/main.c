#include <stdio.h>
#include "include/types.h"
#include "include/table.h"
#include "include/utils.h"
#include "include/dialog.h"

const char *opts[] = {
    "0. Quit",
    "1. Insert",
    "2. Remove",
    "3. Search",
    "4. Print table",
    "5. Print menu"
};

int main() {
    IndexType msize = 0;
    int flag = E_WRONGINPUT;
    while (flag != E_OK || msize <= 0) {
        flag = get_int("Enter table size: ", &msize);
        if (flag == E_ALLOCERROR) {
            return EXIT_FAILURE;
        }
    }

    clear_screen();

    Table *table = init_table(msize);
    if (!table) {
        return EXIT_FAILURE;
    }
    print_opts(opts, sizeof(opts) / sizeof(opts[0]));


    Item *item = calloc(1, sizeof(Item));
    item->info = calloc(1, sizeof(size_t));
    *(item->info) = 5;

    KeySpace elem = {0};
    elem.key = calloc(5, sizeof(char));
    (elem.key)[0] = 'a';
    elem.par = NULL;
    elem.info = item;

    insert(table, &elem);
    print_table(table);
    if (insert(table, &elem) == E_DUPLICATE) {
        printf("Insertion error\n");
    }

    remove_element(table, &elem);
    print_table(table);

    free_table(table);
    return 0;
}