#include <stdio.h>

#include "errors.h"

int process_error(int err_code) {
  switch (err_code) {
  case ERR_MODE:
    puts("Ошибка при вводе режима работы программы!");
    break;
  case ERR_INPUT:
    puts("Ошибка ввода!");
    break;
  case ERR_DIMENSIONS:
    puts("Некорректные измерения матрицы!");
    break;
  case ERR_NO_MEMORY:
    puts("Недостаточно памяти!");
    break;
  case ERR_DIMENSIONS_MULTIPLY:
    puts("Некорректные измерения матриц для умножения!");
    break;
  default:
    break;
  }
  return err_code;
}
