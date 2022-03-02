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
            std::shared_ptr<DummyParallelGateway> parallelGateway = std::dynamic_pointer_cast<DummyParallelGateway>(source);
            if (parallelGateway) {
                parallelGateway->_outPrallelRules.push_back(flow->_targetRef);
            }
            parallelGateway = std::dynamic_pointer_cast<DummyParallelGateway>(target);
            if (parallelGateway) {
                parallelGateway->_inParallelRules.push_back(flow->_sourceRef);
            }
        }
        {
            std::shared_ptr<DummyExclusiveGateway> exclusiveGateway = std::dynamic_pointer_cast<DummyExclusiveGateway>(source);
            if (exclusiveGateway) {
                exclusiveGateway->_exclusiveRules.insert(parseRule(flow->_conditionExpression));
            }
        }
        {
            std::shared_ptr<DummyEventBasedGateway> eventBasedGateway = std::dynamic_pointer_cast<DummyEventBasedGateway>(source);
            if (eventBasedGateway){
                auto subStrs = split(flow->_conditionExpression, '=');
                eventBasedGateway->_eventRules.insert({flow->_conditionExpression,flow->_targetRef});
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

std::shared_ptr<DummyProcess> parseFromFlowableBpmn(std::string file) {
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
    return parseProcess(processes.at(0));
}


int main() {
    LOG(INFO) << parseFromFlowableBpmn("/home/xuzhenhai/puppy/Process/Hatch/flowable.bpmn20.xml");
//    std::string text = "{B}==\"B\"";
//    auto subStrs = split(text, '=');
//    LOG(INFO) << split(split(subStrs[0], '{')[0], '}')[0] << subStrs[1];
}