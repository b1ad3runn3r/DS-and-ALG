#ifndef UTILS_H
#define UTILS_H
#define CHUNK 255

#include <stdio.h>

void clear_screen();
char *readline(const char *prompt);
int get_int(const char *prompt, int *res);
int get_size_t(const char *prompt, size_t *res);

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
        prime++;

        if (is_prime(prime)) {
            found = 1;
        }
    }

    return prime;
}

#endif //UTILS_H
