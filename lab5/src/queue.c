#include "include/queue.h"
#include <stdlib.h>

Queue *init_queue() {
    return calloc(1, sizeof(Queue));
}

static Node *new_node(void *data) {
    Node *new = calloc(1, sizeof(Node));
    if (!new) {
        return NULL;
    }

    new->data = data;
    new->next = NULL;

    return new;
}

int enqueue(Queue *queue, void *data) {
    Node *new = new_node(data);
    if (!new) {
        return EXIT_FAILURE;
    }

    if (!queue->rear) {
        queue->rear = new;
        queue->front = new;
        return EXIT_SUCCESS;
    }

    queue->rear->next = new;
    queue->rear = new;

    return EXIT_SUCCESS;
}

void *dequeue(Queue *queue) {
    if (!queue->front) {
        return NULL;
    }

    Node *tmp = queue->front;
    void *data = tmp->data;
    queue->front = queue->front->next;
    free(tmp);

    if (!queue->front) {
        queue->rear = NULL;
    }

    return data;
}

void free_queue(Queue *queue) {
    if (!queue) {
        return ;
    }

    Node *cur_ptr = queue->front;
    while (cur_ptr) {
        Node *tmp = cur_ptr;
        cur_ptr = cur_ptr->next;
        free(tmp);
    }

    free(queue);
}

void *first(Queue *queue) {
    if (!queue) {
        return NULL;
    }

    if (!queue->front) {
        return NULL;
    }

    return queue->front->data;
}
