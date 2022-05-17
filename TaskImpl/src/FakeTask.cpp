//
// Created by tian on 2022/3/5.
//

#include "FakeTask.h"
#include "chrono"
bool FakeTask::initTask(Process::ProcessContext *manager) {
    return true;
}

void FakeTask::run(std::shared_ptr<Process::ProcessContext> manager) {
    for(auto & item:_values){
        manager->getProcessValues()->wlock()->operator[](item.first) = item.second;
    }
   LOG(INFO)<<_text;
    if(!_eventName.empty()){
        manager->notifyEvent(_eventName,this);
    }
    if(_n>0){
       std::this_thread::sleep_for(std::chrono::milliseconds(_n));
    }
}
