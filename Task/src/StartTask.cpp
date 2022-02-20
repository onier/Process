//
// Created by xuzhenhai on 2022/2/15.
//

#include "StartTask.h"
#include "AbstractTask.h"
#include "ProcessContext.h"

bool StartTask::initTask(Process::ProcessContext* manager) {
    return true;
}

void StartTask::run(std::shared_ptr<Process::ProcessContext> context) {
    LOG(INFO) <<  _taskName << " process is start";
}
