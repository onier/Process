//
// Created by xuzhenhai on 2021/10/22.
//

#include "ParallelGateway.h"
#include "XML.h"

ParallelGateway::ParallelGateway() {
    _taskCount = 0;
}

void ParallelGateway::run(folly::Synchronized<std::map<std::string, boost::any>> &values) {
}