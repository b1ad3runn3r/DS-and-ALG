#include "airport.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count(const char *str, char search) {
    size_t cnt = 0;
    const size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        if (str[i] == search) ++cnt;
    }

    return cnt;
}

char *buffered_input() {
    char buffer[BUFFER] = {0};
    size_t res_size = 1; // it's for \0

    char *res = calloc(BUFFER, sizeof(char));
    if (!res) return NULL;

    while(fgets(buffer, BUFFER, stdin)) {
        res_size += strlen(buffer);

        res = realloc(res, res_size * sizeof(char));
        if (!res) {
            return NULL;
        }

        strcat(res, buffer);
    }

    return res;
}

Queue *parse_input(char *input, size_t *amt) {
    char *saveptr_i = NULL, *saveptr_c = NULL, *word = NULL, *client = NULL;
    char *id = NULL; size_t ta = 0, ts = 0;

    Client *current = NULL, **line = NULL, **line_tmp = NULL; // line stands for queue
    size_t line_size = 0;

    Queue *res = init_queue();
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

Airport *init_airport(size_t size) {
    Airport *airport = calloc(1, sizeof(Airport));
    if (!airport) return NULL;

    airport->size = size;
    airport->receptions = calloc(size, sizeof(Reception));
    if (!airport->receptions) {
        free_airport(airport);
        return NULL;
    }

    Reception *ptr = airport->receptions;
    while (ptr < ptr + size) {
        ptr->queue = init_queue();
        if (!ptr) {
            free_airport(airport);
            return NULL;
        }
        ++ptr;
    }

    return airport;
}

void print_airport(Airport *airport) {
    return ; //TODO: implement later
}

void free_airport(Airport *airport) {
    if (airport) {
        if (airport->receptions) {
            for (size_t i = 0; i < airport->size; ++i) {
                free_queue((airport->receptions + i)->queue, free_client);
            }
            free(airport->receptions);
        }
        free(airport);
    }
}

void print_client(const void *c) {
    Client *cur = (Client *) c;
    printf("%s %zu %zu", cur->id, cur->ta, cur->ts);
}

void free_client(void *c) {
    Client *cur = (Client *)c;
    if (!cur) return;
    free(cur->id);
    free(cur);
}
