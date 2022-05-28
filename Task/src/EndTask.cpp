//
// Created by xuzhenhai on 2022/2/15.
//

#include "EndTask.h"

bool EndTask::initTask(Process::ProcessContext *manager) {
    return true;
}

void EndTask::run(std::shared_ptr<Process::ProcessContext> context) {
    LOG(INFO) << _taskName << " is end";
}

std::string EndTask::getTaskType() {
    return "EndTask";
}

EndTask::EndTask() {
    _name = "EndTask";
}
