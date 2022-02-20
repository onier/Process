//
// Created by xuzhenhai on 2021/10/21.
//

#include "AbstractTask.h"
#include "glog/logging.h"
#include "XML.h"

Process::AbstractTask::AbstractTask() {

}

std::string Process::AbstractTask::getID() {
    return _id;
}

std::string Process::AbstractTask::getName() {
    return _name;
}

std::string Process::AbstractTask::getNextTaskID() {
    return _nextTaskID;
}

std::string Process::AbstractTask::getPreTaskID() {
    return _preTaskID;
}

void Process::AbstractTask::run(std::shared_ptr<Process::ProcessContext> context) {
    exit(11);
}