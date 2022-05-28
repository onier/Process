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
#include "xercesc/parsers/XercesDOMParser.hpp"

namespace Process {
    typedef folly::Synchronized<std::vector<std::shared_ptr<Process::Task>>> SuspendTask;

    class ProcessContext : public std::enable_shared_from_this<ProcessContext> {
    public:
        ProcessContext(int threadCount);

        void loadXML(std::string xml);

        void loadFile(std::string file);

        std::string saveXML();

        void saveDocumentElemenet(std::shared_ptr<xercesc::DOMDocument> document, xercesc::DOMElement *rootElement);

        std::vector<std::shared_ptr<Process::Task>> getTasksByType(std::string type);

        std::shared_ptr<Process::Task> getStartTask();

        std::vector<std::shared_ptr<Process::Task>> getEndTask();

        std::shared_ptr<Process::Task> getTaskByID(std::string id);

        std::vector<std::shared_ptr<Process::Task>> getPreTaskByID(std::string id);

        bool saveDomElement(xercesc::DOMElement *domElement, xercesc::DOMDocument *document);

        bool loadDomElement(xercesc::DOMNode *domElement);

        void notifyEvent(std::string eventType, Process::Task *task);

        void addTask(std::shared_ptr<Task> task);

        void setTaskName(std::string taskName) {
            _taskName = taskName;
            for (auto &t: _tasks) {
                t->_taskName = taskName;
            }
        };

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

        std::vector<xercesc::DOMNode *> getSubProcessTasks(xercesc::DOMNode *subProcessNode);

        auto getElementsByName(xercesc::DOMNode *parent, std::string name);

        std::string _currentFilePath;
    public:
        std::shared_ptr<folly::Synchronized<std::map<std::string, boost::any>>> _processValues;
        std::shared_ptr<folly::CPUThreadPoolExecutor> _executor;
        std::shared_ptr<std::map<std::string, std::function<void()>>> _eventHandler;
        int _threadCount;
        std::vector<std::shared_ptr<Process::Task>> _tasks;
        std::shared_ptr<SuspendTask> _suspendTasks;
    private:
        std::vector<std::shared_ptr<xercesc::XercesDOMParser>> _parseCaches;
        std::string _taskName;
    };
}

#endif //PUPPY_PROCESSCONTEXT_H
