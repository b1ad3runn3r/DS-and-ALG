#include "include/graph.h"
#include "include/list.h"
#include "include/queue.h"
#include "include/priority_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

static inline int compare_keys(key_tt *key1, key_tt *key2) {
    return strcmp(key1, key2);
}

static inline int compare_edges(void *p1, void *p2) {
    return compare_keys(((Edge *)p1)->end->key, ((Edge *)p2)->end->key);
}

static void free_edge(void *p1){
    if (!p1) {
        return ;
    }

    free(p1);
}

Graph *init_graph() {
    return calloc(1, sizeof(Graph));
}

static void free_vertex(Vertex *vertex) {
    if (!vertex) {
        return ;
    }


    if (vertex->adjacent) {
        list_free(vertex->adjacent, free_edge);
    }
    vertex->adjacent = NULL;
    free(vertex->key);

    vertex->key = NULL;
    free(vertex);
}

void free_graph(Graph *graph) {
    if (!graph) {
        return ;
    }

    for (uint64_t i = 0; i < graph->vertices_size; i++) {
        free_vertex(graph->vertices[i]);
    }

    free(graph->vertices);
    free(graph);
}

int add_vertex(Graph *graph, key_tt *start) {
    if (!graph) {
        return EXIT_FAILURE;
    }

    if (!start) {
        return EXIT_FAILURE;
    }

    Vertex *new = calloc(1, sizeof(Vertex));
    if (!new) {
        return EXIT_FAILURE;
    }

    new->key = start;
    new->visited = false;
    new->adjacent = list_init();

    if (!new->adjacent) {
        free_vertex(new);
        return EXIT_FAILURE;
    }

    Vertex **new_vertices = realloc(graph->vertices, (graph->vertices_size + 1) * sizeof(Vertex *));
    if (!new_vertices) {
        free_vertex(new);
        return EXIT_FAILURE;
    }

    graph->vertices = new_vertices;
    graph->vertices[graph->vertices_size] = new;
    ++(graph->vertices_size);

    return EXIT_SUCCESS;
}

int add_edge(Graph *graph, key_tt *start, key_tt *end, weight_t weight) {
    if (!graph) {
        return EXIT_FAILURE;
    }

    if (!start || !end) {
        return EXIT_FAILURE;
    }

    Vertex *start_vertex = NULL;
    Vertex *end_vertex = NULL;
    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        if (!compare_keys(start, graph->vertices[i]->key)) {
            start_vertex = graph->vertices[i];
            if (end_vertex) {
                break;
            }
        }

        if (!compare_keys(end, graph->vertices[i]->key)) {
            end_vertex = graph->vertices[i];
            if (start_vertex) {
                break;
            }
        }
    }

    if (!start_vertex || !end_vertex) {
        return EXIT_FAILURE;
    }

    Edge *new = calloc(1, sizeof(Edge));
    if (!new) {
        return EXIT_FAILURE;
    }

    new->end = end_vertex;
    new->weight = weight;

    if (list_push(start_vertex->adjacent, new, compare_edges)) {
        free(new);
        return EXIT_FAILURE;
    }

    Edge *new_rev = calloc(1, sizeof(Edge));
    if (!new_rev) {
        return EXIT_FAILURE;
    }

    new_rev->end = start_vertex;
    new_rev->weight = weight;

    if (list_push(end_vertex->adjacent, new_rev, compare_edges)) {
        free(new_rev);
        free(new);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int delete_vertex(Graph *graph, key_tt *start) {
    if (!graph) {
        return EXIT_FAILURE;
    }

    if (!start) {
        return EXIT_FAILURE;
    }

    Vertex *vertex = NULL;
    uint64_t idx = 0;
    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        if (!compare_keys(start, graph->vertices[i]->key)) {
            vertex = graph->vertices[i];
            idx = i;
            break;
        }
    }

    if (!vertex) {
        return EXIT_FAILURE;
    }

    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        List *adjacent = graph->vertices[i]->adjacent;

        Edge edge_1 = { .end = vertex };
        Edge edge_2 = { .end = graph->vertices[i]};

        list_delete(adjacent, &edge_1, compare_edges, free_edge);
        list_delete(vertex->adjacent, &edge_2, compare_edges, free_edge);

    }

    if (graph->vertices_size > 1 && idx < graph->vertices_size - 1) {
        memmove(graph->vertices + idx, graph->vertices + idx + 1, (graph->vertices_size - idx - 1) * sizeof(Vertex *));
    }

    free_vertex(vertex);

    Vertex **new_vertices = realloc(graph->vertices, (graph->vertices_size - 1) * sizeof(Vertex *));
    if (!new_vertices) {
        return EXIT_FAILURE;
    }

    graph->vertices = new_vertices;
    --(graph->vertices_size);

    return EXIT_SUCCESS;
}

