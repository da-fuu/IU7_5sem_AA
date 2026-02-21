#include "check_solver.h"
#include "solver.h"
#include <algorithm>
#include <check.h>
#include <functional>
#include <vector>
using SolverFunc = std::function<void(const Graph &, int, CompareMode, int,
                                      std::vector<int> &)>;

void run_test_simple_greater(SolverFunc solver) {
  Graph g;
  int numVertices = 4;
  g.in_edges.resize(numVertices);
  g.out_edges.resize(numVertices);

  g.out_edges[0].push_back(1);
  g.in_edges[1].push_back(0);
  g.out_edges[1].push_back(0);
  g.in_edges[0].push_back(1);
  g.out_edges[0].push_back(2);
  g.in_edges[2].push_back(0);

  std::vector<int> result;
  solver(g, 2, MODE_GREATER, numVertices, result);

  ck_assert_int_eq(result.size(), 1);
  if (!result.empty()) {
    ck_assert_int_eq(result[0], 0);
  }
}

void run_test_less_isolated(SolverFunc solver) {
  Graph g;
  int numVertices = 5;
  g.in_edges.resize(numVertices);
  g.out_edges.resize(numVertices);

  g.out_edges[0].push_back(1);
  g.in_edges[1].push_back(0);

  std::vector<int> result;
  solver(g, 1, MODE_LESS, numVertices, result);

  std::sort(result.begin(), result.end());

  ck_assert_int_eq(result.size(), 3);
  if (result.size() == 3) {
    ck_assert_int_eq(result[0], 2);
    ck_assert_int_eq(result[1], 3);
    ck_assert_int_eq(result[2], 4);
  }
}

void run_test_nothing_found(SolverFunc solver) {
  Graph g;
  int numVertices = 3;
  g.in_edges.resize(numVertices);
  g.out_edges.resize(numVertices);

  g.out_edges[0].push_back(1);
  g.in_edges[1].push_back(0);
  g.out_edges[1].push_back(2);
  g.in_edges[2].push_back(1);
  g.out_edges[2].push_back(0);
  g.in_edges[0].push_back(2);

  std::vector<int> result;
  solver(g, 5, MODE_GREATER, numVertices, result);

  ck_assert_int_eq(result.size(), 0);
}

void wrapper_seq(const Graph &g, int k, CompareMode mode, int active,
                 std::vector<int> &res) {
  solveSequential(g, k, mode, active, res);
}

void wrapper_par(const Graph &g, int k, CompareMode mode, int active,
                 std::vector<int> &res) {
  solveParallel(g, k, mode, 4, active, res);
}

START_TEST(test_seq_simple) { run_test_simple_greater(wrapper_seq); }
END_TEST

START_TEST(test_seq_less) { run_test_less_isolated(wrapper_seq); }
END_TEST

START_TEST(test_seq_empty) { run_test_nothing_found(wrapper_seq); }
END_TEST

START_TEST(test_par_simple) { run_test_simple_greater(wrapper_par); }
END_TEST

START_TEST(test_par_less) { run_test_less_isolated(wrapper_par); }
END_TEST

START_TEST(test_par_empty) { run_test_nothing_found(wrapper_par); }
END_TEST

Suite *graph_suite(void) {
  Suite *s;
  TCase *tc_seq;
  TCase *tc_par;

  s = suite_create("SolverTests");

  tc_seq = tcase_create("Sequential");
  tcase_add_test(tc_seq, test_seq_simple);
  tcase_add_test(tc_seq, test_seq_less);
  tcase_add_test(tc_seq, test_seq_empty);
  suite_add_tcase(s, tc_seq);

  tc_par = tcase_create("Parallel");
  tcase_add_test(tc_par, test_par_simple);
  tcase_add_test(tc_par, test_par_less);
  tcase_add_test(tc_par, test_par_empty);
  suite_add_tcase(s, tc_par);

  return s;
}
