//
// Created by xuzhenhai on 2021/10/21.
//

#pragma once

#include <xercesc/dom/DOMElement.hpp>
#include "Task.h"
#include "ProcessContext.h"

namespace Process {
    struct AbstractTask : public Task {
    public:
        AbstractTask();

        std::string getID() override;

        std::string getName() override;

        std::string getNextTaskID() override;

        std::string getPreTaskID() override;

        //must
        void run(std::shared_ptr<ProcessContext> context) override;

        virtual bool saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document) {
            return false;
        };

        virtual bool initTask(ProcessContext *processContext) {
            return false;
        }

        virtual bool loadDomElement(xercesc::DOMNode *domElement) {
            return false;
        };

        std::string getTaskType() override;
    RTTR_ENABLE(Task)
    };
}


