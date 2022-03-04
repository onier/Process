//
// Created by xuzhenhai on 2022/2/19.
//

#ifndef PUPPY_PROCESSCONTEXT_H
#define PUPPY_PROCESSCONTEXT_H

#include "vector"
#include "memory"
#include "Task.h"
#include "folly/Synchronized.h"
#include "boost/any.hpp"
#include "folly/executors/CPUThreadPoolExecutor.h"
#include "folly/futures/Future.h"
#include "atomic"

namespace Process {
    class ProcessContext : public std::enable_shared_from_this<ProcessContext> {
    public:
        ProcessContext(int threadCount);

        void loadXML(std::string xml);

        std::string saveXML();

        std::vector<std::shared_ptr<Process::Task>> getTasksByType(std::string type);

        std::shared_ptr<Process::Task> getStartTask();

        std::vector<std::shared_ptr<Process::Task>> getEndTask();

        std::shared_ptr<Process::Task> getTaskByID(std::string id);

        std::vector<std::shared_ptr<Process::Task>> getPreTaskByID(std::string id);

        bool saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document);

        bool loadDomElement(xercesc::DOMNode *domElement);

        void notifyEvent(std::string eventType, Process::Task *task);

    public:
        auto getExecutor() {
            return _executor;
        }

        auto getProcessValues() {
            return _processValues;
        }

        auto getTaskName() {
            return _taskName;
        }

        auto getTasks() {
            return _tasks;
        }

        std::function<void(void)> _taskFinishFunction;
        //0 default 1 start 2 retry 3 reset
        std::atomic_int _status;
    private:
        bool initTasks();

        bool checkProcessTasks();

        void createElement(rttr::instance obj2, xercesc::DOMElement *domElement, xercesc::DOMDocument *document,
                           std::shared_ptr<Task> task = nullptr);
    public:
        std::shared_ptr<folly::Synchronized<std::map<std::string, boost::any>>> _processValues;
        std::shared_ptr<folly::CPUThreadPoolExecutor> _executor;
        std::shared_ptr<std::map<std::string, std::function<void()>>> _eventHandler;
        std::string _taskName;
        std::vector<std::shared_ptr<Process::Task>> _tasks;
    };
}

#endif //PUPPY_PROCESSCONTEXT_H
