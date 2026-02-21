#include <check.h>
#include <stdio.h>

#include "check_operations.h"
#include "errors.h"

int main(void) {
  Suite *s_multiply = multiplication_suite();
  SRunner *runner = srunner_create(s_multiply);

  srunner_run_all(runner, CK_SILENT);
  int failed_tests = srunner_ntests_failed(runner);
  printf("%d\n%d\n", srunner_ntests_run(runner) - failed_tests, failed_tests);
  srunner_free(runner);

  return OK;
}
