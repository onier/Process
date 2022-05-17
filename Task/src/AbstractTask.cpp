//
// Created by xuzhenhai on 2021/10/21.
//

#include "AbstractTask.h"
#include "glog/logging.h"
#include "XML.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "boost/lexical_cast.hpp"

Process::AbstractTask::AbstractTask() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    _id = boost::lexical_cast<std::string>(uuid);
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
    LOG(ERROR) << "you must overrite the default run";
    exit(11);
}

std::string Process::AbstractTask::getTaskType() {
    return _taskType;
}

void Process::AbstractTask::setNextTaskID(std::string id, bool f) {
    _nextTaskID = id;
}

void Process::AbstractTask::setPreTaskID(std::string id, bool f) {
    _preTaskID = id;
}