int delete_edge(Graph *graph, key_tt *start, key_tt *end) {
    if (!graph) {
        return EXIT_FAILURE;
    }

    if (!start || !end) {
        return EXIT_FAILURE;
    }

    Vertex *start_vertex = NULL;
    Vertex *end_vertex = NULL;
    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        if (!compare_keys(start, graph->vertices[i]->key)) {
            start_vertex = graph->vertices[i];
            if (end_vertex) {
                break;
            }
        }

        if (!compare_keys(end, graph->vertices[i]->key)) {
            end_vertex = graph->vertices[i];
            if (start_vertex) {
                break;
            }
        }
    }

    if (!start_vertex || !end_vertex) {
        return EXIT_FAILURE;
    }

    List *start_list = start_vertex->adjacent;
    List *end_list = end_vertex->adjacent;

    Edge start_edge = { .end = end_vertex };
    Edge end_edge = { .end = start_vertex };

    list_delete(start_list, &start_edge, compare_edges, free_edge);
    list_delete(end_list, &end_edge, compare_edges, free_edge);

    return EXIT_SUCCESS;
}

int change_vertex(Graph *graph, key_tt *start, key_tt *new_start) {
    if (!graph) {
        return EXIT_FAILURE;
    }

    if (!start || !new_start) {
        return EXIT_FAILURE;
    }

    Vertex *vertex = NULL;
    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        if (!compare_keys(start, graph->vertices[i]->key)) {
            vertex = graph->vertices[i];
            break;
        }
    }

    if (!vertex) {
        return EXIT_FAILURE;
    }

    free(vertex->key);
    vertex->key = new_start;

    return EXIT_SUCCESS;
}

int change_edge(Graph *graph, key_tt *start, key_tt *end, weight_t new_weight) {
    if (!graph) {
        return EXIT_FAILURE;
    }

    if (!start || !end) {
        return EXIT_FAILURE;
    }

    Vertex *start_vertex = NULL;
    Vertex *end_vertex = NULL;

    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        if (!compare_keys(start, graph->vertices[i]->key)) {
            start_vertex = graph->vertices[i];
            if (end_vertex) {
                break;
            }
        }

        if (!compare_keys(end, graph->vertices[i]->key)) {
            end_vertex = graph->vertices[i];
            if (start_vertex) {
                break;
            }
        }
    }

    if (!start_vertex || !end_vertex) {
        return EXIT_FAILURE;
    }

    List *start_list = start_vertex->adjacent;
    List *end_list = end_vertex->adjacent;

    Edge start_edge = { .end = end_vertex, .weight = 0 };
    Edge end_edge = { .end = start_vertex, .weight = 0 };

    Edge *start_edge_2 = list_get(start_list, &start_edge, compare_edges);
    Edge *end_edge_2 = list_get(end_list, &end_edge, compare_edges);

    if (!start_edge_2 || !end_edge_2) {
        return EXIT_FAILURE;
    }

    start_edge_2->weight = new_weight;
    end_edge_2->weight = new_weight;

    return EXIT_SUCCESS;
}

