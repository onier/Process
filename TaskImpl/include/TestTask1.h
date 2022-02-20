//
// Created by xuzhenhai on 2021/10/21.
//

#ifndef PUPPY_TESTTASK1_H
#define PUPPY_TESTTASK1_H

#include "AbstractTask.h"

struct TestTask1 : public Process::AbstractTask {
public:
    bool initTask(Process::ProcessContext * manager) override;

    void run(std::shared_ptr<Process::ProcessContext> manager) override;


RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_TESTTASK1_H
