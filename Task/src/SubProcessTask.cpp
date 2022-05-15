//
// Created by xuzhenhai on 2021/10/26.
//

#include "SubProcessTask.h"
#include "XML.h"
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

SubProcessTaskShape::SubProcessTaskShape() {
    _subProcess = std::make_shared<Process::Process>(1);
}

SubProcessTaskShape::SubProcessTaskShape(std::shared_ptr<Process::Process> subProcess) {
    _subProcess = subProcess;
}

void SubProcessTaskShape::run(std::shared_ptr<Process::ProcessContext> context) {
    _subProcess->startProcess(context, true);
}

bool SubProcessTaskShape::initTask(Process::ProcessContext *manager) {
    _subProcess->getProcessContext()->_executor = manager->_executor;
    return true;
}

void SubProcessTaskShape::setTaskFinishFunction(std::function<void(void)> taskFinishFunction) {
    _taskFinishFunction = taskFinishFunction;
    _subProcess->getProcessContext()->_taskFinishFunction = _taskFinishFunction;
}

bool SubProcessTaskShape::saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document) {
    auto taskManager = document->createElement(XStr("SubTaskManager"));
    taskManager->setAttribute(XStr("taskName"), XStr(_subProcess->getProcessContext()->getTaskName().data()));
    domElement->appendChild(taskManager);
    _subProcess->getProcessContext()->saveDomElement(taskManager, document);
    return true;
}

bool SubProcessTaskShape::loadDomElement(xercesc::DOMNode *domElement) {
    int n = domElement->getChildNodes()->getLength();
    for (int i = 0; i < n; ++i) {
        auto nodeName = puppy::common::XML::toStr(domElement->getChildNodes()->item(i)->getNodeName());
        if (nodeName == "SubTaskManager"||nodeName == "TaskManager") {
            auto taskName = domElement->getChildNodes()->item(i)->getAttributes()->getNamedItem(XStr("taskName"));
            _subProcess->getProcessContext()->_taskName=puppy::common::XML::toStr(taskName->getNodeValue());
            _subProcess->getProcessContext()->loadDomElement(domElement->getChildNodes()->item(i));
        }
    }
    return true;
}

std::string SubProcessTaskShape::getTaskType() {
    return "SubProcessTask";
}
