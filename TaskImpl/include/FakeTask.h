//
// Created by tian on 2022/3/5.
//

#ifndef FUXIOS_FAKETASK_H
#define FUXIOS_FAKETASK_H

#include "AbstractTask.h"
#include "map"
class FakeTask : public Process::AbstractTask {
public:
    bool initTask(Process::ProcessContext * manager) override;

    void run(std::shared_ptr<Process::ProcessContext> manager) override;


    std::string _text;
    int _n;
    long _l;
    double _d;
    std::map<std::string,std::string> _values;
    std::string _eventName;
    RTTR_ENABLE(AbstractTask)
};


#endif //FUXIOS_FAKETASK_H
