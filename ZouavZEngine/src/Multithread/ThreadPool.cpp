#include "Multithread/ThreadPool.h"


void ThreadPool::Initialize()
{
    finishedJob.store(0);

    threadNumber = std::thread::hardware_concurrency();

    for (unsigned int i = 0; i < threadNumber; ++i)
    {
        threads.emplace_back(std::thread([this]
        {
            Task* task;
            while (looping)
            {
                task = taskSystem.ExecuteAvailableTask();
                if (task)
                {
                    task->operator()();
                    finishedJob.fetch_add(1);
                }
                else
                {
                    std::unique_lock<std::mutex> lock(wakeMutex);
                    wakeCondition.wait(lock);
                }
                task = nullptr;
            }
        }
        ));

        threads[threads.size() - 1].detach();
    }
}



void ThreadPool::Wait()
{
    while (finishedJob.load() < currentJob)
    {

    }
}

ThreadPool::~ThreadPool() {
    looping = false;
}
