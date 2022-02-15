//
// Created by xuzhenhai on 2022/2/15.
//

#include "EndTask.h"

bool EndTask::initTask(std::shared_ptr <Process::TaskManager> manager) {
    return true;
}

void EndTask::run(folly::Synchronized <std::map<std::string, boost::any>> &values) {
    LOG(INFO) << "the process is start";
}
