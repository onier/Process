//
// Created by xuzhenhai on 2021/10/21.
//

#ifndef PUPPY_TESTTASK3_H
#define PUPPY_TESTTASK3_H

#include "AbstractTask.h"

class TestTask3 : public Process::AbstractTask {
public:
    bool initTask(Process::ProcessContext * context) override;


    void run(std::shared_ptr<Process::ProcessContext> manager) override;

RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_TESTTASK3_H
