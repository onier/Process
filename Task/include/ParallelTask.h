//
// Created by xuzhenhai on 2021/10/22.
//

#ifndef PUPPY_PARALLELTASK_H
#define PUPPY_PARALLELTASK_H

#include "Task.h"
#include "AbstractTask.h"


class ParallelTask : public Process::AbstractTask {
public:
    ParallelTask();

    void run(folly::Synchronized<std::map<std::string, boost::any>> &values) override;

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


#endif //PUPPY_PARALLELTASK_H
