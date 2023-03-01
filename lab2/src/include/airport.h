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

void _print(const void *c);

void _free(void *);

#endif //AIRPORT_H
