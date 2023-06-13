#ifndef UTILS_H
#define UTILS_H
#define CHUNK 255

#include <stdio.h>

char *readline(const char *prompt);
int get_int(const char *prompt, int *res);
int get_size_t(const char *prompt, size_t *res);
int get_long(const char *prompt, long *res);

int is_prime(int n);

int next_prime(int n);

#endif //UTILS_H
