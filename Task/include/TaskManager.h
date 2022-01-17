//
// Created by xuzhenhai on 2021/10/25.
//

#ifndef PUPPY_TASKMANAGER_H
#define PUPPY_TASKMANAGER_H

#include <xercesc/dom/DOMElement.hpp>
#include "memory"
#include "Task.h"
#include "AbstractTask.h"

namespace Process {
    struct TaskGroup {
        std::vector<std::string> _tasks;

        TaskGroup();

        TaskGroup(const std::vector<std::string> &tasks);

    RTTR_ENABLE()
    };

    class TaskManager : public std::enable_shared_from_this<TaskManager> {
    public:
        std::shared_ptr<Process::AbstractTask> getStartTask();

        std::vector<std::shared_ptr<Process::AbstractTask>> _tasks;

        std::shared_ptr<Process::AbstractTask> getTaskByID(std::string id);

        bool initTasks();

        std::string saveXML();

        bool loadXML(std::string value);

        bool saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document);

        bool loadDomElement(xercesc::DOMNode *domElement);

    private:
        void createElement(rttr::instance obj2, xercesc::DOMElement *domElement, xercesc::DOMDocument *document,
                           std::shared_ptr<AbstractTask> task = nullptr);
    };
}

#endif //PUPPY_TASKMANAGER_H
