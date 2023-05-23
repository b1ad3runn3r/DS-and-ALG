#include "include/dialog.h"
#include "include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void print_opts(const char *opts[], size_t size) {
    for (int i = 0; i < size; ++i) {
        printf("%s\n", opts[i]);
    }
}

int dialog(int opts_size) {
    int choice = 0;
    int status = 0;

    do {
        status = get_int("Enter option: ", &choice);
        if (status) {
            printf("Wrong input! Try again.\n");
        }
    } while (choice < 0 || choice >= opts_size || status != 0);

    return choice;
}

int d_add_vertex(Graph *graph) {
    char *key = readline("Enter key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    if (add_vertex(graph, key)) {
        printf("Insertion error.\n");
        free(key);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int d_add_edge(Graph *graph) {
    char *key = readline("Enter start key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    char *key2 = readline("Enter end key: --> ");
    if (!key2) {
        printf("Wrong input.\n");
        free(key);
        return EXIT_FAILURE;
    }

    int8_t weight = 0;
    if (get_int8_t("Enter weight: --> ", &weight) && errno == EINVAL) {
        free(key);
        free(key2);
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    if (add_edge(graph, key, key2, weight)) {
        printf("Insertion error.\n");
        free(key);
        free(key2);
        return EXIT_FAILURE;
    }

    free(key);
    free(key2);
    return EXIT_SUCCESS;
}

int d_delete_vertex(Graph *graph) {
    char *key = readline("Enter key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    if (delete_vertex(graph, key)) {
        printf("Deletion error.\n");
        free(key);
        return EXIT_FAILURE;
    }

    free(key);
    return EXIT_SUCCESS;
}

int d_delete_edge(Graph *graph) {
    char *key = readline("Enter start key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    char *key2 = readline("Enter end key: --> ");
    if (!key2) {
        printf("Wrong input.\n");
        free(key);
        return EXIT_FAILURE;
    }

    if (delete_edge(graph, key, key2)) {
        printf("Deletion error.\n");
        free(key);
        free(key2);
        return EXIT_FAILURE;
    }

    free(key);
    free(key2);
    return EXIT_SUCCESS;
}

int d_change_vertex(Graph *graph) {
    char *key = readline("Enter key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    char *new_key = readline("Enter new key: --> ");
    if (!new_key) {
        printf("Wrong input.\n");
        free(key);
        return EXIT_FAILURE;
    }

    if (change_vertex(graph, key, new_key)) {
        printf("Change error.\n");
        free(key);
        free(new_key);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int d_change_edge(Graph *graph) {
    char *key = readline("Enter start key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    char *key2 = readline("Enter end key: --> ");
    if (!key2) {
        printf("Wrong input.\n");
        free(key);
        return EXIT_FAILURE;
    }

    int8_t weight = 0;
    if (get_int8_t("Enter weight: --> ", &weight) && errno == EINVAL) {
        free(key);
        free(key2);
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    if (change_edge(graph, key, key2, weight)) {
        printf("Change error.\n");
        free(key);
        free(key2);
        return EXIT_FAILURE;
    }

    free(key);
    free(key2);
    return EXIT_SUCCESS;
}

int d_print_as_list(Graph *graph) {
    print_as_list(graph);

    return EXIT_SUCCESS;
}

int d_print_as_matrix(Graph *graph) {
    print_as_matrix(graph);

    return EXIT_SUCCESS;
}

int d_traverse(Graph *graph) {
    char *key = readline("Enter start key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    traverse(graph, key);

    free(key);
    return EXIT_SUCCESS;
}

int d_shortest_path(Graph *graph) {
    char *key = readline("Enter start key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    char *key2 = readline("Enter end key: --> ");
    if (!key2) {
        printf("Wrong input.\n");
        free(key);
        return EXIT_FAILURE;
    }

    shortest_path(graph, key, key2);

    free(key);
    free(key2);
    return EXIT_SUCCESS;
}

int d_special_operation(Graph *graph) {
    char *key = readline("Enter start key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    special_operation(graph, key);

    free(key);
    return EXIT_SUCCESS;
}
