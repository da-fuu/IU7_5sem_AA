#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

struct Graph {
  std::vector<std::vector<int>> in_edges;
  std::vector<std::vector<int>> out_edges;

  int getNumVertices() const { return in_edges.size(); }
};

void generateTestGraphFile(const std::string &filename, int verticesCount,
                           int maxEdges);

Graph loadGraph(const std::string &filename);

#endif // GRAPH_H
