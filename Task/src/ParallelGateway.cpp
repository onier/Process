//
// Created by xuzhenhai on 2021/10/22.
//

#include "ParallelGateway.h"
#include "XML.h"

ParallelGateway::ParallelGateway() {
    _taskCount = 0;
}

void ParallelGateway::run(std::shared_ptr<Process::ProcessContext> context) {
}

std::string ParallelGateway::getTaskType() {
    return "ParallelGateway";
}

void ParallelGateway::setNextTaskID(std::string id, bool f) {
    if (f) {
        auto iter = std::find_if(_outTasks.begin(), _outTasks.end(), [id](auto task) {
            return id == task;
        });
        if (iter != _outTasks.end()) {
            _outTasks.erase(iter);
        }
    } else {
        auto iter = std::find_if(_outTasks.begin(), _outTasks.end(), [id](auto task) {
            return id == task;
        });
        if (iter == _outTasks.end()) {
            _outTasks.push_back(id);
        }
    }
}

void ParallelGateway::setPreTaskID(std::string id, bool f) {
    if (f) {
        auto iter = std::find_if(_inTasks.begin(), _inTasks.end(), [id](auto task) {
            return id == task;
        });
        if (iter != _inTasks.end()) {
            _inTasks.erase(iter);
        }
    } else {
        auto iter = std::find_if(_inTasks.begin(), _inTasks.end(), [id](auto task) {
            return id == task;
        });
        if (iter == _inTasks.end()) {
            _inTasks.push_back(id);
        }
    }
}