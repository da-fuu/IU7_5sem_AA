#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "io.h"

int read_array(int **array) {
  if (array == NULL)
    return ERR_ARGS;

  size_t len = 0;
  size_t capacity = 10;

  *array = malloc(sizeof(int) * capacity);
  if (*array == NULL)
    return ERR_NO_MEMORY;

  puts("Введите последовательность, оканчивающуюся нулём: ");

  do {
    if (scanf("%d", &((*array)[len])) != 1) {
      free(*array);
      return ERR_INPUT;
    }
    if (len + 1 == capacity) {
      capacity += capacity + 1;
      int *tmp = realloc(*array, sizeof(int) * capacity);
      if (tmp == NULL) {
        free(*array);
        return ERR_NO_MEMORY;
      }
      *array = tmp;
    }
  } while ((*array)[len++] != 0);

  return OK;
}
