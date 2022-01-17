//
// Created by xuzhenhai on 2021/10/26.
//

#pragma once

#include "AbstractTask.h"
#include "TaskManager.h"
#include "Process.h"

class SubProcessTask : public Process::AbstractTask {
public:
    SubProcessTask();

    void run(folly::Synchronized<std::map<std::string, boost::any>> &values) override;

    bool initTask(std::shared_ptr<Process::TaskManager> manager) override;

    void setTaskFinishFunction(std::function<void(void)> taskFinishFunction);

    std::string _subTaskProcess;
    std::shared_ptr<Process::Process> _subProcess;
    std::shared_ptr<Process::TaskManager> _subTaskManager;
    std::function<void(void)> _taskFinishFunction;

    bool saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document);

    bool loadDomElement(xercesc::DOMNode *domElement);

RTTR_ENABLE(Process::AbstractTask);
};

