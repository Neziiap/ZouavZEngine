//
// Created by pitch on 02/12/2020.
//

#ifndef MULTITHREAD_TASK_H
#define MULTITHREAD_TASK_H

#include <functional>
#include <vector>
#include <tuple>
#include <thread>
#include <iostream>

class TaskDataBase
{
public:
    TaskDataBase() = default;
    virtual ~TaskDataBase() = default;

    virtual void operator()() const = 0;
};

template<typename... Args>
class TaskData : public TaskDataBase
{
private:

    std::tuple<Args...> args;
    std::function<void(Args...)> function;

public:

    explicit TaskData(std::function<void(Args...)>&& _function, Args&&... _args)
            :   function(std::forward<std::function<void(Args...)>>(_function)),
                args(std::forward<Args>(_args)...)
    {}

    void operator()() const final
    {
        std::apply(function, args);
    }
};


class Task
{
private:

    std::unique_ptr<TaskDataBase> taskData;

public:
    int taskID;
    Task() = default;

    template<typename CallType, typename... Args>
    Task(CallType&& _function, int tasskid, Args&&... _args)
    {
        taskID = tasskid;
        //std::cout << "Create " << taskID << std::endl;
        taskData = std::make_unique<TaskData<Args...>>(std::forward<CallType>(_function), std::forward<Args>(_args)...);
    }

    void operator()() const
    {
        //std::cout << "Execute " << taskID << std::endl;
        taskData->operator()();
    }
};

#endif //MULTITHREAD_TASK_H
