#include <stdbool.h>
#include <stdio.h>

#include "errors.h"
#include "io.h"
#include "matrix.h"
#include "measure.h"
#include "operations.h"

int get_mode(bool *is_measure) {
  puts("Выберите режим работы:");
  puts("0 - ручное умножение матриц");
  puts("1 - замеры времени умножения");
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
    matrix_t matrix_in_1, matrix_in_2, matrix_out;
    puts("Введите первую матрицу");
    ret_code = read_matrix(&matrix_in_1);
    if (ret_code != OK)
      return process_error(ret_code);
    puts("Введите вторую матрицу");
    ret_code = read_matrix(&matrix_in_2);
    if (ret_code != OK) {
      free_matrix(matrix_in_1);
      return process_error(ret_code);
    }

    ret_code = prepare_out(matrix_in_1, matrix_in_2, &matrix_out);
    if (ret_code != OK)
      return process_error(ret_code);

    ret_code = multiply_std(matrix_in_1, matrix_in_2, matrix_out);
    if (ret_code != OK)
      return process_error(ret_code);
    puts("Результат умножения стандартным алгоритмом:");
    write_matrix(matrix_out);
    ret_code = multiply_vinograd(matrix_in_1, matrix_in_2, matrix_out);
    if (ret_code != OK)
      return process_error(ret_code);
    puts("Результат умножения алгоритмом Винограда:");
    write_matrix(matrix_out);

    ret_code = multiply_vinograd_opt(matrix_in_1, matrix_in_2, matrix_out);
    if (ret_code != OK)
      return process_error(ret_code);
    puts("Результат умножения оптимизированным алгоритмом Винограда:");
    write_matrix(matrix_out);
  } else {
    ret_code = run_benchmarks();
    if (ret_code != OK)
      return process_error(ret_code);
  }

  return OK;
}
