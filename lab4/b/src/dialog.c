#include "include/dialog.h"
#include "include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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

int d_insert(Node **tree) {
    char *key = readline("Enter data: -->");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    uint64_t data = 0;
    if (get_size_t("Enter key: -->", &data)) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    if (insert(tree, key, data)) {
        printf("Insertion error.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int d_delete(Node **tree) {
    char *key = readline("Enter data: -->");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    uint64_t data = 0;
    if (get_size_t("Enter key: -->", &data)) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int d_traverse(Node **tree) {
    char *key = readline("Enter key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    char *start = readline("Enter start key: --> ");
    if (!start) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    char *end = readline("Enter end key: --> ");
    if (!end) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    traverse(*tree, start, end);

    return EXIT_SUCCESS;
}

int d_search(Node **tree) {
    char *key = readline("Enter key: --> ");
    if (!key) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    uint64_t position = 0;
    if (get_size_t("Enter position (index): -->", &position)) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    value_t *result = search(tree, key, position);

    if (!result) {
        printf("Nothing found.\n");
        return EXIT_FAILURE;
    }
    else {
        printf("Found value: %lu", *result);
    }

    return EXIT_SUCCESS;
}

int d_search_min(Node **tree) {
    uint64_t position = 0;
    if (get_size_t("Enter position (index): -->", &position)) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    value_t *result = search_min(tree, position);

    if (!result) {
        printf("Nothing found.\n");
        return EXIT_FAILURE;
    }
    else {
        printf("Found value: %lu", *result);
    }

    return EXIT_SUCCESS;
}

int d_print_tree(Node **tree) {
    print_tree(*tree, 0);
    return EXIT_SUCCESS;
}

int d_load_tree(Node **tree) {
    if (*tree) {
        char choice;
        printf("The tree is not empty.\nDo you want to override it? ");
        printf("[y]es/[n]o: ");
        scanf("%c", &choice);
        switch (choice) {
            case 'y':
                break;
            case 'n':
                printf("Aborting...\n");
                return EXIT_SUCCESS;
            default:
                printf("Unknown option. Aborting...\n");
                return EXIT_FAILURE;
        }
    }

    char *path = readline("Enter path to file: -->");
    if (!path) {
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(path, "r");
    if (!fp) {
        printf("File does not exist.\n");
        free(path);
        return EXIT_FAILURE;
    }

    if (load_tree(fp, tree)) {
        printf("Wrong input.\n");
        fclose(fp);
        free(path);
        return EXIT_FAILURE;
    }

    fclose(fp);
    free(path);
    return EXIT_SUCCESS;
}
