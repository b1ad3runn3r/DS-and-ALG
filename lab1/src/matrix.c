#include "include/matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print(Matrix matrix) {
    if (matrix.size != 0 && matrix.lines != NULL) {
        Line* cur_row = matrix.lines;
        for (int i = 0; i < matrix.size; i++) {
            for (int j = 0; j < cur_row->size; j++) {
                printf("%d ", (cur_row->data)[j]);
            }
            printf("\n");
            cur_row += 1;
        }
    }
}

static int read_int(int *num, int is_idx){
    int n = 0;
    while (n == 0) {
        n = scanf("%d", num);
        if (n < 0) return EXIT_FAILURE;
        
        else if (n == 0 || (is_idx && *num <= 0)) {
            printf("%s\n","Wrong input. Try again: ");
            scanf("%*c"); // Cleaning input buffer 
        }
    }

    return EXIT_SUCCESS;
}

int input(Matrix* matrix) {
    int n = 0, m = 0;
    printf("%s", "Enter m: ");

    if (read_int(&m, 1)) return EXIT_FAILURE;

    matrix->size = m;
    matrix->lines = (Line *)calloc(m, sizeof(Line));

    if (matrix->lines == NULL) return EXIT_FAILURE;
    Line *cur_line = matrix->lines;

    for (int i = 0; i < m; i++) {
        printf("%s", "Enter current n: ");
        
        if (read_int(&n, 1)) return EXIT_FAILURE;
        
        cur_line->size = n;
        cur_line->data = (int *)calloc(n, sizeof(int));

        if (cur_line->data == NULL) return EXIT_FAILURE;
        
        printf("%s", "Now enter desired numbers: ");
        for (int j = 0; j < n; j++) {
            if (read_int(cur_line->data + j, 0)) return EXIT_FAILURE;
        }
        cur_line += 1;
    }

    return EXIT_SUCCESS;
}

static inline int get_first_larger(int size, int* data) {
    for (int i = 0; i < size - 1; i++) {
        if (data[i + 1] > data[i]) return (i + 1);
    }

    return -1;
}

static inline int get_last_smaller(int size, int* data) {
    for (int i = size - 1; i > 1; i--) {
        if (data[i] < data[i - 1]) return i;
    }

    return -1;
}

static inline void swap(int *ptr1, int *ptr2) {
    int tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

void _free(Matrix matrix) {
    if (matrix.size != 0 && matrix.lines != NULL) {
        Line* cur_line = matrix.lines;
        for (int i = 0; i < matrix.size; i++) {
            if (cur_line->size != 0 && cur_line->data != NULL) {
                free(cur_line->data);
            }
            cur_line += 1;
        }
        free(matrix.lines);
    }

    matrix.size = 0;
    matrix.lines = NULL;
}

int task(Matrix old, Matrix* new) {
    /* Part 1. Copy old matrix to new */
    new->size = old.size;
    new->lines = (Line *)calloc(new->size, sizeof(Line));

    if (new->lines == NULL) return EXIT_FAILURE;

    Line *l_old = old.lines, *l_new = new->lines;  //old and new matrices lines pointers
    
    int l_new_size = 0;
    int *l_new_data = NULL;

    for (int i = 0; i < new->size; i++) {
        l_new_size = l_old->size;
        l_new_data = (int *)calloc(l_new_size, sizeof(int));

        if (l_new_data == NULL) return EXIT_FAILURE;

        memcpy(l_new_data, l_old->data, l_new_size * sizeof(int));
        int idx1 = get_first_larger(l_new_size, l_new_data);
        int idx2 = get_last_smaller(l_new_size, l_new_data);

        /* Part 2. Swap numbers by desired conditions */
        if (idx1 != -1 && idx2 != -1) {
            swap(l_new_data + idx1, l_new_data + idx2);
        }

        l_new->data = l_new_data;
        l_new->size = l_new_size;

        l_old += 1;
        l_new += 1;
    }

    return EXIT_SUCCESS;
}
