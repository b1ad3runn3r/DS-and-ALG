#ifndef DIALOG_H
#define DIALOG_H

#include "llrb_tree.h"
#include <stddef.h>

void print_opts(const char *opts[], size_t size);

int dialog(int opts_size);

int d_insert(Node **Node);
int d_delete(Node **Node);
int d_traverse(Node **Node);
int d_search(Node **Node);
int d_search_min(Node **Node);
int d_print_tree(Node **Node);
int d_load_tree(Node **Node);

#endif // DIALOG_H
