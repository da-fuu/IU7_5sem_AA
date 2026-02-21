#ifndef ERRORS_H__
#define ERRORS_H__

#define OK 0
#define ERR_MODE 1
#define ERR_INPUT 2
#define ERR_DIMENSIONS 3
#define ERR_NO_MEMORY 4
#define ERR_DIMENSIONS_MULTIPLY 5
#define ERR_FILE 6
#define ERR_UNIT_FAIL 7

int process_error(int err_code);

#endif // #ifndef ERRORS_H__
