#include "include/llrb_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static inline int compare(key_tt *key1, key_tt *key2) {
    int res = strcmp(key1, key2);
    if (res < 0) {
        return -1;
    }
    else if (res == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

static inline int is_red(Node *node) {
    if (!node) {
        return 0;
    }

    return node->color == RED;
}

static inline Node *rotate_left(Node *node) {
    Node *right = node->right;
    node->right = right->left;
    right->left = node;
    right->color = node->color;
    node->color = RED;

    return right;
}

static inline Node *rotate_right(Node *node) {
    if (!node) {
        return NULL;
    }

    Node *left = node->left;
    node->left = left->right;
    left->right = node;
    left->color = node->color;
    node->color = RED;

    return left;
}

static inline void flip_color(Node *node) {
    node->color = !(node->color);
    node->left->color = !(node->left->color);
    node->right->color = !(node->right->color);
}

value_t *search(Node **root, key_tt *key, uint64_t position) {
    Node *root_ptr = *root;
    if (!root_ptr) {
        return NULL;
    }

    while (root_ptr) {
        int res = compare(key, root_ptr->key);
        switch (res) {
            case 0:
                if (position < root_ptr->value_size) {
                    return root_ptr->value + position;
                }
                else {
                    return NULL;
                }

            case -1:
                root_ptr = root_ptr->left;
                break;

            case 1:
                root_ptr = root_ptr->right;
                break;
        }
    }

    return NULL;
}

value_t *search_min(Node **root, uint64_t position) {
    Node *root_ptr = *root;
    if (!root_ptr) {
        return NULL;
    }

    while (root_ptr->left) {
        root_ptr = root_ptr->left;
    }

    if (position < root_ptr->value_size) {
        return root_ptr->value + position;
    }
    else {
        return NULL;
    }
}

static Node *__find_min(Node *node) {
    if (!node) {
        return NULL;
    }

    while (node->left) {
        node = node->left;
    }

    return node;
}

static Node *create_node(key_tt *key, value_t value) {
    Node *node = calloc(1, sizeof(Node));
    if (!node) {
        return NULL;
    }

    node->key = key;
    node->value = calloc(1, sizeof(value_t));
    if (!node->value) {
        free(node);
        return NULL;
    }

    node->value[0] = value;
    node->value_size = 1;
    node->color = RED;

    return node;
}

static int create_duplicate(Node *node, value_t value) {
    value_t *tmp_ptr = realloc(node->value, (node->value_size + 1) * sizeof(value_t));
    if (!tmp_ptr) {
        return EXIT_FAILURE;
    }

    node->value = tmp_ptr;
    node->value[node->value_size] = value;
    ++node->value_size;

    return EXIT_SUCCESS;
}

static Node *__insert(Node *node, key_tt *key, value_t value) {
    if (!node) {
        return create_node(key, value);
    }

    if (is_red(node->left) && is_red(node->right)) {
        flip_color(node);
    }

    int res = compare(key, node->key);
    switch (res) {
        case 0:
            if (create_duplicate(node, value)) {
                return NULL;
            }
            break;

        case -1:
            node->left = __insert(node->left, key, value);
            break;

        case 1:
            node->right = __insert(node->right, key, value);
            break;
    }

    if (is_red(node->right) && !is_red(node->left)) {
        node = rotate_left(node);
    }

    if (is_red(node->left) && is_red(node->left->left)) {
        node = rotate_right(node);
    }

    return node;
}

int insert(Node **root, key_tt *key, value_t value) {
    Node *tmp_ptr = __insert(*root, key, value);
    if (!tmp_ptr) {
        return EXIT_FAILURE;
    }

    tmp_ptr->color = BLACK;
    *root = tmp_ptr;

    return EXIT_SUCCESS;
}

static void free_element(Node *node) {
    if (!node->key || !node->value) {
        return ;
    }

    free(node->key);
    free(node->value);
    free(node);
}

static Node *move_red_left(Node *node) {
    flip_color(node);
    if (is_red(node->right->left)) {
        node->right = rotate_right(node->right);
        node = rotate_left(node);
        flip_color(node);
    }

    return node;
}

static Node *move_red_right(Node *node) {
    flip_color(node);
    if (is_red(node->left->left)) {
        node = rotate_right(node);
        flip_color(node);
    }

    return node;
}

static Node *balance(Node *node) {
    if (is_red(node->right)) {
        node = rotate_left(node);
    }

    if (is_red(node->left) && is_red(node->left->left)) {
        node = rotate_right(node);
    }

    if (is_red(node->left) & is_red(node->right)) {
        flip_color(node);
    }

    return node;
}

static Node *__delete_min(Node *node) {
    if (!node) {
        return NULL;
    }

    if (!node->left) {
        free_element(node);
        return EXIT_SUCCESS;
    }

    if (!is_red(node->left) && !is_red(node->left->left)) {
        node = move_red_left(node);
    }

    node->left = __delete_min(node->left);
    return balance(node);
}

static int delete_min(Node **root) {
    Node *tmp_ptr = __delete_min(*root);
    if (!tmp_ptr) {
        return EXIT_FAILURE;
    }

    tmp_ptr->color = BLACK;
    *root = tmp_ptr;

    return EXIT_SUCCESS;
}

static Node *__delete(Node *node, key_tt *key) {
    int res = compare(key, node->key);
    if (res < 0) {
        if (!is_red(node->left) && !is_red(node->left->left)) {
            node = move_red_left(node);
        }

        node->left = __delete(node->left, key);
    }
    else {
        if (is_red(node->left)) {
            node = rotate_right(node);
        }

        if (res == 0 && !node->right) {
            free_element(node);
            return NULL;
        }

        if (!is_red(node->right) && !is_red(node->right->left)) {
            node = move_red_right(node);
        }

        if (compare(key, node->key) == 0) {
            Node *tmp_ptr = __find_min(node->right);
            if (!tmp_ptr) {
                return NULL;
            }

            node->key = tmp_ptr->key;
            node->value = tmp_ptr->value;
            node->right = __delete_min(node->right);
        }
        else {
            node->right = __delete(node->right, key);
        }
    }

    return balance(node);
}

int delete(Node **root, key_tt *key) {
    Node *tmp_ptr = __delete(*root, key);
    if (!tmp_ptr) {
        return EXIT_FAILURE;
    }

    tmp_ptr->color = BLACK;
    *root = tmp_ptr;

    return EXIT_SUCCESS;
}

static void print_element(Node *node) {
    if (!node->key || !node->value) {
        return ;
    }

    for (uint64_t i = 0; i < node->value_size; ++i) {
        printf("(%s,%lu)", node->key, node->value[i]);
    }
}

void traverse(Node *node, key_tt *start, key_tt *end) {
    if (!node) {
        return ;
    }

    int res_start = compare(node->key, start);
    int res_end = compare(node->key, end);

    if (res_start >= 0 && res_end <= 0) {
        return ;
    }
    else {
        print_element(node);
        putchar(' ');

        traverse(node->left, start, end);
        traverse(node->right, start, end);
    }
}

void print_tree(Node *node, uint64_t level) {
    if (!node) {
        return ;
    }

    uint64_t cnt = level;
    print_tree(node->right, level + 1);

    while (cnt--) {
        putchar('\t');
    }

    print_element(node);
    putchar('\n');

    print_tree(node->left, level + 1);
}

void free_tree(Node **root) {
    Node *root_ptr = *root;
    if (root_ptr) {
        free_tree(&root_ptr->left);
        free_tree(&root_ptr->right);
        free_element(root_ptr);
    }

    root_ptr = NULL;
}

int load_tree(FILE *fp, Node **root) {
    if (!fp) {
        return EXIT_FAILURE;
    }

    Node *root_ptr = *root;
    if (root_ptr) {
        free_tree(root);
        root_ptr = NULL;
    }

    uint64_t cnt = 0;
    key_tt *key = NULL;
    value_t value = 0;

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        if (cnt % 2 == 0) {
            key = strdup(line);
            if (!key) {
                free(line);

                return EXIT_FAILURE;
            }
        }
        else {
            if ((value = strtoul(line, NULL, 10)) == 0 && errno == EINVAL) {
                free(key);
                free(line);

                return EXIT_FAILURE;
            }

            if (insert(root, key, value)) {
                free(key);
                free(line);

                return EXIT_FAILURE;
            }
        }

        free(line);
        line = NULL;
        cnt += 1;
    }

    free(line);
    return EXIT_SUCCESS;
}
