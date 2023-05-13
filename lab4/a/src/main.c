#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/tree.h"

int main(int argc, char **argv) {
    Tree *root = NULL;
    char *str1 = strdup("Niggers");
    char *str2 = strdup("Fuck");
    char *str3 = strdup("Fuck");
    char *str4 = strdup("Pidor");

    insert(&root, 5, str1);
    insert(&root, 3, str2);
    insert(&root, 3, str3);
    insert(&root, 1, str4);
    insert(&root, 1, str4);
    insert(&root, 7, str4);
    insert(&root, 7, str4);
    insert(&root, 8, str4);
    insert(&root, 6, str4);
    print_tree(root, 0);

    size_t min_size = 0;
    Tree **minimum = find_min(root, &min_size);
    if (!minimum) {
        printf("No min\n");
    }
    else {
        for (size_t i = 0; i < min_size; ++i) {
            print_element(minimum[i]);
            putchar('\n');
        }
        free(minimum);
    }

    size_t find_size = 0;
    Tree **to_find = find(root, 3, &find_size);
    if (!to_find) {
        printf("No such element\n");
    }
    else {
        for (size_t i = 0; i < find_size; ++i) {
            print_element(to_find[i]);
            putchar('\n');
        }
        free(to_find);
    }
    traverse(&root, ULLONG_MAX);
    putchar('\n');

    delete(&root, 3);
    delete(&root, 3);
    delete(&root, 7);
    delete(&root, 7);

    print_tree(root, 0);

    free(str1);
    free(str2);
    free(str3);
    free(str4);

    free_tree(&root);
    return 0;
}