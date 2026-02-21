#include "graph.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>

void generateTestGraphFile(const std::string &filename, int verticesCount,
                           int maxEdges) {
  std::ofstream out(filename);
  out << "digraph G {\n";
  std::mt19937 rng(42);
  std::uniform_int_distribution<int> dist(0, verticesCount - 1);
  std::uniform_int_distribution<int> edgesDist(0, maxEdges);

  for (int i = 0; i < verticesCount; ++i) {
    if (i % 50000 == 0)
      out << i << ";\n";
    int edges = edgesDist(rng);
    for (int j = 0; j < edges; ++j) {
      int target = dist(rng);
      if (target != i)
        out << i << " -> " << target << ";\n";
    }
  }
  out << (verticesCount - 1) << ";\n";
  out << "}\n";
  out.close();
}

Graph loadGraph(const std::string &filename) {
  Graph g;
  std::ifstream in(filename);
  if (!in.is_open())
    throw std::runtime_error("File not found: " + filename);

  std::string line;
  while (std::getline(in, line)) {
    size_t arrowPos = line.find("->");
    if (arrowPos != std::string::npos) {
      try {
        const char *str = line.c_str();
        char *end;
        while (*str && !isdigit(*str))
          str++;
        int u_int = std::strtol(str, &end, 10);
        str = end;
        while (*str && !isdigit(*str))
          str++;
        if (*str) {
          int v_int = std::strtol(str, &end, 10);
          size_t u = static_cast<size_t>(u_int);
          size_t v = static_cast<size_t>(v_int);

          if (v >= g.in_edges.size())
            g.in_edges.resize(v + 1);
          g.in_edges[v].push_back(u_int);

          if (u >= g.out_edges.size())
            g.out_edges.resize(u + 1);
          g.out_edges[u].push_back(v_int);
        }
      } catch (...) {
      }
    }
  }
  return g;
}
