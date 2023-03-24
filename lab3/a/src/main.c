#include <stdio.h>
#include "include/types.h"
#include "include/table.h"
#include "include/utils.h"
#include "include/dialog.h"

int main() {
    int n_opts = 5;
    const char *opts[] = {
            "0. Quit",
            "1. Insert",
            "2. Remove",
            "3. Search",
            "4. Print table",
    };

    int (*f_opts[])(Table *) = {
            NULL,
            d_insert,
            d_remove,
            d_search,
            d_print
    };

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

    print_opts(opts, 5);
    int choice = 0;
    int f_res = 0;

    while ((choice = dialog(opts, 5))) {
        if (f_opts[choice]) {
            f_res = f_opts[choice](table);
            if (f_res != E_OK) {
                printf("Wrong input! Try again\n");
            }
        }
        else {
            break;
        }
    }

    free_table(table);
    return 0;
}