//
// Created by xuzhenhai on 2021/10/21.
//

#include "Task.h"

Process::Task::Task() {}

Process::Task::~Task() {}

Process::TaskException::TaskException() {

}

Process::TaskException::~TaskException() {

}

const char *Process::TaskException::what() const noexcept {
    return _type.c_str();
}

Process::TaskException::TaskException(const std::string &type) : _type(type) {}
