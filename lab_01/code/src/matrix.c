#include <malloc.h>

#include "matrix.h"

void free_matrix(matrix_t matrix) { free(matrix.elements); }

double **allocate_matrix(size_t rows, size_t cols) {
  double *shift;
  double **matrix = malloc((sizeof(double *) + sizeof(double) * cols) * rows);
  if (!matrix)
    return NULL;

  shift = (double *)(matrix + rows);
  for (size_t i = 0; i < rows; i++) {
    matrix[i] = shift;
    shift += cols;
  }

  return matrix;
}

matrix_t create_matrix(size_t n, size_t m) {
  matrix_t matrix;
  matrix.rows = n;
  matrix.cols = m;
  matrix.elements = allocate_matrix(n, m);
  return matrix;
}
