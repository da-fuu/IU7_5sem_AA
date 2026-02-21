#ifndef SOLVER_H
#define SOLVER_H

#include "graph.h"
#include "types.h"
#include <vector>

void solveSequential(const Graph &g, int k, CompareMode mode,
                     int activeVertices, std::vector<int> &resultVector);

void solveParallel(const Graph &g, int k, CompareMode mode, int numThreads,
                   int activeVertices, std::vector<int> &resultVector);

#endif // SOLVER_H
