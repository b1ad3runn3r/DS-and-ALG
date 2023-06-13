#include "include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *readline(const char* prompt) {
    printf("%s", prompt);

    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, stdin);

    return buf;
}

int get_int(const char *prompt, int *res) {
    char *line = readline(prompt);
    if (!line) {
        return EXIT_FAILURE;
    }

    int buf = 0;
    if (sscanf(line, "%d", &buf) <= 0) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    *res = buf;
    return EXIT_SUCCESS;
}

int get_size_t(const char *prompt, size_t *res) {
    char *line = readline(prompt);
    if (!line) {
        return EXIT_FAILURE;
    }

    size_t buf = 0;
    if (sscanf(line, "%zu", &buf) <= 0) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    *res = buf;
    return EXIT_SUCCESS;
}
