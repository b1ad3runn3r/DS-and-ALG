#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/utils.h"
#include "include/types.h"
#include <errno.h>

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
        return E_ALLOC;
    }

    int buf = 0;
    if ((buf = strtol(line, NULL, 10)) == 0 && errno == EINVAL) {
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

    size_t buf = 0;
    if ((buf = strtoul(line, NULL, 10)) == 0 && errno == EINVAL) {
        free(line);
        return E_WRONGINPUT;
    }

    free(line);
    *res = buf;
    return E_OK;
}

int get_long(const char *prompt, long *res) {
    char *line = readline(prompt);
    if (!line) {
        return E_ALLOC;
    }

    long buf = 0;
    if ((buf = strtol(line, NULL, 10)) == 0 && errno == EINVAL) {
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
