#ifndef MATRIX_H__
#define MATRIX_H__

#include <stddef.h>

typedef struct {
  double **elements;
  size_t rows;
  size_t cols;
} matrix_t;

void free_matrix(matrix_t matrix);

double **allocate_matrix(size_t rows, size_t cols);

matrix_t create_matrix(size_t rows, size_t cols);

#endif // #ifndef MATRIX_H__
