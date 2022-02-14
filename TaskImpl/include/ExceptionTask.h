//
// Created by xuzhenhai on 2022/2/11.
//

#ifndef PUPPY_EXCEPTIONTASK_H
#define PUPPY_EXCEPTIONTASK_H

#include "AbstractTask.h"

class ExceptionTask : public Process::AbstractTask {
    bool initTask(std::shared_ptr<Process::TaskManager> manager) override;

    void run(folly::Synchronized<std::map<std::string, boost::any>> &values) override;

public:
    int _runTimeExceptionCounter;

RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_EXCEPTIONTASK_H
