#include "errors.h"
#include "matrix.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_SIZE_EVEN 100
#define MAX_SIZE_EVEN 400
#define MIN_SIZE_ODD 101
#define MAX_SIZE_ODD 401
#define STEP 10
#define NUM_RUNS 100

static void fill_random_matrix(matrix_t *m) {
  for (size_t i = 0; i < m->rows; ++i)
    for (size_t j = 0; j < m->cols; ++j)
      m->elements[i][j] = (double)rand() / RAND_MAX * 10.0;
}

static double benchmark_multiply_std(matrix_t a, matrix_t b, matrix_t c) {
  clock_t start = clock();
  multiply_std(a, b, c);
  clock_t end = clock();
  return ((double)(end - start)) / (CLOCKS_PER_SEC / 1000);
}

static double benchmark_multiply_vinograd(matrix_t a, matrix_t b, matrix_t c) {
  clock_t start = clock();
  multiply_vinograd(a, b, c);
  clock_t end = clock();
  return ((double)(end - start)) / (CLOCKS_PER_SEC / 1000);
}

static double benchmark_multiply_vinograd_opt(matrix_t a, matrix_t b,
                                              matrix_t c) {
  clock_t start = clock();
  multiply_vinograd_opt(a, b, c);
  clock_t end = clock();
  return ((double)(end - start)) / (CLOCKS_PER_SEC / 1000);
}

int run_benchmarks() {
  FILE *output_file = fopen("data_even.txt", "w");
  if (output_file == NULL)
    return ERR_FILE;

  printf("Размер\tСтандартный(мс)\tВиноград(мс)\tВиноград опт.(мс)\n");

  for (int size = MIN_SIZE_EVEN; size <= MAX_SIZE_EVEN; size += STEP) {
    matrix_t a = {0}, b = {0}, c = {0};
    a = create_matrix(size, size);
    b = create_matrix(size, size);
    c = create_matrix(size, size);

    double total_std = 0.0, total_vin = 0.0, total_opt = 0.0;

    for (int run = 0; run < NUM_RUNS; ++run) {
      fill_random_matrix(&a);
      fill_random_matrix(&b);

      total_std += benchmark_multiply_std(a, b, c);
      total_vin += benchmark_multiply_vinograd(a, b, c);
      total_opt += benchmark_multiply_vinograd_opt(a, b, c);
    }

    double avg_std = total_std / NUM_RUNS;
    double avg_vin = total_vin / NUM_RUNS;
    double avg_opt = total_opt / NUM_RUNS;

    printf("%d\t%.2f\t\t%.2f\t\t%.2f\t\n", size, avg_std, avg_vin, avg_opt);
    fprintf(output_file, "%d %.6f %.6f %.6f\n", size, avg_std, avg_vin,
            avg_opt);

    free_matrix(a);
    free_matrix(b);
    free_matrix(c);
  }
  fclose(output_file);
  output_file = fopen("data_odd.txt", "w");
  if (output_file == NULL)
    return ERR_FILE;
  printf("-----------------------------------------------\n");
  for (int size = MIN_SIZE_ODD; size <= MAX_SIZE_ODD; size += STEP) {
    matrix_t a = {0}, b = {0}, c = {0};
    a = create_matrix(size, size);
    b = create_matrix(size, size);
    c = create_matrix(size, size);

    double total_std = 0.0, total_vin = 0.0, total_opt = 0.0;

    for (int run = 0; run < NUM_RUNS; ++run) {
      fill_random_matrix(&a);
      fill_random_matrix(&b);

      total_std += benchmark_multiply_std(a, b, c);
      total_vin += benchmark_multiply_vinograd(a, b, c);
      total_opt += benchmark_multiply_vinograd_opt(a, b, c);
    }

    double avg_std = total_std / NUM_RUNS;
    double avg_vin = total_vin / NUM_RUNS;
    double avg_opt = total_opt / NUM_RUNS;

    printf("%d\t%.2f\t\t%.2f\t\t%.2f\t\n", size, avg_std, avg_vin, avg_opt);
    fprintf(output_file, "%d %.6f %.6f %.6f\n", size, avg_std, avg_vin,
            avg_opt);

    free_matrix(a);
    free_matrix(b);
    free_matrix(c);
  }
  fclose(output_file);
  return OK;
}
