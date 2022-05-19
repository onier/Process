//
// Created by xuzhenhai on 2021/10/21.
//

#ifndef PUPPY_EXCLUSIVEGATEWAY_H
#define PUPPY_EXCLUSIVEGATEWAY_H

#include "AbstractTask.h"
#include "folly/Synchronized.h"
#include "boost/any.hpp"

enum class ExclusiveRuleOperator {
    GREATER = 0x0001,
    LESS = 0x0002,
    EQUAL = 0x0003,
    GREATER_EUQAL = 0x0004,
    LESS_EUQAL = 0x0005
};

enum class ExclusiveRuleType {
    DOUBLE,
    STRING
};

struct ExclusiveRule {
    std::string _text;
    std::string _valueName;
    ExclusiveRuleOperator _operator;
    std::string _value;
    std::string _taskID;
    ExclusiveRuleType _type;

    ExclusiveRule(const std::string &valueName, ExclusiveRuleOperator op, double value, const std::string &taskId);

    ExclusiveRule(const std::string &valueName, ExclusiveRuleOperator op, int value, const std::string &taskId);

    bool checkRule(std::shared_ptr<folly::Synchronized<std::map<std::string, boost::any>>> values);

    ExclusiveRule();

RTTR_ENABLE()
};

struct VariantRule {
    std::string _id;
    std::shared_ptr<rttr::variant> _variant;
};

class ExclusiveGateway : public Process::AbstractTask {
public:
    ExclusiveGateway();

    std::string getNextTaskID() override;

    void run(std::shared_ptr<Process::ProcessContext> context) override;

    std::string getTaskType() override;

    std::map<std::string, std::function<bool(folly::Synchronized<std::map<std::string, boost::any>> &)>> _ruleFunctions;

    std::vector<ExclusiveRule> _subTasks;

    void setNextTaskID(std::string id, bool f) override;

    void setPreTaskID(std::string id, bool f) override;

private:
    std::vector<VariantRule> _subTaskVariant;
RTTR_ENABLE(AbstractTask)
};


#endif //PUPPY_EXCLUSIVEGATEWAY_H
