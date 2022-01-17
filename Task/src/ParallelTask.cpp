//
// Created by xuzhenhai on 2021/10/22.
//

#include "ParallelTask.h"
#include "XML.h"

ParallelTask::ParallelTask() {
    _taskCount = 0;
}

void ParallelTask::run(folly::Synchronized<std::map<std::string, boost::any>> &values) {
}