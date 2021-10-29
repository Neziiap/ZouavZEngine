//
// Created by pitch on 02/12/2020.
//

#ifndef MULTITHREAD_THREADPOOL_H
#define MULTITHREAD_THREADPOOL_H

#include <atomic>
#include <thread>
#include <condition_variable>
#include "TaskSystem.h"

class ThreadPool
{
private:

    std::vector<std::thread> threads;

    TaskSystem taskSystem;

    std::condition_variable wakeCondition;
    std::mutex wakeMutex;
    std::atomic<int> finishedJob;
    int currentJob = 0;
    unsigned int threadNumber = 0;
    bool looping = true;
public:

    void Initialize();

    template<typename CallType, typename... Args>
    void Execute(CallType&& _function, Args&&... _args)
    {
        currentJob += 1;
        taskSystem.Add(_function, _args...);

        wakeCondition.notify_one();
        std::this_thread::yield();
    }

    void Wait();

    ~ThreadPool();
};


#endif //MULTITHREAD_THREADPOOL_H
