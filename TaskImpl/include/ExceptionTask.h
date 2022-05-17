//
// Created by xuzhenhai on 2022/2/11.
//

#ifndef PUPPY_EXCEPTIONTASK_H
#define PUPPY_EXCEPTIONTASK_H

#include "AbstractTask.h"

class ExceptionTask : public Process::AbstractTask {
    bool initTask(Process::ProcessContext* manager) override;

    void run(std::shared_ptr<Process::ProcessContext> manager) override;

public:
    int _runTimeExceptionCounter;

RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_EXCEPTIONTASK_H
