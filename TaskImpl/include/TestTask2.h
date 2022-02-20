//
// Created by xuzhenhai on 2021/10/21.
//

#ifndef PUPPY_TESTTASK2_H
#define PUPPY_TESTTASK2_H

#include "AbstractTask.h"

class TestTask2 : public Process::AbstractTask {
public:
    bool initTask(Process::ProcessContext * manager) override;

    void run(std::shared_ptr<Process::ProcessContext> manager) override;


RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_TESTTASK2_H
