//
// Created by xuzhenhai on 2022/3/1.
//

#include "FlowableHatch.h"
#include "iostream"
#include "fstream"
#include "glog/logging.h"
#include "xercesc/util/PlatformUtils.hpp"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/dom/DOMNodeFilter.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include "XML.h"
#include <boost/algorithm/string.hpp>
#include "rttr/registration.h"
#include "boost/lexical_cast.hpp"
#include "SubProcessTask.h"

#define toStr(args)  puppy::common::XML::toStr(args)

std::string readFile(std::string path) {
    std::string line, content;
    std::ifstream myfile(path);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            content += line + '\n';
        }
        myfile.close();
    }
    return content;
}

auto getElementsByName(xercesc::DOMNode *parent, std::string name) {
    std::vector<xercesc::DOMNode *> result;
    auto children = parent->getChildNodes();
    for (int i = 0; i < children->getLength(); ++i) {
        if (toStr(children->item(i)->getNodeName()) == name) {
            result.push_back(children->item(i));
        }
    }
    return result;
}

std::string getChildContent(xercesc::DOMNode *node, std::string name) {
    auto children = getElementsByName(node, name);
    if (children.empty()) {
        return "";
    }
    return toStr(children.at(0)->getTextContent());
}

std::string getAttributeByName(xercesc::DOMNode *node, std::string name) {
    auto attrs = node->getAttributes();
    for (int i = 0; i < attrs->getLength(); ++i) {
        if (name == toStr(attrs->item(i)->getNodeName())) {
            return toStr(attrs->item(i)->getNodeValue());
        }
    }
    return "";
}

std::shared_ptr<DummyStartEvent> parseStartEvent(xercesc::DOMNode *pNode) {
    DummyStartEvent startEvent;
    auto attributes = pNode->getAttributes();
    for (int i = 0; i < attributes->getLength(); ++i) {
        std::string name = toStr(attributes->item(i)->getNodeName());
        if (name == "id") {
            startEvent._id = toStr(attributes->item(i)->getNodeValue());
            break;
        }
        LOG(INFO) << toStr(attributes->item(i)->getNodeName());
    }
    auto documentations = getElementsByName(pNode, "documentation");
    if (documentations.size() == 1)
        startEvent._documentation = toStr(documentations.at(0)->getTextContent());
    return std::make_shared<DummyStartEvent>(startEvent);
}

std::shared_ptr<DummyEndEvent> parseEndEvent(xercesc::DOMNode *pNode) {
    DummyEndEvent startEvent;
    auto attributes = pNode->getAttributes();
    for (int i = 0; i < attributes->getLength(); ++i) {
        std::string name = toStr(attributes->item(i)->getNodeName());
        if (name == "id") {
            startEvent._id = toStr(attributes->item(i)->getNodeValue());
            break;
        }
        LOG(INFO) << toStr(attributes->item(i)->getNodeName());
    }
    auto documentations = getElementsByName(pNode, "documentation");
    if (documentations.size() == 1)
        startEvent._documentation = toStr(documentations.at(0)->getTextContent());
    return std::make_shared<DummyEndEvent>(startEvent);
}

std::shared_ptr<PrimitiveTask> parsePrimitiveTask(xercesc::DOMNode *pNode) {
    PrimitiveTask primitiveTask;
    primitiveTask._id = getAttributeByName(pNode, "id");
    primitiveTask._name = getAttributeByName(pNode, "name");
    primitiveTask._documentation = getChildContent(pNode, "documentation");
    auto extensionElements = getElementsByName(pNode, "extensionElements");
    for (auto &exElement:extensionElements) {
        auto properties = getElementsByName(exElement, "flowable:formProperty");
        for (auto &pro:properties) {
            Parameter parameter;
            parameter._id = getAttributeByName(pro, "id");
            parameter._type = getAttributeByName(pro, "type");
            parameter._value = getAttributeByName(pro, "default");
            primitiveTask._parameters.push_back(parameter);
        }
    }
    return std::make_shared<PrimitiveTask>(primitiveTask);
}

std::shared_ptr<SequenceFlow> parseSequenceFlow(xercesc::DOMNode *node) {
    SequenceFlow sequenceFlow;
    sequenceFlow._id = getAttributeByName(node, "id");
    sequenceFlow._sourceRef = getAttributeByName(node, "sourceRef");
    sequenceFlow._targetRef = getAttributeByName(node, "targetRef");
    sequenceFlow._conditionExpression = getChildContent(node, "conditionExpression");
    return std::make_shared<SequenceFlow>(sequenceFlow);
}

