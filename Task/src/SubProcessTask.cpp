//
// Created by xuzhenhai on 2021/10/26.
//

#include "SubProcessTask.h"
#include "XML.h"
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

SubProcessTask::SubProcessTask() {;
}

void SubProcessTask::run(std::shared_ptr<Process::ProcessContext> context) {
    _subProcess->startProcess(context,true);
}

bool SubProcessTask::initTask(Process::ProcessContext * manager) {
    _subProcess = std::make_shared<Process::Process>(1);
    _subProcess->getProcessContext()->_executor=manager->_executor;
    return true;
}

void SubProcessTask::setTaskFinishFunction(std::function<void(void)> taskFinishFunction) {
    _taskFinishFunction = taskFinishFunction;
    _subProcess->getProcessContext()->_taskFinishFunction = _taskFinishFunction;
}

bool SubProcessTask::saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document) {
    auto taskManager = document->createElement(XStr("SubTaskManager"));
    taskManager->setAttribute(XStr("taskName"), XStr(_subProcess->getProcessContext()->getTaskName().data()));
    domElement->appendChild(taskManager);
    _subProcess->getProcessContext()->saveDomElement(taskManager, document);
    return true;
}

bool SubProcessTask::loadDomElement(xercesc::DOMNode *domElement) {
    int n = domElement->getChildNodes()->getLength();
    for (int i = 0; i < n; ++i) {
        auto nodeName = puppy::common::XML::toStr(domElement->getChildNodes()->item(i)->getNodeName());
        if (nodeName == "SubTaskManager") {
            LOG(INFO)<<puppy::common::XML::toStr(domElement->getChildNodes()->item(i)->getAttributes()->getNamedItem(XStr("taskName"))->getNodeValue());
            _subProcess->getProcessContext()->loadDomElement(domElement->getChildNodes()->item(i));
        }
    }
    return true;
}
