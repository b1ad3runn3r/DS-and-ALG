#include "include/queue.h"
#include <stdlib.h>
#include <string.h>

#if defined(Q_VECTOR)

Queue *init_queue() {
    Queue *res = calloc(1, sizeof(Queue));
    if (!res) {
        return NULL;
    }

    res->data = calloc(Q_MAX, sizeof(void *));
    if (!(res->data)) {
        free(res);
        return NULL;
    }

    return res;
}

int enqueue(Queue *q, void *data) {
    if (q->front == q->rear && q->len == Q_MAX) {
        return EXIT_FAILURE;
    }
    
    void *mid_ptr = data;
    q->data[q->rear] = mid_ptr;
    q->rear += 1;
    q->rear %= Q_MAX;

    q->len += 1;

    return EXIT_SUCCESS;
}

void *dequeue(Queue *q) {
    if (q->front == q->rear && q->len == 0) {
        return NULL;
    }

    void *res = NULL;
    res = q->data[q->front];
    q->front += 1;
    q->front %= Q_MAX;
    q->len -= 1;

    return res;
}

void print_queue(const Queue *q, void (*_print)(const void *)) {
    if (!q || !q->len) {
        return;
    }

    size_t cur_idx = q->front;
    cur_idx += 1;
    cur_idx -= 1;

    while (cur_idx != q->rear) {
        (*_print)(q->data[cur_idx]);
        cur_idx = (cur_idx + 1) % Q_MAX;
    }
}

void free_queue(Queue *q, void (*_free)(void *)) {
    if (!q) {
        return;
    }


    if (q->data) {

        size_t cur_idx = q->front;
        cur_idx += 1;
        cur_idx -= 1;

        while (cur_idx != q->rear) {
            (*_free)(q->data[cur_idx]);
            cur_idx = (cur_idx + 1) % Q_MAX;
        
        }
        free(q->data);
    }

    free(q);
}

void *first(const Queue *q) {
    if (!q || !(q->len)) {
        return NULL;
    }
    return q->data[q->front];
}


#elif defined(Q_LIST)

Queue *init_queue() {
    return calloc(1, sizeof(Queue));
}

static Node *new_node() {
    return calloc(1, sizeof(Node));
}

int enqueue(Queue *q, void *data) {
    Node *tmp = new_node();
    if (!tmp) {
        return EXIT_FAILURE;
    }

    tmp->data = data;
    tmp->next = NULL;

    if (!q->rear) {
        q->front = q->rear = tmp;
        return EXIT_SUCCESS;
    }

    q->rear->next = tmp;
    q->rear = tmp;

    return EXIT_SUCCESS;
}

void *dequeue(Queue *q) {
    if (!(q->front)) {
        return NULL;
    }

    Node *tmp = q->front;
    void *ret = tmp->data;
    q->front = tmp->next;

    if (!q->front) {
        q->rear = NULL;
    }

    free(tmp);

    return ret;
}

void print_queue(const Queue *q, void (*_print)(const void *)) {
    if (!q->front) {
        return;
    }

    Node *tmp = q->front;
    while (tmp) {
        (*_print)(tmp->data);
        tmp = tmp->next;
    }
}

void free_queue(Queue *q, void (*_free)(void *)) {
    if (!q) {
        return;
    }

    while (q->front) {
        (*_free)(dequeue(q));
    }

    free(q);
}

void *first(const Queue *q) {
    if ((q->front)) {
        return q->front->data;
    }
    
    if (!(q->front)) {
        return NULL;
    }

    else {
        return NULL;
    }
}

#endif
