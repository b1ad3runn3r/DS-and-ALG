#include "include/list.h"
#include <stdlib.h>

List *list_init() {
    return calloc(1, sizeof(List));
}

int list_push(List *list, void *data, int (*cmp)(void *, void *)) {
    if (!list) {
        return EXIT_FAILURE;
    }

    if (!data) {
        return EXIT_FAILURE;
    }

    Node *new = calloc(1, sizeof(Node));
    if (!new) {
        return EXIT_FAILURE;
    }

    new->data = data;
    new->next = NULL;

    Node *cur_ptr = list->head;
    if (!cur_ptr) {
        list->head = new;
        ++(list->size);
        return EXIT_SUCCESS;
    }

    while (cur_ptr->next) {
        if ((*cmp)(cur_ptr->data, data) == 0) {
            free(new);
            return EXIT_FAILURE;
        }

        cur_ptr = cur_ptr->next;
    }

    cur_ptr->next = new;
    ++(list->size);

    return EXIT_SUCCESS;
}

void *list_get(List *list, void *data, int (*cmp)(void *, void *)) {
    if (!list) {
        return NULL;
    }

    if (!data) {
        return NULL;
    }

    Node *cur_ptr = list->head;
    if (!cur_ptr) {
        return NULL;
    }

    while (cur_ptr) {
        if ((*cmp)(cur_ptr->data, data) == 0) {
            return cur_ptr->data;
        }

        cur_ptr = cur_ptr->next;
    }

    return NULL;
}

int list_delete(List *list, void *data, int (*cmp)(void *, void *), void (*free_data)(void *)) {
    if (!list) {
        return EXIT_FAILURE;
    }

    if (!data) {
        return EXIT_FAILURE;
    }

    Node *cur_ptr = list->head;
    Node *prev_ptr = NULL;
    if (!cur_ptr) {
        return EXIT_FAILURE;
    }

    while (cur_ptr) {
        if ((*cmp)(cur_ptr->data, data) == 0) {
            if (!prev_ptr) {
                list->head = cur_ptr->next;
            }
            else {
                prev_ptr->next = cur_ptr->next;
            }
            (*free_data)(cur_ptr->data);
            free(cur_ptr);

            --(list->size);
            return EXIT_SUCCESS;
        }
        prev_ptr = cur_ptr;
        cur_ptr = cur_ptr->next;
    }

    return EXIT_FAILURE;
}

void list_free(List *list, void (*free_data)(void *)) {
    if (!list) {
        return ;
    }

    Node *cur_ptr = list->head;
    Node *next_ptr = NULL;
    while (cur_ptr) {
        next_ptr = cur_ptr->next;
        (*free_data)(cur_ptr->data);
        free(cur_ptr);
        cur_ptr = next_ptr;
    }

    free(list);
}
