#ifndef AIRPORT_H
#define AIRPORT_H

#include <stdlib.h>
#include "queue.h"

typedef struct Client {
    char *id;
    size_t ta;
    size_t ts;
} Client;

Queue *parse_input(char *input, size_t *amt);

void print_client(const void *c);

void free_client(void *);

#endif //AIRPORT_H
