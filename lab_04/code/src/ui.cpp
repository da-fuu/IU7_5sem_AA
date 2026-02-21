#include "ui.h"
#include "benchmark.h"
#include "graph.h"
#include "solver.h"
#include <chrono>
#include <cstring>
#include <iostream>

void actionGenerate() {
  std::string fname;
  int nodes, edges;
  std::cout << "Имя файла: ";
  std::cin >> fname;
  std::cout << "Кол-во вершин: ";
  std::cin >> nodes;
  std::cout << "Максимальное количество рёбер: ";
  std::cin >> edges;

  generateTestGraphFile(fname, nodes, edges);
  std::cout << "Файл создан.\n";
}

void actionAnalyze() {
  std::string fname;
  int k, threads;
  std::string modeStr;

  std::cout << "Имя файла: ";
  std::cin >> fname;

  Graph g;
  try {
    g = loadGraph(fname);
    std::cout << "Загружено " << g.getNumVertices() << " вершин.\n";
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return;
  }

  std::cout << "Порог связей: ";
  std::cin >> k;
  std::cout << "Режим сравнения с порогом (< или >): ";
  std::cin >> modeStr;
  CompareMode mode = (modeStr == "<") ? MODE_LESS : MODE_GREATER;
  std::cout << "Потоков: ";
  std::cin >> threads;

  std::vector<int> res;
  res.reserve(g.getNumVertices());

  auto start = std::chrono::high_resolution_clock::now();
  solveParallel(g, k, mode, threads, g.getNumVertices(), res);
  auto end = std::chrono::high_resolution_clock::now();
  double ms = std::chrono::duration<double, std::milli>(end - start).count();

  std::cout << "Параллельный алгоритм: \n";
  std::cout << "Найдено: " << res.size() << " вершин.\n";
  for (int i = 0; i < res.size(); i++) {
    std::cout << res[i] << " ";
  }
  std::cout << "\nЗатраченное время: " << ms << " мс.\n";

  std::vector<int> res2;
  res2.reserve(g.getNumVertices());

  start = std::chrono::high_resolution_clock::now();
  solveSequential(g, k, mode, g.getNumVertices(), res2);
  end = std::chrono::high_resolution_clock::now();
  ms = std::chrono::duration<double, std::milli>(end - start).count();

  std::cout << "Последовательный алгоритм: \n";
  std::cout << "Найдено: " << res2.size() << " вершин.\n";
  for (int i = 0; i < res.size(); i++) {
    std::cout << res[i] << " ";
  }
  std::cout << "\nЗатраченное время: " << ms << " мс.\n";
}

void runTUI() {
  int choice = -1;
  while (choice != 0) {
    std::cout << "1. Создать файл с графом\n";
    std::cout << "2. Обработать граф\n";
    std::cout << "3. Провести замеры\n";
    std::cout << "0. Выход\n";
    std::cout << "Select: ";
    std::cin >> choice;

    switch (choice) {
    case 1:
      actionGenerate();
      break;
    case 2:
      actionAnalyze();
      break;
    case 3:
      runFullBenchmark();
      break;
    case 0:
      break;
    default:
      std::cout << "Неправильный пункт меню.\n";
    }
  }
}

void printUsage() {
  std::cout << "Использование:\n";
  std::cout
      << "  --gen <file> <nodes> <edges>            Создать файл с графом\n";
  std::cout << "  --analyze <file> <k> <mode> <threads>   Обработать граф\n";
  std::cout << "      mode: '<' или '>'\n";
  std::cout << "  --bench                                 Провести замеры\n";
}

void runCLI(int argc, char *argv[]) {
  if (argc < 2) {
    printUsage();
    return;
  }

  std::string cmd = argv[1];

  if (cmd == "--gen") {
    if (argc < 5) {
      std::cout << "Ошибка: недостаточно аргументов для --gen\n";
      printUsage();
      return;
    }
    std::string fname = argv[2];
    int nodes = std::stoi(argv[3]);
    int edges = std::stoi(argv[4]);
    generateTestGraphFile(fname, nodes, edges);
  } else if (cmd == "--analyze") {
    if (argc < 6) {
      std::cout << "Ошибка: недостаточно аргументов для --analyze\n";
      printUsage();
      return;
    }

    std::string fname = argv[2];
    int k = std::stoi(argv[3]);
    std::string modeStr = argv[4];
    int threads = std::stoi(argv[5]);

    CompareMode mode = (modeStr == "<") ? MODE_LESS : MODE_GREATER;

    Graph g;
    try {
      g = loadGraph(fname);
      std::cout << "Загружено " << g.getNumVertices() << " вершин.\n";
    } catch (const std::exception &e) {
      std::cerr << e.what() << "\n";
      return;
    }

    std::vector<int> res;
    res.reserve(g.getNumVertices());

    auto start = std::chrono::high_resolution_clock::now();
    solveParallel(g, k, mode, threads, g.getNumVertices(), res);
    auto end = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Найдено: " << res.size() << " вершин.\n";
    std::cout << "Затраченное время: " << ms << " мс.\n";
  } else if (cmd == "--bench") {
    runFullBenchmark();
  } else {
    std::cout << "Неизвестная команда: " << cmd << "\n";
    printUsage();
  }
}
