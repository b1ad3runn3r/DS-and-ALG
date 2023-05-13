#ifndef DIALOG_H
#define DIALOG_H

#include "tree.h"
#include <stdlib.h>

void print_opts(const char *opts[], size_t size);

int dialog(int opts_size);

int d_insert(Tree **tree);
int d_delete(Tree **tree);
int d_traverse(Tree **tree);
int d_search(Tree **tree);
int d_search_min(Tree **tree);
int d_print_tree(Tree **tree);
int d_load_tree(Tree **tree);

#endif // DIALOG_H
