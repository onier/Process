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
        if (_type == ExclusiveRuleType::DOUBLE) {
            double temp = boost::any_cast<double>(lock->at(_valueName));
            switch (_operator) {
                case ExclusiveRuleOperator::EQUAL:
                    return temp == folly::to<double>(_value);
                case ExclusiveRuleOperator::GREATER:
                    return temp > folly::to<double>(_value);
                case ExclusiveRuleOperator::LESS:
                    return temp < folly::to<double>(_value);
                case ExclusiveRuleOperator::GREATER_EUQAL:
                    return temp >= folly::to<double>(_value);
                case ExclusiveRuleOperator::LESS_EUQAL:
                    return temp <= folly::to<double>(_value);
            }
        }
        if (_type == ExclusiveRuleType::STRING) {
            std::string temp = boost::any_cast<std::string>(lock->at(_valueName));
            return temp == _value;
        }
    }
    return false;
}

void ExclusiveGateway::run(std::shared_ptr<Process::ProcessContext> context) {
    if (_subTaskVariant.empty()) {
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

    } else {
        auto values = context->getProcessValues();
        for (auto &st: _subTaskVariant) {
            ExclusiveRule task = (*st._variant).get_value<ExclusiveRule>();
            if (task.checkRule(values)) {
                _nextTaskID = task._taskID;
//            LOG(INFO) << " ExclusiveGateway  found  next task " << task._taskID ;
                return;
            }
        }
        _nextTaskID = "";
    }
    return;
}

ExclusiveRule::ExclusiveRule() {

}

ExclusiveGateway::ExclusiveGateway() {

}

std::string ExclusiveGateway::getTaskType() {
    return "ExclusiveGateway";
}

void ExclusiveGateway::setNextTaskID(std::string id, bool f) {
    if (f) {
        auto iter = std::find_if(_subTaskVariant.begin(), _subTaskVariant.end(), [id](VariantRule &rule) {
            return rule._id == id;
        });
        if (iter != _subTaskVariant.end()) {
            _subTaskVariant.erase(iter);
        }
    } else {
        auto iter = std::find_if(_subTaskVariant.begin(), _subTaskVariant.end(), [id](VariantRule &rule) {
            return rule._id == id;
        });
        if (iter == _subTaskVariant.end()) {
            rttr::variant var = rttr::type::get_by_name("ExclusiveRule").create();
            std::shared_ptr<rttr::variant> variant = std::make_shared<rttr::variant>(var);
            auto pro = variant->get_type().get_property("TaskID");
            pro.set_value(*variant, id);
            _subTaskVariant.push_back({id, variant});
            Process::AbstractTask::notify("addExclusiveRule", variant);
        }
    }
}

void ExclusiveGateway::setPreTaskID(std::string id, bool f) {
    AbstractTask::setPreTaskID(id, f);
}

ExclusiveRule::ExclusiveRule(const std::string &valueName, ExclusiveRuleOperator op, double value, const std::string &taskId)
        : _valueName(valueName), _operator(op), _taskID(taskId) {}


