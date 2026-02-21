#include "operations.h"
#include "errors.h"

double calc_avg_iter(int *array) {
  long long sum = 0;
  size_t len = 0;

  while (array[len] != 0) {
    sum += array[len];
    len++;
  }
  len++;

  double avg = (double)sum / (double)len;
  return avg;
}

double calc_avg_rec(int *array, long long *sum, size_t *len) {
  double avg;

  (*len)++;

  if (*array == 0) {
    avg = (double)*sum / (double)*len;
    return avg;
  }

  *sum += *array;
  array++;

  avg = calc_avg_rec(array, sum, len);
  return avg;
}
