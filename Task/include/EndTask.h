//
// Created by xuzhenhai on 2022/2/15.
//

#ifndef PUPPY_ENDTASK_H
#define PUPPY_ENDTASK_H

#include "AbstractTask.h"

class EndTask : public Process::AbstractTask {
public:
    bool initTask(std::shared_ptr <Process::TaskManager> manager) override;

    void run(folly::Synchronized <std::map<std::string, boost::any>> &values) override;


    RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_ENDTASK_H
