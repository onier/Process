//
// Created by tian on 2022/3/6.
//

#include "WaitForTask.h"
#include "chrono"

bool WaitForTask::initTask(Process::ProcessContext *manager) {
   return true;
}

void WaitForTask::run(std::shared_ptr<Process::ProcessContext> manager) {
    LOG(INFO)<<" wait for "<<_valueName << " is "<<_value;
    for(;;){
        auto lock = manager->getProcessValues()->rlock();
        if(lock->count(_valueName)){
            std::string temp = boost::any_cast<std::string>(lock->at(_valueName));
            if(temp ==_value){
                LOG(INFO)<<" wait is over on "<<_valueName << " is "<<_value;
                return ;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

std::string WaitForTask::getTaskType() {
    return "UserTask";
}
