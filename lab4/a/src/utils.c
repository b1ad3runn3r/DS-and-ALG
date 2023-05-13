#include "include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void clear_screen() {
    printf("\033[;H\033[J");
}

char *readline(const char* prompt) {
    printf("%s", prompt);
    char buf[CHUNK + 1] = {0};
    char* result = NULL;
    size_t len = 0;
    int scan_res = 0;

    do {
        scan_res = scanf("%255[^\n]", buf);
        if (scan_res < 0) {
            free(result);
            return NULL;
        }
        else if (scan_res > 0) {
            size_t chunk_len = strlen(buf);
            size_t str_len = len + chunk_len;
            char *tmp = realloc(result, sizeof(char) * (str_len + 1));
            if (!tmp) {
                break;
            }
            result = tmp;
            memcpy(result + len, buf, chunk_len);
            len = str_len;
        }
        else {
            scanf("%*c");
        }
    } while (scan_res > 0);

    if (len > 0) {
        result[len] = '\0';
    }
    else {
        result = calloc(1, sizeof(char));
    }

    return result;
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

