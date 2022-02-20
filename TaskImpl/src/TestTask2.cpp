//
// Created by xuzhenhai on 2021/10/21.
//

#include "TestTask2.h"
#include "glog/logging.h"
#include "XML.h"

bool TestTask2::initTask(Process::ProcessContext * manager) {
    return true;
}

void TestTask2::run(std::shared_ptr<Process::ProcessContext> manager) {
    LOG(INFO) << _name;
}