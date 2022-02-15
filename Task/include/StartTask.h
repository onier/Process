//
// Created by xuzhenhai on 2022/2/15.
//

#ifndef PUPPY_STARTTASK_H
#define PUPPY_STARTTASK_H
#include "AbstractTask.h"

class StartTask : public Process::AbstractTask {
public:
    bool initTask(std::shared_ptr <Process::TaskManager> manager) override;

    void run(folly::Synchronized <std::map<std::string, boost::any>> &values) override;


    RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_STARTTASK_H
