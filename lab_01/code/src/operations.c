#include <malloc.h>
#include <math.h>

#include "errors.h"
#include "operations.h"

int prepare_out(matrix_t matrix_in_1, matrix_t matrix_in_2,
                matrix_t *matrix_out) {
  if (matrix_in_1.cols != matrix_in_2.rows)
    return ERR_DIMENSIONS_MULTIPLY;

  matrix_out->rows = matrix_in_1.rows;
  matrix_out->cols = matrix_in_2.cols;
  matrix_out->elements = allocate_matrix(matrix_out->rows, matrix_out->cols);
  if (!matrix_out->elements)
    return ERR_NO_MEMORY;

  return OK;
}

int multiply_std(matrix_t matrix_in_1, matrix_t matrix_in_2,
                 matrix_t matrix_out) {
  if (matrix_in_1.cols != matrix_in_2.rows)
    return ERR_DIMENSIONS_MULTIPLY;

  for (size_t i = 0; i < matrix_out.rows; i++)
    for (size_t j = 0; j < matrix_out.cols; j++) {
      matrix_out.elements[i][j] = 0;
      for (size_t k = 0; k < matrix_in_1.cols; k++)
        matrix_out.elements[i][j] =
            matrix_out.elements[i][j] +
            matrix_in_1.elements[i][k] * matrix_in_2.elements[k][j];
    }
  return OK;
}

int multiply_vinograd(matrix_t matrix_in_1, matrix_t matrix_in_2,
                      matrix_t matrix_out) {
  size_t M = matrix_out.rows;
  size_t N = matrix_in_1.cols;
  size_t K = matrix_out.cols;

  if (matrix_in_1.cols != matrix_in_2.rows)
    return ERR_DIMENSIONS_MULTIPLY;

  double *row_array = malloc(sizeof(double) * M);
  if (row_array == NULL)
    return ERR_NO_MEMORY;

  double *col_array = malloc(sizeof(double) * K);
  if (col_array == NULL)
    return ERR_NO_MEMORY;

  for (size_t i = 0; i < M; i++) {
    row_array[i] = 0;
    for (size_t j = 0; j < N / 2; j++)
      row_array[i] = row_array[i] + matrix_in_1.elements[i][2 * j] *
                                        matrix_in_1.elements[i][2 * j + 1];
  }

  for (size_t i = 0; i < K; i++) {
    col_array[i] = 0;
    for (size_t j = 0; j < N / 2; j++)
      col_array[i] = col_array[i] + matrix_in_2.elements[2 * j][i] *
                                        matrix_in_2.elements[2 * j + 1][i];
  }

  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < K; j++) {
      matrix_out.elements[i][j] = -row_array[i] - col_array[j];
      for (size_t k = 0; k < N / 2; k++) {
        matrix_out.elements[i][j] = matrix_out.elements[i][j] +
                                    (matrix_in_1.elements[i][2 * k] +
                                     matrix_in_2.elements[2 * k + 1][j]) *
                                        (matrix_in_1.elements[i][2 * k + 1] +
                                         matrix_in_2.elements[2 * k][j]);
      }
    }
  }

  if (N % 2 == 1) {
    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < K; j++)
        matrix_out.elements[i][j] =
            matrix_out.elements[i][j] +
            matrix_in_1.elements[i][N - 1] * matrix_in_2.elements[N - 1][j];
  }

  free(row_array);
  free(col_array);
  return OK;
}

int multiply_vinograd_opt(matrix_t matrix_in_1, matrix_t matrix_in_2,
                          matrix_t matrix_out) {
  size_t M = matrix_out.rows;
  size_t N = matrix_in_1.cols;
  size_t K = matrix_out.cols;

  if (matrix_in_1.cols != matrix_in_2.rows)
    return ERR_DIMENSIONS_MULTIPLY;

  double *row_array = malloc(sizeof(double) * M);
  if (row_array == NULL)
    return ERR_NO_MEMORY;

  double *col_array = malloc(sizeof(double) * K);
  if (col_array == NULL)
    return ERR_NO_MEMORY;

  size_t half_N = N / 2;

  for (size_t i = 0; i < M; i++) {
    row_array[i] = 0;
    for (size_t j = 0; j < half_N; j++)
      row_array[i] = row_array[i] + matrix_in_1.elements[i][(j << 1)] *
                                        matrix_in_1.elements[i][(j << 1) + 1];
  }

  for (size_t i = 0; i < K; i++) {
    col_array[i] = 0;
    for (size_t j = 0; j < half_N; j++)
      col_array[i] = col_array[i] + matrix_in_2.elements[(j << 1)][i] *
                                        matrix_in_2.elements[(j << 1) + 1][i];
  }

  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < K; j++) {
      matrix_out.elements[i][j] = -row_array[i] - col_array[j];
      for (size_t k = 0; k < half_N; k++) {
        matrix_out.elements[i][j] = matrix_out.elements[i][j] +
                                    (matrix_in_1.elements[i][(k << 1)] +
                                     matrix_in_2.elements[(k << 1) + 1][j]) *
                                        (matrix_in_1.elements[i][(k << 1) + 1] +
                                         matrix_in_2.elements[(k << 1)][j]);
      }
    }
  }

  if (N % 2 == 1) {
    size_t N_minus_1 = N - 1;
    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < K; j++)
        matrix_out.elements[i][j] =
            matrix_out.elements[i][j] + matrix_in_1.elements[i][N_minus_1] *
                                            matrix_in_2.elements[N_minus_1][j];
  }

  free(row_array);
  free(col_array);
  return OK;
}
