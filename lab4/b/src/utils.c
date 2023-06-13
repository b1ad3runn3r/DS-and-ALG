#include "include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

char *readline(const char* prompt) {
    printf("%s", prompt);
    char *buf = NULL;
    size_t size = 0;

    if (getline(&buf, &size, stdin) == -1) {
        free(buf);
        return NULL;
    }

    return buf;
}

int get_int(const char *prompt, int *res) {
    char *line = readline(prompt);
    if (!line) {
        return EXIT_FAILURE;
    }

    int buf = 0;
    if ((buf = strtol(line, NULL, 10)) == 0 && errno == EINVAL) {
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
    if ((buf = strtoul(line, NULL, 10)) == 0 && errno == EINVAL) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    *res = buf;
    return EXIT_SUCCESS;
}
