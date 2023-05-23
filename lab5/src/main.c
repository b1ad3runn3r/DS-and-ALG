#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/graph.h"
#include "include/dialog.h"

const char *opts[] = {
        "0. Quit",
        "1. Add Vertex",
        "2. Add edge",
        "3. Delete vertex",
        "4. Delete edge",
        "5. Change vertex",
        "6. Change edge",
        "7. Print as list",
        "8. Print as matrix",
        "9. Traverse",
        "10. Shortest path",
        "11. Special operation"
};

int (*const f_opts[])(Graph *) = {
        NULL,
        d_add_vertex,
        d_add_edge,
        d_delete_vertex,
        d_delete_edge,
        d_change_vertex,
        d_change_edge,
        d_print_as_list,
        d_print_as_matrix,
        d_traverse,
        d_shortest_path,
        d_special_operation
};

int n_opts = 12;

int main(int argc, char **argv) {
    Graph *graph = init_graph();
    if (!graph) {
        printf("Graph initialization error.\n");
        return EXIT_FAILURE;
    }

    print_opts(opts, n_opts);
    putchar('\n');
    int choice = 0;
    int f_res = 0;

    while ((choice = dialog(n_opts))) {
        if (f_opts[choice]) {
            f_res = f_opts[choice](graph);
        }
        else {
            break;
        }

        print_opts(opts, n_opts);
        putchar('\n');
    }

    free_graph(graph);
    return 0;
}