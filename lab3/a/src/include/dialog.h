#ifndef DIALOG_H
#define DIALOG_H

#include <stdlib.h>
#include "table.h"

void print_opts(const char *opts[], size_t size);
int dialog(char **opts, int opts_size);

KeySpace *create_element(char* key, char* par, int data);

int d_insert(Table *table);
int d_remove(Table *table);
int d_search(Table *table);
int d_print(Table *table);

#endif //DIALOG_H
