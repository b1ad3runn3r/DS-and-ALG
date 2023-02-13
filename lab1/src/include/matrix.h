#ifndef MATRIX_H
#define MATRIX_H

typedef struct Line {
    int     size;
    int     *data;
} Line;

typedef struct Matrix {
    int     size;
    Line    *lines;
} Matrix;

void print(Matrix matrix);

int input(Matrix *matrix);

static int get_first_smallest(int size, int *data);

static int get_last_largest(int size, int *data);

static inline void swap(int *ptr1, int *ptr2);

void _free(Matrix matrix);

void task(Matrix* matrix);

#endif // MATRIX_H