#ifndef OPERATIONS_H__
#define OPERATIONS_H__

#include "matrix.h"

int multiply_std(matrix_t matrix_in_1, matrix_t matrix_in_2,
                 matrix_t matrix_out);

int multiply_vinograd(matrix_t matrix_in_1, matrix_t matrix_in_2,
                      matrix_t matrix_out);

int multiply_vinograd_opt(matrix_t matrix_in_1, matrix_t matrix_in_2,
                          matrix_t matrix_out);

int prepare_out(matrix_t matrix_in_1, matrix_t matrix_in_2,
                matrix_t *matrix_out);

#endif // #ifndef OPERATIONS_H__
