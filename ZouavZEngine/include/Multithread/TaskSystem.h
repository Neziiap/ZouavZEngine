#pragma once

#include "Task.h"
#include <condition_variable>

class TaskSystem
{
private:
    std::vector<Task> tasks;
    std::mutex lock;
    int nbTask = 0;
    int taskGet = 0;
public:

    TaskSystem() = default;

    template<typename CallType, typename... Args>
    void Add(CallType&& _function, Args&&... _args)
    {
        lock.lock();
        tasks.push_back({_function, nbTask++, _args...});
        lock.unlock();
    }

    Task* ExecuteAvailableTask()
    {
        lock.lock();
        if (tasks.empty() || taskGet > tasks.size() - 1)
        {
            lock.unlock();
            return nullptr;
        }
        //std::cout << "get " << tasks[taskGet].taskID << std::endl;
        Task* task = &tasks[taskGet];
        taskGet++;
        lock.unlock();
        return task;
    }

    void ExecuteTask(int i)
    {
        tasks[i].operator()();
    }
};