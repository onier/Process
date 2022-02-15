//
// Created by xuzhenhai on 2022/2/15.
//

#include "StartTask.h"
#include "AbstractTask.h"

bool StartTask::initTask(std::shared_ptr<Process::TaskManager> manager) {
    return true;
}

void StartTask::run(folly::Synchronized <std::map<std::string, boost::any>> &values) {
    LOG(INFO) <<  _taskName << " process is start";
}
