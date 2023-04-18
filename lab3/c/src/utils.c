#include "include/utils.h"
#include "include/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        return E_ALLOC;
    }

    int buf = 0;
    if (sscanf(line, "%d", &buf) <= 0) {
        free(line);
        return E_WRONGINPUT;
    }

    free(line);
    *res = buf;
    return E_OK;
}

int get_size_t(const char *prompt, size_t *res) {
    char *line = readline(prompt);
    if (!line) {
        return E_ALLOC;
    }

    int buf = 0;
    if (sscanf(line, "%d", &buf) <= 0) {
        free(line);
        return E_WRONGINPUT;
    }

    free(line);
    *res = buf;
    return E_OK;
}

int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }

    if (n <= 3) {
        return 1;
    }

    if (n % 2 == 0 || n % 3 == 0) {
        return 0;
    }

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }

    return 1;
}

int next_prime(int n) {
    if (n <= 1) {
        return 2;
    }

    int prime = n;
    int found = 0;

    while (!found) {
        if (is_prime(prime)) {
            found = 1;
        }
        else {
            prime++;
        }
    }

    return prime;
}
