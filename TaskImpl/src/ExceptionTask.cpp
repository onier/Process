//
// Created by xuzhenhai on 2022/2/11.
//

#include "ExceptionTask.h"
#include "Task.h"
#include "glog/logging.h"

bool ExceptionTask::initTask(std::shared_ptr<Process::TaskManager> manager) {
    _runTimeExceptionCounter = 0;
    return true;
}

void ExceptionTask::run(folly::Synchronized<std::map<std::string, boost::any>> &values) {
    if (_runTimeExceptionCounter < 5) {
        _runTimeExceptionCounter++;
        LOG(INFO) << "_runTimeExceptionCounter is " << _runTimeExceptionCounter;
        throw Process::TaskRuntimeException("", "retry again");
    } else {
        _runTimeExceptionCounter = 1;
    }
    throw Process::TaskError("reset process");
}
