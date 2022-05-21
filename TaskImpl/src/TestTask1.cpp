//
// Created by xuzhenhai on 2021/10/21.
//

#include "TestTask1.h"
#include "glog/logging.h"
#include "XML.h"
#include <chrono>
#include <thread>
bool TestTask1::initTask(Process::ProcessContext * manager) {
    return true;
}

void TestTask1::run(std::shared_ptr<Process::ProcessContext> manager) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    LOG(INFO) << _name;
}
