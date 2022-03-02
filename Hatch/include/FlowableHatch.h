//
// Created by xuzhenhai on 2022/3/1.
//

#ifndef PUPPY_FLOWABLEHATCH_H
#define PUPPY_FLOWABLEHATCH_H

#include <string>
#include "string"
#include "vector"
#include "map"
#include "memory"

struct Link {
    std::string _preTaskID;
    std::string _nextTaskID;

    virtual ~Link() {};
};

struct DummyStartEvent : public Link {
    std::string _id;
    std::string _name;
    std::string _documentation;
};

struct DummyEndEvent : public Link {
    std::string _id;
    std::string _name;
    std::string _documentation;
};

struct Parameter {
    /**
     * 参数的id
     */
    std::string _id;
    /**
     * 参数类型 boolean string int long double
     */
    std::string _type;
    std::string _value;
};

struct PrimitiveTask : public Link {
    /**
     * Task名称
     */
    std::string _name;
    /**
     * taskid start和end可以为空
     */
    std::string _id;
    /**
     * 描述
     */
    std::string _documentation;
    /**
     * 参数列表
     */
    std::vector<Parameter> _parameters;
};

struct DummyExclusiveGateway : public Link {
    /**
     * task event id
     */
    std::string _id;
    /**
     * 网关名称
     */
    std::string _name;
    /**
     * 条件语句   taskID
     */
    std::map<std::string, std::string> _exclusiveRules;
};

struct DummyEventBasedGateway : public Link {
    /**
     * task event id
     */
    std::string _id;
    /**
     * 网关名称
     */
    std::string _name;
    /**
     * 条件语句   taskID
     */
    std::map<std::string, std::string> _eventRules;
};


struct DummyParallelGateway : public Link {
/**
     * task event id
     */
    std::string _id;
    /**
     * 网关名称
     */
    std::string _name;
    /**
     *  等待完成的任务
     */
    std::vector<std::string> _inParallelRules;
    /**
     * 等待开始执行的任务
     */
    std::vector<std::string> _outPrallelRules;
};

struct SequenceFlow {
    std::string _id;
    std::string _sourceRef;
    std::string _targetRef;
    std::string _conditionExpression;
};

struct DummyProcess {
    std::shared_ptr<DummyStartEvent> _startEvent;
    std::shared_ptr<DummyEndEvent> _endEvent;
    std::vector<std::shared_ptr<PrimitiveTask>> _tasks;
    std::vector<std::shared_ptr<SequenceFlow>> _flows;
    std::vector<std::shared_ptr<DummyParallelGateway>> _parallelGateways;
    std::vector<std::shared_ptr<DummyExclusiveGateway>> _exclusiveGateways;
    std::vector<std::shared_ptr<DummyEventBasedGateway>> _eventBasedGateways;
    std::vector<std::shared_ptr<DummyProcess>> _subProcess;

    void constructTaskFlow();

    std::shared_ptr<Link> getLink(std::string id);

};

std::vector<std::string> split(const std::string &s, char seperator);

std::shared_ptr<DummyProcess> parseFromFlowableBpmn(std::string file);

#endif //PUPPY_FLOWABLEHATCH_H
