//
// Created by xuzhenhai on 2021/10/21.
//

#include "TestTask3.h"
#include "glog/logging.h"
#include "XML.h"

bool TestTask3::initTask(Process::ProcessContext* manager) {
    return true;
}

void TestTask3::run(std::shared_ptr<Process::ProcessContext> manager) {
    LOG(INFO) << _name;
}
