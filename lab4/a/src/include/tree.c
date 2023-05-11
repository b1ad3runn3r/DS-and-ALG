#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int is_equal(KeyType k1, KeyType k2) {
    return k1 == k2;
}

static inline int greater_than(KeyType k1, KeyType k2) {
    return k1 >= k2;
}

static inline DataType *copy(DataType *info) {
    return strdup(info);
}

void print_element(Tree *element) {
    if (!element) {
        return ;
    }

    if (!element->info) {
        return ;
    }

    printf("(%zu, %s)", element->key, element->info);
}

void print_tree(Tree *ptr, int level) {
    int i = level;
    if (!ptr) {
        return;
    }

    print_tree(ptr->right, level + 1);
    while (i-- > 0) {
        printf("\t");
    }

    print_element(ptr);
    putchar('\n');

    print_tree(ptr->left, level + 1);
}

static void free_element(Tree *element) {
    if (!element) {
        return ;
    }

    if (!element->info) {
        return ;
    }

    free(element->info);
}

void free_tree(Tree **root) {
    Tree *root_ptr = *root;
    if (root_ptr) {
        free_tree(&(root_ptr->left));
        free_tree(&(root_ptr->right));
        free_element(root_ptr);
        free(root_ptr);
    }

    root_ptr = NULL;
}

void traverse(Tree **root, KeyType key) {
    Tree *root_ptr = *root;
    if (!root_ptr) {
        return ;
    }

    if (key == NO_KEY) {
        print_element(root_ptr);
        putchar(' ');
        traverse(&(root_ptr->left), key);
        traverse(&(root_ptr->right), key);
    }
    else {
        if (!greater_than(key, root_ptr->key) || is_equal(key, root_ptr->key)) {
            print_element(root_ptr);
            putchar(' ');
            traverse(&(root_ptr->left), key);
            traverse(&(root_ptr->right), key);
        }
    }
}

Tree** find(Tree *ptr, KeyType key, size_t *result_size) {
    Tree **result = NULL;
    size_t size = 0;

    while (ptr) {
        if (greater_than(key, ptr->key)) {
            if (is_equal(key, ptr->key)) {
                result = realloc(result, sizeof(Tree *) * (++size));
                result[size - 1] = ptr;
            }
            ptr = ptr->right;
        }
        else {
            ptr = ptr->left;
        }
    }

    *result_size = size;
    return result;
}

Tree** find_min(Tree *ptr, size_t *result_size) {
    Tree **result = NULL;
    size_t size = 0;

    if (!ptr) {
        return NULL;
    }

    while (ptr->left) {
        ptr = ptr->left;
    }

    KeyType min_key = ptr->key;
    result = realloc(result, sizeof(Tree *) * (++size));
    result[size - 1] = ptr;

    while (ptr->right) {
        if (is_equal(min_key, ptr->key)) { // ??????
            result = realloc(result, sizeof(Tree *) * (++size));
            result[size - 1] = ptr;
        }
        ptr = ptr->right;
    }

    *result_size = size;
    return result;
}

static Tree *find_last(Tree *ptr, KeyType key) {
    while (ptr) {
        if (greater_than(key, ptr->key)) {
            if (is_equal(key, ptr->key)) {
                return ptr;
            }
            ptr = ptr->right;
        }
        else {
            ptr = ptr->left;
        }
    }

    return NULL;
}

static Tree *find_last_min(Tree *ptr) {
    if (!ptr) {
        return NULL;
    }

    while (ptr->left) {
        ptr = ptr->left;
    }

    return ptr;
}

int insert(Tree **root, KeyType key, DataType *info) {
    Tree *root_ptr = *root;
    Tree *parent = NULL;

    while (root_ptr) {
        parent = root_ptr;
        if (greater_than(key, root_ptr->key)) {
            root_ptr = root_ptr->right;
        }
        else {
            root_ptr = root_ptr->left;
        }
    }

    root_ptr = calloc(1, sizeof(Tree));
    root_ptr->key = key;
    root_ptr->info = copy(info);
    root_ptr->parent = parent;

    if (!parent) {
        *root = root_ptr;
    }
    else if (!greater_than(key, parent->key)) {
        parent->left = root_ptr;
    }
    else {
        parent->right = root_ptr;
    }

    return EXIT_SUCCESS;
}

int delete(Tree **root, KeyType key) {
    Tree *to_delete = NULL;
    Tree *parent = NULL;
    Tree *ptr = NULL;
    Tree *cur = find_last(*root, key);
    if (!cur) {
        return EXIT_FAILURE;
    }

    if (!cur->left || !cur->right) {
        to_delete = cur;
    }
    else {
        to_delete = find_last_min(cur->right);
    }

    if (to_delete->left) {
        ptr = to_delete->left;
    }
    else {
        ptr = to_delete->right;
    }
    parent = to_delete->parent;

    if (ptr) {
        ptr->parent = parent;
    }

    if (!parent) {
        *root = parent;
    }
    else if (parent->left == to_delete) {
        parent->left = ptr;
    }
    else {
        parent->right = ptr;
    }

    if (to_delete != cur) {
        cur->key = to_delete->key;
    }

    free_element(to_delete);
    free(to_delete);

    return EXIT_SUCCESS;
}
