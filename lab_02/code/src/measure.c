#include "errors.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_SIZE 1000
#define MAX_SIZE 40000
#define STEP 1000
#define NUM_RUNS 500

void fill_random_arr(int *arr, size_t size) {
  for (size_t i = 0; i < size - 1; ++i) {
    do {
      arr[i] = (rand() - (RAND_MAX / 2)) / 1024 % 65536;
    } while (arr[i] == 0);
  }
  arr[size - 1] = 0;
}

const double clk_per_mcs = CLOCKS_PER_SEC / 1000000;

double benchmark_iter(int *arr) {
  double ans;
  clock_t start = clock();
  ans = calc_avg_iter(arr);
  clock_t end = clock();
  (void)ans;
  return ((double)(end - start)) / clk_per_mcs;
}

double benchmark_rec(int *arr) {
  double ans;
  long long sum = 0;
  size_t len = 0;
  clock_t start = clock();
  ans = calc_avg_rec(arr, &sum, &len);
  clock_t end = clock();
  (void)ans;
  return ((double)(end - start)) / clk_per_mcs;
}

int run_benchmarks() {
  int array[MAX_SIZE];
  FILE *output_file = fopen("data.txt", "w");
  if (output_file == NULL)
    return ERR_FILE;

  printf("Размер\tИтеративный(мкс)\tРекурсивный(мкс)\n");
  for (size_t size = MIN_SIZE; size <= MAX_SIZE; size += STEP) {
    fill_random_arr(array, size);

    double total_iter = 0.0, total_rec = 0.0;

    for (int run = 0; run < NUM_RUNS; ++run) {
      total_iter += benchmark_iter(array);
      total_rec += benchmark_rec(array);
    }

    double avg_iter = total_iter / NUM_RUNS;
    double avg_rec = total_rec / NUM_RUNS;

    printf("%zu\t%.3lf\t\t\t%.3lf\n", size, avg_iter, avg_rec);
    fprintf(output_file, "%zu %.3lf %.3lf\n", size, avg_iter, avg_rec);
  }
  fclose(output_file);
  return OK;
}
