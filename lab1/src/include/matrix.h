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

static int read_int(int *num, int is_idx);

int input(Matrix *matrix);

static int get_first_larger(int size, int *data);

static int get_last_smaller(int size, int *data);

static inline void swap(int *ptr1, int *ptr2);

void _free(Matrix matrix);

int task(Matrix old, Matrix *new);

#endif // MATRIX_H
