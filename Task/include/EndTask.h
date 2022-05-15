//
// Created by xuzhenhai on 2022/2/15.
//

#ifndef PUPPY_ENDTASK_H
#define PUPPY_ENDTASK_H

#include "AbstractTask.h"

class EndTask : public Process::AbstractTask {
public:
    bool initTask(Process::ProcessContext* manager) override;

    void run(std::shared_ptr<Process::ProcessContext> context) override;

    std::string getTaskType() override;

RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_ENDTASK_H
