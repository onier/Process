//
// Created by xuzhenhai on 2021/10/26.
//

#pragma once

#include "AbstractTask.h"
#include "Process.h"

class SubProcessTask : public Process::AbstractTask {
public:
    SubProcessTask();

    SubProcessTask(std::shared_ptr<Process::Process> subProcess);

    void run(std::shared_ptr<Process::ProcessContext> context) override;

    bool initTask(Process::ProcessContext * context) override;

    void setTaskFinishFunction(std::function<void(void)> taskFinishFunction);

//    std::string _subTaskProcess;
    std::shared_ptr<Process::Process> _subProcess;
    std::function<void(void)> _taskFinishFunction;

    bool saveDomElement(xercesc::DOMElement *domElement, xercesc::DOMDocument *document);

    bool loadDomElement(xercesc::DOMNode *domElement);

    std::string getTaskType() override;

    std::string _processFile;

RTTR_ENABLE(Process::AbstractTask);
};

