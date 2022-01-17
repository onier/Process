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

bool Process::AbstractTask::initTask(std::shared_ptr<TaskManager> manager) {
    return true;
}

void Process::AbstractTask::run(folly::Synchronized<std::map<std::string, boost::any>> &values) {
    exit(11);
}