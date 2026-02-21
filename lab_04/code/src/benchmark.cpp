#include "benchmark.h"
#include "graph.h"
#include "solver.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

void runFullBenchmark() {
  const int MAX_NODES = 800000;
  const int STEP = 25000;
  const int RUNS = 100;
  const int EDGES = 15;
  std::string filename = "graph_benchmark.dot";

  std::ifstream check(filename);
  if (!check.good()) {
    std::cout << "Создание графа...";
    generateTestGraphFile(filename, MAX_NODES, EDGES);
  }

  Graph graph;
  try {
    std::cout << "Загрузка графа...";
    graph = loadGraph(filename);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return;
  }

  std::vector<int> resultBuffer;
  resultBuffer.reserve(MAX_NODES);

  int threshold = 20;
  CompareMode mode = MODE_GREATER;
  std::vector<int> threadCounts = {1, 2, 4, 8, 16, 32, 64, 128};

  std::vector<int> sizes;
  for (int s = STEP; s <= MAX_NODES; s += STEP)
    sizes.push_back(s);

  std::ofstream dataFile("benchmark_results.txt");
  dataFile << "# Size Sequential";
  for (int t : threadCounts)
    dataFile << " Par_" << t;
  dataFile << "\n";

  for (int size : sizes) {
    std::cout << "Размер: " << size << "... " << std::flush;
    dataFile << size << " ";

    double seqTotal = 0;
    for (int r = 0; r < RUNS; ++r) {
      auto start = std::chrono::high_resolution_clock::now();
      solveSequential(graph, threshold, mode, size, resultBuffer);
      auto end = std::chrono::high_resolution_clock::now();
      seqTotal +=
          std::chrono::duration<double, std::milli>(end - start).count();
    }
    dataFile << (seqTotal / RUNS) << " ";

    for (int k : threadCounts) {
      double parTotal = 0;
      // Прогрев
      solveParallel(graph, threshold, mode, k, size, resultBuffer);

      for (int r = 0; r < RUNS; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        solveParallel(graph, threshold, mode, k, size, resultBuffer);
        auto end = std::chrono::high_resolution_clock::now();
        parTotal +=
            std::chrono::duration<double, std::milli>(end - start).count();
      }
      dataFile << (parTotal / RUNS) << " ";
    }

    dataFile << "\n";
    dataFile.flush();
  }

  dataFile.close();
}