void print_as_list(Graph *graph) {
    if (!graph) {
        return ;
    }

    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        printf("%s: ", graph->vertices[i]->key);
        List *cur_list = graph->vertices[i]->adjacent;
        Node *cur_node = cur_list->head;

        while (cur_node) {
            printf("%s(%d) -> ", ((Edge *)cur_node->data)->end->key, ((Edge *)cur_node->data)->weight);
            cur_node = cur_node->next;
        }
        putchar('\n');
    }
}

void print_as_matrix(Graph *graph) {
    if (!graph) {
        return ;
    }

    putchar(' ');
    putchar(' ');
    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        printf("%c ", graph->vertices[i]->key[0]);
    }

    putchar('\n');

    Vertex *start = NULL;
    Vertex *end = NULL;

    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        start = graph->vertices[i];
        printf("%c ", graph->vertices[i]->key[0]);

        for (uint64_t j = 0; j < graph->vertices_size; ++j) {
            end = graph->vertices[j];

            Edge to_search = { .end = end, .weight = 0 };

            List *cur_list = start->adjacent;
            if (list_get(cur_list, &to_search, compare_edges)) {
                printf("1 ");
            }
            else {
                printf("0 ");
            }
        }

        putchar('\n');
    }
}

static Vertex **bfs(Graph *graph, Vertex *source_vertex, uint64_t *size) {
    Vertex **friends = NULL;
    uint64_t friends_size = 0;

    Queue *queue = init_queue();
    enqueue(queue, source_vertex);
    source_vertex->visited = true;

    Edge *adj_edge = NULL;
    Vertex *cur_vertex = NULL;

    while (first(queue)) {
        cur_vertex = dequeue(queue);
        Node *cur_list = cur_vertex->adjacent->head;

        while (cur_list) {
            adj_edge = (Edge *)(cur_list->data);
            if (!adj_edge->end->visited && adj_edge->weight > 0) {
                adj_edge->end->visited = true;

                Vertex **new_friends = realloc(friends, sizeof(Vertex *) * (friends_size + 1));
                if (!new_friends) {
                    return NULL;
                }

                friends = new_friends;
                friends[friends_size++] = adj_edge->end;

                enqueue(queue, adj_edge->end);
            }

            cur_list = cur_list->next;
        }
    }
    free_queue(queue);

    *size = friends_size;
    return friends;
}

void traverse(Graph *graph, key_tt *source) {
    if (!graph || !source) {
        return ;
    }

    Vertex *source_vertex = NULL;
    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        graph->vertices[i]->visited = false;
        if (!compare_keys(source, graph->vertices[i]->key)) {
            source_vertex = graph->vertices[i];
        }
    }

    if (!source_vertex) {
        return ;
    }

    uint64_t friends_size = 0;
    Vertex **friends = bfs(graph, source_vertex, &friends_size);
    if (!friends) {
        return ;
    }

    printf("Friends of %s: \n", source_vertex->key);
    for (uint64_t i = 0; i < friends_size; ++i) {
        printf("%s, ", friends[i]->key);
    }
    putchar('\n');

    free(friends);
}

static void dijkstra(Graph *graph, Vertex *source_vertex) {
    source_vertex->distance = 0;

    PriorityQueue *queue = pq_init();
    push(queue, source_vertex, source_vertex->distance);

    Vertex *cur_vertex = NULL;
    Edge *cur_edge = NULL;

    while (first_pq(queue)) {
        cur_vertex = pop(queue);
        cur_vertex->visited = true;

        Node *cur_list = cur_vertex->adjacent->head;
        while (cur_list) {
            cur_edge = (Edge *)(cur_list->data);
            if (!cur_edge->end->visited) {
                uint64_t new_distance = cur_vertex->distance + cur_edge->weight;
                if (new_distance < cur_edge->end->distance) {
                    cur_edge->end->distance = new_distance;
                    cur_edge->end->previous = cur_vertex;
                }

                push(queue, cur_edge->end, cur_edge->end->distance);
            }

            cur_list = cur_list->next;
        }
    }
    free_pq(queue);
}

