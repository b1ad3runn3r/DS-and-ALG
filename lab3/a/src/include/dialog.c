#include "dialog.h"
#include "utils.h"
#include "types.h"
#include <stdio.h>

void print_opts(const char *opts[], size_t size) {
    for (int i = 0; i < size; ++i) {
        printf("%s\n", opts[i]);
    }
}

int choose_option(int opt_size, int *option) {
    int opt = 0;
    int flag = E_WRONGINPUT;
    while (flag != E_OK || opt < 0 || opt > opt_size - 1) {
        flag = get_int("Enter option: ", &opt);
        if (flag == E_ALLOCERROR) {
            return E_ALLOCERROR;
        }
    }
    *option = opt;
    return E_OK;
}

int d_insert(Table *table) {
    return E_OK;
}

int d_remove(Table *table) {
    return E_OK;
}

int d_search(Table *table) {
    return E_OK;
}

int d_print(Table *table) {
    return E_OK;
}

