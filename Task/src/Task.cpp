//
// Created by xuzhenhai on 2021/10/21.
//

#include "Task.h"

Process::Task::Task() {}

Process::Task::~Task() {}

Process::TaskException::TaskException() {

}

const char *Process::TaskException::what() const noexcept {
    return _type.c_str();
}

Process::TaskException::TaskException(const std::string message) : _message(message) {}

Process::TaskRuntimeException::TaskRuntimeException(const std::string type, const std::string message) : TaskException(
        message) {
    _type = type;
}

Process::TaskRuntimeException::~TaskRuntimeException() {

}

const char *Process::TaskRuntimeException::what() const noexcept {
    return _message.data();
}

Process::TaskError::TaskError(const std::string message) : TaskException(message) {}

const char *Process::TaskError::what() const noexcept {
    return _message.data();
}

const char *Process::UserDefinedExeception::what() const noexcept {
    return TaskException::what();
}

Process::UserDefinedExeception::UserDefinedExeception(const std::string &message) : TaskException(message) {}
