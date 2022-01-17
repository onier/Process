//
// Created by xuzhenhai on 2021/10/21.
//

#pragma once

#include <xercesc/dom/DOMElement.hpp>
#include "Task.h"

namespace Process {
    struct AbstractTask : public Process::Task {
    public:
        AbstractTask();

        std::string getID() override;

        std::string getName() override;

        std::string getNextTaskID() override;

        std::string getPreTaskID() override;

        //must
        bool initTask(std::shared_ptr<TaskManager> manager) override;

        //must
        void run(folly::Synchronized<std::map<std::string, boost::any>> &values) override;

        virtual bool saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document) {
            return false;
        };

        virtual bool loadDomElement(xercesc::DOMNode *domElement) {
            return false;
        };
    RTTR_ENABLE()
    public:
        std::string _id;
        std::string _name;
        std::string _nextTaskID;
        std::string _preTaskID;
    };
}