std::shared_ptr<DummyExclusiveGateway> parseExclusiveGateway(xercesc::DOMNode *node) {
    DummyExclusiveGateway exclusiveGateway;
    exclusiveGateway._id = getAttributeByName(node, "id");
    exclusiveGateway._name = getAttributeByName(node, "name");
    return std::make_shared<DummyExclusiveGateway>(exclusiveGateway);
}

std::shared_ptr<DummyParallelGateway> parseParallelGateway(xercesc::DOMNode *node) {
    DummyParallelGateway parallelGateway;
    parallelGateway._id = getAttributeByName(node, "id");
    parallelGateway._name = getAttributeByName(node, "name");
    return std::make_shared<DummyParallelGateway>(parallelGateway);
}

std::shared_ptr<Link> DummyProcess::getLink(std::string id) {
    if (_startEvent->_id == id) {
        return _startEvent;
    }
    if (_endEvent->_id == id) {
        return _endEvent;
    }
    for (auto link:_tasks) {
        if (link->_id == id) {
            return link;
        }
    }
    for (auto link:_exclusiveGateways) {
        if (link->_id == id) {
            return link;
        }
    }
    for (auto link:_parallelGateways) {
        if (link->_id == id) {
            return link;
        }
    }
    for (auto link:_eventBasedGateways) {
        if (link->_id == id) {
            return link;
        }
    }
    return nullptr;
}

std::pair<std::string, std::string> parseRule(std::string value) {
    auto subStrs = split(value, '=');
    if (subStrs.size() == 2)
        return {split(split(subStrs[0], '{')[0], '}')[0], subStrs[1]};
    else {
        return {};
    }
}

void DummyProcess::constructTaskFlow() {
    for (auto &flow:_flows) {
        std::shared_ptr<Link> source = getLink(flow->_sourceRef);
        std::shared_ptr<Link> target = getLink(flow->_targetRef);
        if (source && target) {
            source->_nextTaskID = flow->_targetRef;
            target->_preTaskID = flow->_sourceRef;
        }
        {
            std::shared_ptr<DummyParallelGateway> parallelGateway = std::dynamic_pointer_cast<DummyParallelGateway>(
                    source);
            if (parallelGateway) {
                parallelGateway->_outPrallelRules.push_back(flow->_targetRef);
            }
            parallelGateway = std::dynamic_pointer_cast<DummyParallelGateway>(target);
            if (parallelGateway) {
                parallelGateway->_inParallelRules.push_back(flow->_sourceRef);
            }
        }
        {
            std::shared_ptr<DummyExclusiveGateway> exclusiveGateway = std::dynamic_pointer_cast<DummyExclusiveGateway>(
                    source);
            if (exclusiveGateway) {
                exclusiveGateway->_exclusiveRules.insert(parseRule(flow->_conditionExpression));
            }
        }
        {
            std::shared_ptr<DummyEventBasedGateway> eventBasedGateway = std::dynamic_pointer_cast<DummyEventBasedGateway>(
                    source);
            if (eventBasedGateway) {
                auto subStrs = split(flow->_conditionExpression, '=');
                eventBasedGateway->_eventRules.insert({flow->_conditionExpression, flow->_targetRef});
            }
        }
    }
}

std::shared_ptr<DummyEventBasedGateway> parseEventBasedGateway(xercesc::DOMNode *node) {
    DummyEventBasedGateway eventBasedGateway;
    eventBasedGateway._id = getAttributeByName(node, "id");
    eventBasedGateway._name = getAttributeByName(node, "name");
    return std::make_shared<DummyEventBasedGateway>(eventBasedGateway);
}

std::shared_ptr<DummyProcess> parseProcess(xercesc::DOMNode *pNode) {
    auto startEvents = getElementsByName(pNode, "startEvent");
    std::shared_ptr<DummyProcess> ret = std::make_shared<DummyProcess>();
    ret->_startEvent = parseStartEvent(startEvents.at(0));
    auto endEvents = getElementsByName(pNode, "endEvent");
    ret->_endEvent = parseEndEvent(endEvents.at(0));
    auto primitiveTasks = getElementsByName(pNode, "userTask");
    for (auto &task:primitiveTasks) {
        ret->_tasks.push_back(parsePrimitiveTask(task));
    }
    auto sequenceFlows = getElementsByName(pNode, "sequenceFlow");
    for (auto &sequenceFlow:sequenceFlows) {
        ret->_flows.push_back(parseSequenceFlow(sequenceFlow));
    }
    auto subProcesses = getElementsByName(pNode, "subProcess");
    for (auto &subProcess:subProcesses) {
        ret->_subProcess.push_back(parseProcess(subProcess));
    }
    auto exclusiveGateways = getElementsByName(pNode, "exclusiveGateway");
    for (auto &exclusiveGateway:exclusiveGateways) {
        ret->_exclusiveGateways.push_back(parseExclusiveGateway(exclusiveGateway));
    }
    auto parallelGateways = getElementsByName(pNode, "parallelGateway");
    for (auto &parallelGateway:parallelGateways) {
        ret->_parallelGateways.push_back(parseParallelGateway(parallelGateway));
    }
    auto eventBasedGateways = getElementsByName(pNode, "eventBasedGateway");
    for (auto &eventBasedGateway:eventBasedGateways) {
        ret->_eventBasedGateways.push_back(parseEventBasedGateway(eventBasedGateway));
    }
    ret->constructTaskFlow();
    return ret;
}

