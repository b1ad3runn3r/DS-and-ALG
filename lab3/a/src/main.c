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
    int option = -1;
    while (option != 0){
        if (choose_option(sizeof(opts) / sizeof(opts[0]), &option) == E_ALLOCERROR) {
            free_table(table);
            return EXIT_FAILURE;
        }
        switch (option) {
            case 0: // Quit
                break;

            case 1: // Insert
                d_insert(table);
                break;

            case 2: // Remove
                d_remove(table);
                break;

            case 3: // Search
                d_search(table);
                break;

            case 4: // Print table
                d_print(table);
                break;

            case 5: // Print menu
                print_opts(opts, sizeof(opts) / sizeof(opts[0]));
                break;

            default:
                printf("Unknown option\n");
                break;
        }
    }

    free_table(table);
    return 0;
}