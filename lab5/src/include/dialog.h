#ifndef DIALOG_H
#define DIALOG_H

#include "graph.h"
#include <stddef.h>

void print_opts(const char *opts[], size_t size);

int dialog(int opts_size);

int d_add_vertex(Graph *graph);
int d_add_edge(Graph *graph);

int d_delete_vertex(Graph *graph);
int d_delete_edge(Graph *graph);

int d_change_vertex(Graph *graph);
int d_change_edge(Graph *graph);

int d_print_as_list(Graph *graph);
int d_print_as_matrix(Graph *graph);

int d_traverse(Graph *graph);
int d_shortest_path(Graph *graph);
int d_special_operation(Graph *graph);

#endif // DIALOG_H
