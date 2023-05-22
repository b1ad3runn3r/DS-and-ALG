#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "list.h"

typedef char key_tt;
typedef int8_t weight_t;

#define INFINITY UINT64_MAX

struct Edge;
struct Vertex;

typedef struct Edge {
    struct Vertex *end;
    weight_t weight;
} Edge;

typedef struct Vertex {
    key_tt *key;
    bool visited;
    uint64_t distance;
    struct Vertex *previous;
    List *adjacent; // List of Edge *
} Vertex;

typedef struct Graph {
    Vertex **vertices;
    uint64_t vertices_size;
} Graph;

Graph *init_graph();
void free_graph(Graph *graph);

int add_vertex(Graph *graph, key_tt *start);
int add_edge(Graph *graph, key_tt *start, key_tt *end, weight_t weight);

int delete_vertex(Graph *graph, key_tt *start);
int delete_edge(Graph *graph, key_tt *start, key_tt *end);

int change_vertex(Graph *graph, key_tt *start, key_tt *new_start);
int change_edge(Graph *graph, key_tt *start, key_tt *end, weight_t new_weight);

void print_as_list(Graph *graph);
void print_as_matrix(Graph *graph);

void traverse(Graph *graph, key_tt *source);
void shortest_path(Graph *graph, key_tt *start, key_tt *end);
void special_operation(Graph *graph, key_tt *target);

#endif // GRAPH_H
