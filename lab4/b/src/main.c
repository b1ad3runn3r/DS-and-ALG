#include "include/llrb_tree.h"
#include "include/dialog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *opts[] = {
        "0. Quit",
        "1. Insert",
        "2. Delete",
        "3. Traverse",
        "4. Search",
        "5. Special search",
        "6. Print tree",
        "7. Load tree",
        "8. Timing"
};

int (*const f_opts[])(Node **) = {
        NULL,
        d_insert,
        d_delete,
        d_traverse,
        d_search,
        d_search_min,
        d_print_tree,
        d_load_tree
};

int n_opts = 8;

int main(int argc, char **argv) {
    Node *root = NULL;

    print_opts(opts, n_opts);
    putchar('\n');
    int choice = 0;
    int f_res = 0;

    while ((choice = dialog(n_opts))) {
        if (f_opts[choice]) {
            f_res = f_opts[choice](&root);
        }
        else {
            break;
        }

        print_opts(opts, n_opts);
        putchar('\n');
    }
    free_tree(&root);
    return 0;
}
