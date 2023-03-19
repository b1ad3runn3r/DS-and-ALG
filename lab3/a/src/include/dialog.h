#ifndef DIALOG_H
#define DIALOG_H

#include <stdlib.h>
#include "table.h"

void print_opts(const char *opts[], size_t size);
int choose_option(int opt_size, int *option);

int d_insert(Table *table);
int d_remove(Table *table);
int d_search(Table *table);
int d_print(Table *table);

#endif //DIALOG_H
