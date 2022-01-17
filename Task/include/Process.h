//
// Created by xuzhenhai on 2021/10/21.
//

#pragma once

#include "vector"
#include "memory"
#include "Task.h"
#include "folly/Synchronized.h"
#include "boost/any.hpp"
#include "ExclusiveTask.h"
#include "folly/executors/CPUThreadPoolExecutor.h"
#include "folly/futures/Future.h"
#include "TaskManager.h"

namespace Process {
    class Process {
    public:
        Process(const std::shared_ptr<TaskManager> &taskManager);

        ~Process();

        void startProcess(folly::Synchronized<std::map<std::string, boost::any>> &values);

        void processTask(std::shared_ptr<AbstractTask> task);

        folly::CPUThreadPoolExecutor executor{13};

        folly::Synchronized<std::map<std::string, boost::any>> _processValues;
        std::shared_ptr<TaskManager> _taskManager;
        std::function<void(void)> _taskFinishFunction;
    private:
        void processDefaultTask(std::shared_ptr<AbstractTask> task);

        void processParallelTask(std::shared_ptr<AbstractTask> task);

        void processSubProcessTask(std::shared_ptr<AbstractTask> sharedPtr);
    };
}


