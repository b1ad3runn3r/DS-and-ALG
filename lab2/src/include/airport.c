#include "airport.h"
#include "queue.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Queue *parse_input(char *input, size_t *amt) {
    char *saveptr_i = NULL, *saveptr_c = NULL, *word = NULL, *client = NULL;
    char *id = NULL; size_t ta = 0, ts = 0;

    Client *current = NULL, **line = NULL, **line_tmp = NULL; // line stands for queue
    size_t line_size = 0;

    Queue *res = init();
    if (!res) return NULL;

    word = strtok_r(input, " ", &saveptr_i);
    *amt = strtoul(word, NULL, 10);

    if (!(*amt)) {
        free_queue(res, free_client);
        return NULL;
    }

    while((word = strtok_r(NULL, "\t\n ", &saveptr_i))) {
        client = word;
        if (count(client, '/') != 2) {
            free_queue(res, free_client);
            return NULL;
        }

        line_tmp = realloc(line, sizeof(Client *) * (++line_size));
        if (!line_tmp) {
            free_queue(res, free_client);
            for (size_t i = 0; i < line_size - 1; ++i) {
                free_client(line[i]);
            }
            return NULL;
        }
        line = line_tmp;

        client = strtok_r(word, "/", &saveptr_c);
        if (client) id = strdup(client);

        client = strtok_r(NULL, "/", &saveptr_c);
        if (client) ta = strtoul(client, NULL, 10);

        client = strtok_r(NULL, "/", &saveptr_c);
        if (client) ts = strtoul(client, NULL, 10);

        if (!id || ta <= 0 || ts <= 0) {
            free_queue(res, free_client);
            free(id);
            for (size_t i = 0; i < line_size; ++i) {
                free_client(line[i]);
            }
            free(line);
            return NULL;
        }

        current = calloc(1, sizeof(Client));

        if (!current) {
            free_queue(res, free_client);
            free(id);
            for (size_t i = 0; i < line_size; ++i) {
                free_client(line[i]);
            }
            free(line);
            return NULL;
        }

        current->id = id;
        current->ta = ta;
        current->ts = ts;



        line[line_size - 1] = current;

        id = NULL; ta = 0; ts = 0;
    }
    for (size_t i = 0; i < line_size; i++) {
        enqueue(res, line[i]);
    }

    free(line);

    return res;
}

void print_client(const void *c) {
    Client *cur = (Client *) c;
    printf("%s %zu %zu\n", cur->id, cur->ta, cur->ts);
}

void free_client(void *c) {
    Client *cur = (Client *)c;
    if (!cur) return;
    free(cur->id);
    free(cur);
}
