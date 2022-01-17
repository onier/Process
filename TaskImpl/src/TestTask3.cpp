//
// Created by xuzhenhai on 2021/10/21.
//

#include "TestTask3.h"
#include "glog/logging.h"
#include "XML.h"

bool TestTask3::initTask(std::shared_ptr<Process::TaskManager> manager) {
    return true;
}

void TestTask3::run(folly::Synchronized<std::map<std::string, boost::any>> &values) {
    LOG(INFO) << _name;
}