static Vertex **print_path(Graph *graph, Vertex *source_vertex, Vertex *target_vertex, uint64_t *size) {
    Vertex **path = NULL;
    uint64_t path_size = 0;

    for (Vertex *v = target_vertex; v != source_vertex; v = v->previous) {
        Vertex **new_path = realloc(path, sizeof(Vertex *) * (path_size + 1));
        if (!new_path) {
            return NULL;
        }

        path = new_path;
        path[path_size++] = v;
    }

    Vertex **new_path = realloc(path, sizeof(Vertex *) * (path_size + 1));
    if (!new_path) {
        return NULL;
    }

    path = new_path;
    path[path_size++] = source_vertex;

    for (uint64_t i = 0; i < path_size / 2; ++i) {
        Vertex *tmp = path[i];
        path[i] = path[path_size - i - 1];
        path[path_size - i - 1] = tmp;
    }

    *size = path_size;
    return path;
}

void shortest_path(Graph *graph, key_tt *start, key_tt *end) {
    if (!graph || !start || !end) {
        return ;
    }

    if (!compare_keys(start, end)) {
        return ;
    }

    Vertex *start_vertex = NULL;
    Vertex *end_vertex = NULL;

    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        graph->vertices[i]->visited = false;
        graph->vertices[i]->distance = INFINITY;
        graph->vertices[i]->previous = NULL;

        if (!compare_keys(start, graph->vertices[i]->key)) {
            start_vertex = graph->vertices[i];
        }

        if (!compare_keys(end, graph->vertices[i]->key)) {
            end_vertex = graph->vertices[i];
        }
    }

    dijkstra(graph, start_vertex);

    uint64_t path_size = 0;
    Vertex **path = print_path(graph, start_vertex, end_vertex, &path_size);

    printf("Shortest path from %s to %s: \n", start, end);
    for (uint64_t i = 0; i < path_size; ++i) {
        printf("%s -> ", path[i]->key);
    }
    putchar('\n');

    free(path);
}

static uint64_t **generate_matrix(Graph *graph) {
    uint64_t **matrix = calloc(graph->vertices_size, sizeof(uint64_t *));
    if (!matrix) {
        return NULL;
    }

    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        matrix[i] = calloc(graph->vertices_size, sizeof(uint64_t));
        if (!matrix[i]) {
            for (uint64_t j = 0; j < i; ++j) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }

        Vertex *start = graph->vertices[i];
        Node *cur_ptr = start->adjacent->head;

        for (uint64_t j = 0; j < graph->vertices_size; ++j) {
            Vertex *end = graph->vertices[j];
            Edge query = {.end = end, .weight = 0 };

            Edge *result = list_get(start->adjacent, &query, compare_edges);
            if (!result) {
                matrix[i][j] = 0;
            }
            else {
                matrix[i][j] = result->weight;
            }
        }
    }

    return matrix;
}

static uint64_t **floyd_warshall(Graph *graph) {
    uint64_t **matrix = generate_matrix(graph);
    if (!matrix) {
        return NULL;
    }

    for (uint64_t k = 0; k < graph->vertices_size; ++k) {
        for (uint64_t i = 0; i < graph->vertices_size; ++i) {
            for (uint64_t j = 0; j < graph->vertices_size; ++j) {
                if (matrix[i][k] + matrix[k][j] > matrix[i][j]) {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                }
            }
        }
    }

    return matrix;
}

void special_operation(Graph *graph, key_tt *target) {
    if (!graph || !target) {
        return ;
    }

    uint64_t **matrix = floyd_warshall(graph);
    if (!matrix) {
        return ;
    }

    uint64_t max_distance = 0;
    uint64_t save_idx_end = 0;

    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        if (!compare_keys(target, graph->vertices[i]->key)) {
            for (uint64_t j = 0; j < graph->vertices_size; ++j) {
                if (matrix[i][j] > max_distance) {
                    max_distance = matrix[i][j];
                    save_idx_end = j;
                }
            }

            break;
        }
    }

    printf("Maximal weight path:\n%s -> %s\n", target, graph->vertices[save_idx_end]->key);

    for (uint64_t i = 0; i < graph->vertices_size; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}
