#include "include/priority_queue.h"
#include <stdlib.h>

PriorityQueue *pq_init() {
    return calloc(1, sizeof(PriorityQueue));
}

void *first_pq(PriorityQueue *pq) {
    if (!pq) {
        return NULL;
    }

    if (!pq->head) {
        return NULL;
    }

    return pq->head->data;
}

void *pop(PriorityQueue *pq) {
    if (!pq) {
        return NULL;
    }

    if (!pq->head) {
        return NULL;
    }

    PQNode *tmp = pq->head;
    void *data = tmp->data;
    pq->head = pq->head->next;
    free(tmp);

    return data;
}

static PQNode *new_node(void *data, uint64_t priority) {
    PQNode *new = calloc(1, sizeof(PQNode));
    if (!new) {
        return NULL;
    }

    new->data = data;
    new->priority = priority;
    new->next = NULL;

    return new;
}

void push(PriorityQueue *pq, void *data, uint64_t priority) {
    PQNode *new = new_node(data, priority);
    if (!new) {
        return ;
    }

    if (!pq->head) {
        pq->head = new;
        return ;
    }

    if (pq->head->priority > priority) {
        new->next = pq->head;
        pq->head = new;
        return ;
    }

    PQNode *cur_ptr = pq->head;
    while (cur_ptr->next && cur_ptr->next->priority <= priority) {
        cur_ptr = cur_ptr->next;
    }

    new->next = cur_ptr->next;
    cur_ptr->next = new;
}

void free_pq(PriorityQueue *pq) {
    if (!pq) {
        return ;
    }

    PQNode *cur_ptr = pq->head;
    while (cur_ptr) {
        PQNode *tmp = cur_ptr;
        cur_ptr = cur_ptr->next;
        free(tmp);
    }

    free(pq);
}

int pq_is_empty(PriorityQueue *pq) {
    if (!pq) {
        return 1;
    }

    if (!pq->head) {
        return 1;
    }

    return 0;
}


