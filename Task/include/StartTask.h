//
// Created by xuzhenhai on 2022/2/15.
//

#ifndef PUPPY_STARTTASK_H
#define PUPPY_STARTTASK_H
#include "AbstractTask.h"

class StartTask : public Process::AbstractTask {
public:
    bool initTask(Process::ProcessContext* context) override;

    void run(std::shared_ptr<Process::ProcessContext> context) override;

    std::string getTaskType() override;

RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_STARTTASK_H
