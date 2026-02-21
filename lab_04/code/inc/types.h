#ifndef TYPES_H
#define TYPES_H

#include <vector>

struct alignas(64) ThreadData {
  int count;
  int offset;
  char padding[56];
};

enum CompareMode { MODE_GREATER, MODE_LESS };

#endif // TYPES_H
