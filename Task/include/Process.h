//
// Created by xuzhenhai on 2021/10/21.
//

#pragma once

#include "vector"
#include "memory"
#include "Task.h"
#include "folly/Synchronized.h"
#include "boost/any.hpp"
#include "folly/executors/CPUThreadPoolExecutor.h"
#include "folly/futures/Future.h"
#include "atomic"
#include "ProcessContext.h"

namespace Process {
    class Process {
    public:
        Process(int threadCount);

        Process(std::shared_ptr<ProcessContext> processContext);

        ~Process();

        void startProcess(std::shared_ptr<ProcessContext> context, bool isSub = false);

        void processTask(std::shared_ptr<Task> task);

        std::shared_ptr<ProcessContext> getProcessContext();

        void loadXML(std::string xml);

        void loadFile(std::string filePath);

        std::string saveXML();

        void initProcessValues(std::map<std::string,boost::any> values);

    private:
        void processDefaultTask(std::shared_ptr<Task> task);

        void processParallelTask(std::shared_ptr<Task> task);

        void processSubProcessTask(std::shared_ptr<Task> sharedPtr);

        std::shared_ptr<ProcessContext> _processContext;

        void initEventHandlers();

        std::mutex _mutex;
    };
}


