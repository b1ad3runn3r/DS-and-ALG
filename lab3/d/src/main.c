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

    const int n_opts = 5;
    const char *opts[] = {
            "0. Quit",
            "1. Insert",
            "2. Remove",
            "3. Search",
            "4. Print table",
    };

    int (* const f_opts[])(Table *) = {
            NULL,
            d_insert,
            d_remove,
            d_search,
            d_print
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

        msize = next_prime(msize);
        table->msize = msize;

        save_table(table);
        char *tmp = calloc(table->msize, sizeof(KeySpace) + sizeof(Item));
        if (!tmp) {
            free_table(table);
            return EXIT_FAILURE;
        }

        fseek(table->fp, 0, SEEK_END);
        fwrite(tmp, sizeof(KeySpace) + sizeof(Item), table->msize, table->fp);
        free(tmp);
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