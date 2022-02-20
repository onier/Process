//
// Created by xuzhenhai on 2022/2/20.
//

#ifndef PUPPY_EVENTGATEWAY_H
#define PUPPY_EVENTGATEWAY_H


#include "AbstractTask.h"
#include "folly/Synchronized.h"
#include "boost/any.hpp"
#include "map"

class EventGateway : public Process::AbstractTask{
public:
    void run(std::shared_ptr<Process::ProcessContext> context) override;

    bool initTask(Process::ProcessContext *processContext) override;
    std::map<std::string,std::string> _eventRules;
RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_EVENTGATEWAY_H
