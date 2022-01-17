//
// Created by xuzhenhai on 2021/10/21.
//

#include "TestTask1.h"
#include "glog/logging.h"
#include "XML.h"

bool TestTask1::initTask(std::shared_ptr<Process::TaskManager> manager) {
    return true;
}

void TestTask1::run(folly::Synchronized<std::map<std::string, boost::any>> &values) {
    LOG(INFO) << _name;
}
