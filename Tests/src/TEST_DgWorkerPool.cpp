
#include <atomic>
#include <thread>
#include <chrono>

#include "TestHarness.h"
#include "DgWorkerPool.h"

TEST(Stack_DgWorkerPool, creation_DgWorkerPool)
{
  struct Data
  {
    std::atomic<int> doneTasks;
    std::atomic<int> donePostTasks;
  };

  Data data = {0, 0};

  Dg::WorkerPool workerPool(4);

  int const taskCount = 100;
  for (int i = 0; i < taskCount; i++)
  {
    Dg::ErrorCode err = workerPool.AddTask([](void * pData)
      {
        ((Data*)pData)->doneTasks++;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
      }, 
      &data, false, 
      [](void * pData){((Data*)pData)->donePostTasks++;});

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    CHECK(err == Dg::ErrorCode::None);
  }

  while (workerPool.HasActiveWorkers())
  {
    workerPool.DoPostWork();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  workerPool.DoPostWork();

  CHECK(data.doneTasks == taskCount);
  CHECK(data.donePostTasks == taskCount);
}