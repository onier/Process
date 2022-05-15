//
// Created by xuzhenhai on 2021/10/21.
//

#include "ExclusiveGateway.h"
#include "folly/Conv.h"
#include "XML.h"
#include "ProcessContext.h"

std::string ExclusiveGateway::getNextTaskID() {
    return _nextTaskID;
}

bool ExclusiveRule::checkRule(std::shared_ptr<folly::Synchronized<std::map<std::string, boost::any>>> values) {
    auto lock = values->rlock();
    if (lock->count(_valueName) > 0) {
        if (_type == "double") {
            double temp = boost::any_cast<double>(lock->at(_valueName));
            switch (_operator) {
                case Operator::EQUAL:
                    return temp == folly::to<double>(_value);
                case Operator::GREATER:
                    return temp > folly::to<double>(_value);
                case Operator::LESS:
                    return temp < folly::to<double>(_value);
                case Operator::GREATER_EUQAL:
                    return temp >= folly::to<double>(_value);
                case Operator::LESS_EUQAL:
                    return temp <= folly::to<double>(_value);
            }
        }
        if(_type == "string"){
            std::string temp = boost::any_cast<std::string>(lock->at(_valueName));
             return temp == _value;
        }
    }
    return false;
}

void ExclusiveGateway::run(std::shared_ptr<Process::ProcessContext> context) {
    if (_subTasks.empty()) {
        LOG(INFO) << "sub task is empty use default next task";
        return;
    }
    auto values = context->getProcessValues();
    for (auto &task: _subTasks) {
        if (task.checkRule(values)) {
            _nextTaskID = task._taskID;
//            LOG(INFO) << " ExclusiveGateway  found  next task " << task._taskID ;
            return;
        }
    }
    _nextTaskID = "";
    LOG(ERROR) << " ExclusiveGateway not found any next task";
    return;
}

ExclusiveRule::ExclusiveRule() {

}

ExclusiveGateway::ExclusiveGateway() {

}

std::string ExclusiveGateway::getTaskType() {
    return "ExclusiveGateway";
}

ExclusiveRule::ExclusiveRule(const std::string &valueName, Operator op, double value, const std::string &taskId)
        : _valueName(valueName), _operator(op),  _taskID(taskId) {}


