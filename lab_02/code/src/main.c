#include <stdbool.h>
#include <stdio.h>

#include "errors.h"
#include "io.h"
#include "measure.h"
#include "operations.h"

int get_mode(bool *is_measure) {
  puts("Выберите режим работы:");
  puts("0 - ручное нахождение среднего значения");
  puts("1 - замеры времени выполнения алгоритма");
  int in;

  if (scanf("%d", &in) != 1) {
    return ERR_MODE;
  }

  if (in < 0 || in > 1) {
    return ERR_MODE;
  }
  *is_measure = (bool)in;
  return OK;
}

int main(void) {
  int ret_code;
  bool is_measure;

  ret_code = get_mode(&is_measure);
  if (ret_code != OK)
    return process_error(ret_code);

  if (!is_measure) {
    int *array;
    ret_code = read_array(&array);
    if (ret_code != OK)
      return process_error(ret_code);

    printf("Среднее значение итеративным алгоритмом: %lf\n",
           calc_avg_iter(array));

    long long sum = 0;
    size_t len = 0;
    printf("Среднее значение рекурсивным алгоритмом: %lf\n",
           calc_avg_rec(array, &sum, &len));
  } else {
    ret_code = run_benchmarks();
    if (ret_code != OK)
      return process_error(ret_code);
  }

  return OK;
}
