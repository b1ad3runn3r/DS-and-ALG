#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdint.h>

char *readline(const char* prompt);
int get_int(const char *prompt, int *res);
int get_size_t(const char *prompt, size_t *res);
int get_int8_t(const char *prompt, int8_t *res);


#endif // UTILS_H
