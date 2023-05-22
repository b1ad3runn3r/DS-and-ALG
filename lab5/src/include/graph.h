#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>

typedef char key_tt;
typedef uint64_t weight_t;

typedef struct Edge {
    key_tt *end;
    weight_t weight;
} Edge;

typedef enum {
    WHITE = 0,
    GREY = 1,
    BLACK = 2
} color_t;

typedef struct Vertex {
    key_tt *start;
    color_t color;
} Vertex;

typedef struct Graph {
    Vertex **vertices;
    uint64_t vertices_size;
} Graph;

#endif // GRAPH_H
