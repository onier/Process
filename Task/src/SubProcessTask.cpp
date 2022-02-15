//
// Created by xuzhenhai on 2021/10/26.
//

#include "SubProcessTask.h"
#include "XML.h"
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

SubProcessTask::SubProcessTask() {
    _subTaskManager = std::make_shared<Process::TaskManager>();
}

void SubProcessTask::run(folly::Synchronized<std::map<std::string, boost::any>> &values) {
    _subProcess->startProcess(values);
}

bool SubProcessTask::initTask(std::shared_ptr<Process::TaskManager> manager) {
    _subTaskManager->initTasks();
    _subProcess = std::make_shared<Process::Process>(_subTaskManager);
    return true;
}

void SubProcessTask::setTaskFinishFunction(std::function<void(void)> taskFinishFunction) {
    _taskFinishFunction = taskFinishFunction;
    _subProcess->_taskFinishFunction = _taskFinishFunction;
}

bool SubProcessTask::saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document) {
    auto taskManager = document->createElement(XStr("SubTaskManager"));
    taskManager->setAttribute(XStr("taskName"), XStr(_subTaskManager->_taskName.data()));
    domElement->appendChild(taskManager);
    _subTaskManager->saveDomElement(taskManager, document);
    return true;
}

bool SubProcessTask::loadDomElement(xercesc::DOMNode *domElement) {
    int n = domElement->getChildNodes()->getLength();
    for (int i = 0; i < n; ++i) {
        auto nodeName = puppy::common::XML::toStr(domElement->getChildNodes()->item(i)->getNodeName());
        if (nodeName == "SubTaskManager") {
            LOG(INFO)<<puppy::common::XML::toStr(domElement->getChildNodes()->item(i)->getAttributes()->getNamedItem(XStr("taskName"))->getNodeValue());
            _subTaskManager->loadDomElement(domElement->getChildNodes()->item(i));
        }
    }
    return true;
}
