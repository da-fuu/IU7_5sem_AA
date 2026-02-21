#include <stdio.h>

#include "errors.h"
#include "io.h"

int read_matrix(matrix_t *matrix) {
  int rows, cols;
  puts("Введите размеры: ");
  if (scanf("%d%d", &rows, &cols) != 2) {
    return ERR_DIMENSIONS;
  }

  if (rows < 1 || cols < 1) {
    return ERR_DIMENSIONS;
  }

  matrix->rows = rows;
  matrix->cols = cols;

  matrix->elements = allocate_matrix(matrix->rows, matrix->cols);
  if (!matrix->elements) {
    return ERR_NO_MEMORY;
  }

  printf("Введите %d элементов: \n", rows * cols);
  for (size_t i = 0; i < matrix->rows; i++)
    for (size_t j = 0; j < matrix->cols; j++)
      if (scanf("%lf", &matrix->elements[i][j]) != 1) {
        free_matrix(*matrix);
        return ERR_INPUT;
      }

  return OK;
}

int write_matrix(matrix_t matrix) {
  for (size_t i = 0; i < matrix.rows; i++) {
    for (size_t j = 0; j < matrix.cols; j++)
      printf("%lf ", matrix.elements[i][j]);
    printf("\n");
  }
  return OK;
}
