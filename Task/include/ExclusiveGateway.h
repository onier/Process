//
// Created by xuzhenhai on 2021/10/21.
//

#ifndef PUPPY_EXCLUSIVEGATEWAY_H
#define PUPPY_EXCLUSIVEGATEWAY_H

#include "AbstractTask.h"
#include "folly/Synchronized.h"
#include "boost/any.hpp"

enum class Operator {
    GREATER = 0x0001,
    LESS = 0x0002,
    EQUAL = 0x0003,
    GREATER_EUQAL = 0x0004,
    LESS_EUQAL = 0x0005
};

struct ExclusiveRule {
    std::string _valueName;
    Operator _operator;
    double _value;
    std::string _taskID;

    ExclusiveRule(const std::string &valueName, Operator op, double value, const std::string &taskId);

    ExclusiveRule(const std::string &valueName, Operator op, int value, const std::string &taskId);

    bool checkRule(folly::Synchronized<std::map<std::string, boost::any>> &values);

    ExclusiveRule();

RTTR_ENABLE()
};

class ExclusiveGateway : public Process::AbstractTask {
public:
    ExclusiveGateway();

    std::string getNextTaskID() override;

    void run(folly::Synchronized<std::map<std::string, boost::any>> &values) override;


    std::map<std::string, std::function<bool(folly::Synchronized<std::map<std::string, boost::any>> &)>> _ruleFunctions;

    std::vector<ExclusiveRule> _subTasks;
RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_EXCLUSIVEGATEWAY_H
