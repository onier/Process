//
// Created by tian on 2022/3/6.
//

#ifndef FUXIOS_WAITFORTASK_H
#define FUXIOS_WAITFORTASK_H

#include "AbstractTask.h"

class WaitForTask  : public Process::AbstractTask {
public:
    bool initTask(Process::ProcessContext * manager) override;

    void run(std::shared_ptr<Process::ProcessContext> manager) override;

    std::string getTaskType() override;

    std::string _valueName;
    std::string _value;
    std::string _type;

    RTTR_ENABLE(AbstractTask)
};


#endif //FUXIOS_WAITFORTASK_H
