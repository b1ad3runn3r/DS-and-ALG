#include "airport.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline size_t count(const char *str, char search) {
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
        char *tmp = (char *)realloc(res, res_size * sizeof(char));
        if (!tmp) {
            free(res);
            return NULL;
        }
        res = tmp;

        res[res_size - 1] = '\0';
        strcat(res, buffer);
    }

    return res;
}

static inline int compare(const void *p1, const void *p2) {
    size_t t1 = (*((Client **)p1))->ta;
    size_t t2 = (*((Client **)p2))->ta;

    if (t1 < t2) return -1;
    else if (t1 > t2) return 1;
    else return 0;
}

Queue *parse_input(char *input, size_t *amt) {
    if (!strlen(input)) return NULL;
    char *saveptr_i = NULL, *saveptr_c = NULL, *word = NULL, *client = NULL;
    char *id = NULL; size_t ta = 0, ts = 0;

    Client *current = NULL, **line = NULL, **line_tmp = NULL; // line stands for queue
    size_t line_size = 0;

    Queue *res = init_queue();
    if (!res) return NULL;

    word = strtok_r(input, " ", &saveptr_i);
    if (!word) { //fuzzer error, didn't think about it
        free_queue(res, free_client);
        return NULL;
    }
    *amt = strtoul(word, NULL, 10);

    if (!(*amt)) {
        free_queue(res, free_client);
        return NULL;
    }

    while((word = strtok_r(NULL, "\t\n ", &saveptr_i))) {
        client = word;
        if (count(client, '/') != 2) {
            for (size_t i = 0; i < line_size; ++i) {
                free_client(line[i]);
            }
            free(line);
            free_queue(res, free_client);
            return NULL;
        }

        ++line_size;
        line_tmp = realloc(line, sizeof(Client *) * line_size);
        if (!line_tmp) {
            for (size_t i = 0; i < line_size - 1; ++i) {
                free_client(line[i]);
            }
            free(line);
            free_queue(res, free_client);
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
            free(id);
            for (size_t i = 0; i < line_size; ++i) {
                free_client(line[i]);
            }
            free(line);
            free_queue(res, free_client);
            return NULL;
        }

        current = calloc(1, sizeof(Client));

        if (!current) {
            free(id);
            for (size_t i = 0; i < line_size; ++i) {
                free_client(line[i]);
            }
            free(line);
            free_queue(res, free_client);
            return NULL;
        }

        current->id = id;
        current->ta = ta;
        current->ts = ts;

        line[line_size - 1] = current;
        id = NULL; ta = 0; ts = 0;
    }
    qsort(line, line_size, sizeof(Client *), compare);

    for (size_t i = 0; i < line_size; ++i) {
        if (enqueue(res, line[i])) {
            printf("Queue overflow detected. Maximum allowed is %d.\n", Q_MAX);
            free_client(line[i]);
        }
    }

    free(line);
    return res;
}

Airport *init_airport(size_t size) {
    if (size > Q_MAX) {
        printf("Queue overflow detected. Maximum allowed is %d.\n", Q_MAX);
        size = Q_MAX;
    }
    Airport *airport = calloc(1, sizeof(Airport));
    if (!airport) return NULL;

    airport->size = size;
    airport->receptions = calloc(size, sizeof(Reception));
    if (!airport->receptions) {
        free_airport(airport);
        return NULL;
    }

    for (size_t i = 0; i < size; ++i) {
        (airport->receptions + i)->queue = init_queue();
    }

    return airport;
}

void print_airport(const Airport *airport) {
    if (!airport || !airport->receptions) return;
    for (size_t i = 0; i < airport->size; i++) {
        printf("Rec. %zu: ", i + 1);
        print_queue((airport->receptions + i)->queue, print_client);
        putchar('\n');
    }
    putchar('\n');
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
    printf("%s ", cur->id);
}

void free_client(void *c) {
    Client *cur = (Client *)c;
    if (!cur) return;
    free(cur->id);
    free(cur);
}

int random_choice(Airport *airport, Queue *crowd) {
    if (!airport) {
        return EXIT_FAILURE;
    }
    size_t cur_time = 1;
    int rec_idx = 0, changed = 0, is_empty = 0;

    Client *cur = first(crowd);
    if (!cur) {
        return EXIT_FAILURE;
    }

    Queue *tmp_queue = NULL;
    Client *tmp_client = NULL;
    while (!is_empty) {
        is_empty = 1;
        if (cur) {
            while (cur->ta == cur_time) {
                if (!first(crowd)) {
                    break;
                }
                changed = 1;
                rec_idx = rand() % airport->size;
                enqueue((airport->receptions + rec_idx)->queue, dequeue(crowd));
                cur = first(crowd);
                if (!cur) {
                    break;
                }
            }
        }

        for (size_t i = 0; i < airport->size; ++i) {
            tmp_queue = (airport->receptions + i)->queue;
            tmp_client = first(tmp_queue);
            if (!tmp_client) {
                is_empty &= 1;
                continue;
            }

            is_empty &= 0;
            if (cur_time == tmp_client->ta + tmp_client->ts) {
                free_client(dequeue(tmp_queue));
                tmp_client = first(tmp_queue);
                if (tmp_client) {
                    tmp_client->ta = cur_time;
                }
                changed = 1;
            }
        }

        if (changed) {
            printf("Time: %zu\n", cur_time);
            print_airport(airport);
        }

        changed = 0;
        ++cur_time;
    }

    return EXIT_SUCCESS;
}
