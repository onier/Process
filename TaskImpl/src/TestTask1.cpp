//
// Created by xuzhenhai on 2021/10/21.
//

#include "TestTask1.h"
#include "glog/logging.h"
#include "XML.h"

bool TestTask1::initTask(Process::ProcessContext * manager) {
    return true;
}

void TestTask1::run(std::shared_ptr<Process::ProcessContext> manager) {
    LOG(INFO) << _name;
}
