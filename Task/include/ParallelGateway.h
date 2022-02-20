//
// Created by xuzhenhai on 2021/10/22.
//

#ifndef PUPPY_PARALLELGATEWAY_H
#define PUPPY_PARALLELGATEWAY_H

#include "Task.h"
#include "AbstractTask.h"


class ParallelGateway : public Process::AbstractTask {
public:
    ParallelGateway();

    void run(std::shared_ptr<Process::ProcessContext> context) override;

    /**
     * 行进入的任务，如果为空直接启动outTask
     */
    std::vector<std::string> _inTasks;
    /**
     * 并行开始执行的任务
     */
    std::vector<std::string> _outTasks;
    std::atomic<int> _taskCount;
    std::mutex _mutex;
RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_PARALLELGATEWAY_H
