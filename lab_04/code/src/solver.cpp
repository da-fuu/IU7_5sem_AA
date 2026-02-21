#include "solver.h"
#include <functional>
#include <semaphore.h>
#include <thread>

struct SharedContext {
  const Graph *graph;
  int K;
  CompareMode mode;

  sem_t semFinishedStage1;
  sem_t semStartStage2;

  std::vector<ThreadData> tData;
  std::vector<int> *resultVector;

  SharedContext(int numThreads) {
    sem_init(&semFinishedStage1, 0, 0);
    sem_init(&semStartStage2, 0, 0);
    tData.resize(numThreads);
  }
  ~SharedContext() {
    sem_destroy(&semFinishedStage1);
    sem_destroy(&semStartStage2);
  }
};

void workerThread(SharedContext &ctx, int threadId, int startIdx, int endIdx) {
  const auto *in_ptr = ctx.graph->in_edges.data();
  const auto *out_ptr = ctx.graph->out_edges.data();

  int localCount = 0;
  size_t k_sz = static_cast<size_t>(ctx.K);

  if (ctx.mode == MODE_GREATER) {
    for (int i = startIdx; i < endIdx; ++i) {
      if (in_ptr[i].size() + out_ptr[i].size() > k_sz)
        localCount++;
    }
  } else {
    for (int i = startIdx; i < endIdx; ++i) {
      if (in_ptr[i].size() + out_ptr[i].size() < k_sz)
        localCount++;
    }
  }

  ctx.tData[threadId].count = localCount;
  sem_post(&ctx.semFinishedStage1);
  sem_wait(&ctx.semStartStage2);

  int writeIndex = ctx.tData[threadId].offset;
  int *res_raw_ptr = ctx.resultVector->data();

  if (ctx.mode == MODE_GREATER) {
    for (int i = startIdx; i < endIdx; ++i) {
      if (in_ptr[i].size() + out_ptr[i].size() > k_sz) {
        res_raw_ptr[writeIndex++] = i;
      }
    }
  } else {
    for (int i = startIdx; i < endIdx; ++i) {
      if (in_ptr[i].size() + out_ptr[i].size() < k_sz) {
        res_raw_ptr[writeIndex++] = i;
      }
    }
  }
}

void solveParallel(const Graph &g, int k, CompareMode mode, int numThreads,
                   int activeVertices, std::vector<int> &resultVector) {
  resultVector.clear();

  SharedContext ctx(numThreads);
  ctx.graph = &g;
  ctx.K = k;
  ctx.mode = mode;
  ctx.resultVector = &resultVector;

  std::vector<std::thread> threads;
  threads.reserve(numThreads);

  int chunkSize = activeVertices / numThreads;
  int remainder = activeVertices % numThreads;
  int start = 0;

  for (int i = 0; i < numThreads; ++i) {
    int end = start + chunkSize + (i < remainder ? 1 : 0);
    threads.emplace_back(workerThread, std::ref(ctx), i, start, end);
    start = end;
  }

  for (int i = 0; i < numThreads; ++i)
    sem_wait(&ctx.semFinishedStage1);

  int totalFound = 0;
  for (int i = 0; i < numThreads; ++i) {
    ctx.tData[i].offset = totalFound;
    totalFound += ctx.tData[i].count;
  }
  resultVector.resize(totalFound);

  for (int i = 0; i < numThreads; ++i)
    sem_post(&ctx.semStartStage2);
  for (auto &t : threads)
    t.join();
}

void solveSequential(const Graph &g, int k, CompareMode mode,
                     int activeVertices, std::vector<int> &resultVector) {
  resultVector.clear();
  const auto *in_ptr = g.in_edges.data();
  const auto *out_ptr = g.out_edges.data();
  size_t k_sz = static_cast<size_t>(k);

  if (mode == MODE_GREATER) {
    for (int i = 0; i < activeVertices; ++i) {
      if (in_ptr[i].size() + out_ptr[i].size() > k_sz) {
        resultVector.push_back(i);
      }
    }
  } else {
    for (int i = 0; i < activeVertices; ++i) {
      if (in_ptr[i].size() + out_ptr[i].size() < k_sz) {
        resultVector.push_back(i);
      }
    }
  }
}
