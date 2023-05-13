#include "include/dialog.h"
#include "include/utils.h"
#include <stdlib.h>
#include <stdio.h>

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
        if (status != E_OK) {
            printf("Wrong input! Try again.\n");
        }
    } while (choice < 0 || choice >= opts_size || status != E_OK);

    return choice;
}

int d_insert(Tree **tree) {
    size_t key;
    if (get_size_t("Enter key: -->", &key)) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    char *data = readline("Enter data: -->");
    if (!data) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    if (insert(tree, key, data)) {
        printf("Insertion error.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int d_delete(Tree **tree) {
    size_t key;
    if (get_size_t("Enter key: -->", &key)) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    if (delete(tree, key)) {
        printf("Deletion error.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int d_traverse(Tree **tree) {
    size_t key;
    if (get_size_t("If you don't want to enter key, enter -1.\nEnter key: -->", &key)) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    traverse(tree, key);

    return EXIT_SUCCESS;
}

int d_search(Tree **tree) {
    size_t key;
    if (get_size_t("Enter key: -->", &key)) {
        printf("Wrong input.\n");
        return EXIT_FAILURE;
    }

    size_t res_size = 0;
    Tree **result = find(*tree, key, &res_size);

    if (!result) {
        printf("Nothing found.\n");
        return EXIT_FAILURE;
    }
    else {
        for (size_t i = 0; i < res_size; ++i) {
            print_element(result[i]);
            putchar('\n');
        }
        free(result);
        return EXIT_SUCCESS;
    }
}

int d_search_min(Tree **tree) {
    size_t res_size = 0;
    Tree **result = find_min(*tree, &res_size);

    if (!result) {
        printf("Nothing found.\n");
        return EXIT_FAILURE;
    }
    else {
        for (size_t i = 0; i < res_size; ++i) {
            print_element(result[i]);
            putchar('\n');
        }
        free(result);
        return EXIT_SUCCESS;
    }
}

int d_print_tree(Tree **tree) {
    print_tree(*tree, 0);
    return EXIT_SUCCESS;
}

int d_load_tree(Tree **tree) {
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
