//
// Created by xuzhenhai on 2022/2/11.
//

#include "ExceptionTask.h"
#include "Task.h"
#include "glog/logging.h"

bool ExceptionTask::initTask(Process::ProcessContext* manager) {
    _runTimeExceptionCounter = 0;
    return true;
}

void ExceptionTask::run(std::shared_ptr<Process::ProcessContext> manager) {
//    if (_runTimeExceptionCounter < 5) {
//        _runTimeExceptionCounter++;
//        LOG(INFO) << "_runTimeExceptionCounter is " << _runTimeExceptionCounter;
//        manager->notifyEvent("EventA",this);
//        throw Process::TaskRuntimeException("", "retry again");
//    } else {
//        _runTimeExceptionCounter = 1;
//    }
    manager->notifyEvent("EventB",this);
//    throw Process::TaskError("reset process");
}