std::vector<std::string> split(const std::string &s, char seperator) {
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while ((pos = s.find(seperator, pos)) != std::string::npos) {
        std::string substring(s.substr(prev_pos, pos - prev_pos));
        boost::trim_right(substring);
        if (!substring.empty()) {
            output.push_back(substring);
        }
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos - prev_pos));
    return output;
}

std::shared_ptr<PrimitiveProcess> parseFromFlowableBpmn(std::string file) {
    std::string xmlContent = readFile(file);
    xercesc::XMLPlatformUtils::Initialize();
    xercesc::XercesDOMParser xercesDOMParser;
    std::shared_ptr<xercesc::MemBufInputSource> memBufIS = std::shared_ptr<xercesc::MemBufInputSource>(
            new xercesc::MemBufInputSource((const XMLByte *) xmlContent.data(), xmlContent.size(), ""));
    xercesDOMParser.parse(*memBufIS);
    auto document = xercesDOMParser.getDocument();
    auto root = document->getDocumentElement();
    auto definitions = document->getElementsByTagName(XStr("definitions"));
    auto processes = getElementsByName(definitions->item(0), "process");
    return createProcess(parseProcess(processes.at(0)));
}

std::shared_ptr<StartTask> createTask(std::shared_ptr<DummyStartEvent> dummyStartEvent) {
    std::shared_ptr<StartTask> startTask = std::make_shared<StartTask>();
    startTask->_id = dummyStartEvent->_id;
    startTask->_name = dummyStartEvent->_name;
    startTask->_preTaskID = dummyStartEvent->_preTaskID;
    startTask->_nextTaskID = dummyStartEvent->_nextTaskID;
    return startTask;
}

std::shared_ptr<EndTask> createTask(std::shared_ptr<DummyEndEvent> dummyStartEvent) {
    std::shared_ptr<EndTask> endTask = std::make_shared<EndTask>();
    endTask->_id = dummyStartEvent->_id;
    endTask->_name = dummyStartEvent->_name;
    endTask->_preTaskID = dummyStartEvent->_preTaskID;
    endTask->_nextTaskID = dummyStartEvent->_nextTaskID;
    return endTask;
}

std::shared_ptr<ParallelGateway> createTask(std::shared_ptr<DummyParallelGateway> parallelGateway) {
    std::shared_ptr<ParallelGateway> parallelGateway1 = std::make_shared<ParallelGateway>();
    parallelGateway1->_id = parallelGateway->_id;
    parallelGateway1->_inTasks = parallelGateway->_inParallelRules;
    parallelGateway1->_outTasks = parallelGateway->_outPrallelRules;
    parallelGateway1->_taskName = parallelGateway->_name;
    parallelGateway1->_preTaskID = parallelGateway->_preTaskID;
    parallelGateway1->_nextTaskID = parallelGateway->_nextTaskID;
    return parallelGateway1;
}

std::shared_ptr<EventGateway> createTask(std::shared_ptr<DummyEventBasedGateway> dummyEndEvent) {
    std::shared_ptr<EventGateway> eventGateway = std::make_shared<EventGateway>();
    eventGateway->_id = dummyEndEvent->_id;
    eventGateway->_name = dummyEndEvent->_name;
    eventGateway->_eventRules = dummyEndEvent->_eventRules;
    eventGateway->_preTaskID = dummyEndEvent->_preTaskID;
    eventGateway->_nextTaskID = dummyEndEvent->_nextTaskID;
    return eventGateway;
}

std::shared_ptr<ExclusiveGateway> createTask(std::shared_ptr<DummyExclusiveGateway> dummyExclusiveGateway) {
    std::shared_ptr<ExclusiveGateway> exclusiveGateway = std::make_shared<ExclusiveGateway>();
    exclusiveGateway->_id = dummyExclusiveGateway->_id;
    exclusiveGateway->_name = dummyExclusiveGateway->_name;
    exclusiveGateway->_rules = dummyExclusiveGateway->_exclusiveRules;
    exclusiveGateway->_preTaskID = dummyExclusiveGateway->_preTaskID;
    exclusiveGateway->_nextTaskID = dummyExclusiveGateway->_nextTaskID;
    return exclusiveGateway;
}

