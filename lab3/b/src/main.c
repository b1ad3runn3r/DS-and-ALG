#include <stdio.h>
#include "include/types.h"
#include "include/table.h"
#include "include/utils.h"
#include "include/dialog.h"

int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const int n_opts = 6;
    const char *opts[] = {
            "0. Quit",
            "1. Insert",
            "2. Remove",
            "3. Search",
            "4. Print table",
            "5. Remove garbage"
    };

    int (* const f_opts[])(Table *) = {
            NULL,
            d_insert,
            d_remove,
            d_search,
            d_print,
            d_garbage
    };

    Table *table = init_table();
    if (!table) {
        return EXIT_FAILURE;
    }

    if (load_table(table, argv[1]) != E_OK) {
        table->fp = fopen(argv[1], "wb+");
        if (!table->fp) {
            free_table(table);
            return EXIT_FAILURE;
        }

        int flag = E_WRONGINPUT;
        int msize = 0;

        while (flag != E_OK || msize <= 0) {
            flag = get_int("Enter table size: ", &msize);
            if (flag == E_ALLOC) {
                free_table(table);
                return EXIT_FAILURE;
            }
        }

        table->msize = msize;
        table->csize = 0;
        save_table(table);
    }

    print_opts(opts, n_opts);
    int choice = 0;
    int f_res = 0;

    while ((choice = dialog(n_opts))) {
        if (f_opts[choice]) {
            f_res = f_opts[choice](table);
            parse_result(f_res);
            save_table(table);
        }
        else {
            break;
        }
    }

    fclose(table->fp);
    free_table(table);
    return 0;
}