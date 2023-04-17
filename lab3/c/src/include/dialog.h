#ifndef DIALOG_H
#define DIALOG_H

#include <stdlib.h>
#include "table.h"

void print_opts(const char *opts[], size_t size);
int dialog(int opts_size);
int choose_release(KeySpace *element);

static KeySpace *create_element(KeyType key, InfoType data);

int d_insert(Table *table);
int d_remove(Table *table);
int d_search(Table *table);
int d_print(Table *table);

void parse_result(int result);

#endif //DIALOG_H