std::shared_ptr<Process::Task> createTask(std::shared_ptr<PrimitiveTask> dummyTask) {
    auto type = rttr::type::get_by_name(dummyTask->_name);
    if (type.is_valid()) {
        auto taskVariant = type.create();
        for (auto &parameter:dummyTask->_parameters) {
            if (parameter._type == "int") {
                taskVariant.get_type().get_property(parameter._id).set_value(taskVariant, boost::lexical_cast<int>(
                        parameter._value));
            } else if (parameter._type == "double") {
                taskVariant.get_type().get_property(parameter._id).set_value(taskVariant, boost::lexical_cast<double>(
                        parameter._value));
            } else if (parameter._type == "long") {
                taskVariant.get_type().get_property(parameter._id).set_value(taskVariant, boost::lexical_cast<long>(
                        parameter._value));
            } else if (parameter._type == "string") {
                taskVariant.get_type().get_property(parameter._id).set_value(taskVariant, parameter._value);
            } else if (parameter._type == "float") {
                taskVariant.get_type().get_property(parameter._id).set_value(taskVariant, boost::lexical_cast<float>(
                        parameter._value));
            }
        }
        return taskVariant.get_value<std::shared_ptr<Process::Task>>();
    }
    return {};
}

std::shared_ptr<PrimitiveProcess> createProcess(std::shared_ptr<DummyProcess> dummyProcess) {
    std::shared_ptr<PrimitiveProcess> primitiveProcess = std::make_shared<PrimitiveProcess>();
    auto startEvent = createTask(dummyProcess->_startEvent);
    if (!startEvent) {
        LOG(FATAL) << " null start task";
    }
    primitiveProcess->_tasks.push_back(startEvent);
    auto endEvent = createTask(dummyProcess->_endEvent);
    if (!endEvent) {
        LOG(FATAL) << " null end task";
    }
    for (auto dumyyTask:dummyProcess->_tasks) {
        auto task = createTask(dumyyTask);
        primitiveProcess->_tasks.push_back(task);
    }
    for (auto gateway:dummyProcess->_exclusiveGateways) {
        auto way = createTask(gateway);
        primitiveProcess->_tasks.push_back(way);
    }
    for (auto gateway:dummyProcess->_parallelGateways) {
        auto way = createTask(gateway);
        primitiveProcess->_tasks.push_back(way);
    }
    for (auto gateway:dummyProcess->_eventBasedGateways) {
        auto way = createTask(gateway);
        primitiveProcess->_tasks.push_back(way);
    }
    for (auto &subP:dummyProcess->_subProcess) {
        primitiveProcess->_subProcess.push_back(createProcess(subP));
    }
    return primitiveProcess;
}

//std::shared_ptr<Process::Process> createProcess( std::vector<std::shared_ptr<Process::Task>> _tasks,int threadCount){
//    std::shared_ptr<Process::ProcessContext> processContext = std::make_shared<Process::ProcessContext>(threadCount);
//    processContext->_tasks = _tasks;
//    return std::make_shared<Process::Process>(processContext);
//}

std::shared_ptr<Process::Process> parseProcess(std::shared_ptr<PrimitiveProcess> primitiveProcess,int threadCount) {
    std::shared_ptr<Process::ProcessContext> processContext = std::make_shared<Process::ProcessContext>(threadCount);
    processContext->_tasks = primitiveProcess->_tasks;
    for(auto & sub:primitiveProcess->_subProcess){
       auto process = parseProcess(sub,threadCount);
       processContext->_tasks.push_back(std::shared_ptr<Process::Task>(new SubProcessTaskShape(process)));
    }
    return std::make_shared<Process::Process>(processContext);
}

std::shared_ptr<Process::Process> createProcessFromFlowableBpmn(std::string file, int threadCount) {
    auto primitiveProcess = parseFromFlowableBpmn("/home/xuzhenhai/puppy/Process/Hatch/flowable.bpmn20.xml");


}

int main() {
    auto task = parseFromFlowableBpmn("/home/xuzhenhai/puppy/Process/Hatch/flowable.bpmn20.xml");;
    LOG(INFO) << task;
    auto process = parseProcess(task,3);
    LOG(INFO)<<process;

//    std::string text = "{B}==\"B\"";
//    auto subStrs = split(text, '=');
//    LOG(INFO) << split(split(subStrs[0], '{')[0], '}')[0] << subStrs[1];
}