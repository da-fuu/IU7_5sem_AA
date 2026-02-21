#include <check.h>
#include <stdio.h>

#include "check_solver.h"

int main(void) {
  Suite *suite = graph_suite();
  SRunner *runner = srunner_create(suite);

  srunner_run_all(runner, CK_SILENT);
  int failed_tests = srunner_ntests_failed(runner);
  printf("%d\n%d\n", srunner_ntests_run(runner) - failed_tests, failed_tests);
  srunner_free(runner);

  return 0;
}
