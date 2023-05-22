#ifndef DIALOG_H
#define DIALOG_H

#include "llrb_tree.h"
#include <stddef.h>

void print_opts(const char *opts[], size_t size);

int dialog(int opts_size);

int d_insert(Node **tree);
int d_delete(Node **tree);
int d_traverse(Node **tree);
int d_search(Node **tree);
int d_search_min(Node **tree);
int d_print_tree(Node **tree);
int d_load_tree(Node **tree);
int d_timing(Node **tree);

#endif // DIALOG_H
