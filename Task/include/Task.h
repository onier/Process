//
// Created by xuzhenhai on 2021/10/21.
//

#pragma once

#include <string>
#include "rttr/registration.h"
#include "folly/Synchronized.h"
#include "boost/any.hpp"
#include <xercesc/dom/DOMElement.hpp>

//#include "TaskManager.h"
typedef std::function<void(void)> ExceptionHandler;
namespace Process {
    class ProcessContext;

    /**
     * task异常基类
     */
    class TaskException : public std::exception {
        TaskException();

    public:
        TaskException(const std::string message);

        virtual ~TaskException() {};

    public:
        std::string _type;
        std::string _message;

        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
    };

    /**
     * 重试异常
     */
    class TaskRuntimeException : public TaskException {
    public:
        TaskRuntimeException(const std::string type, const std::string message);

        ~TaskRuntimeException() override;

        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
    };

    /**
     * process重新启动
     */
    class TaskError : public TaskException {
    public:
        TaskError(const std::string type);

        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
    };

    class UserDefinedExeception : public TaskException {
    public:
        UserDefinedExeception(const std::string &message);

        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
    };

    struct Task {
        typedef std::function<void(std::string, boost::any, std::string)> MessageEvent;

        Task();

        virtual ~Task();

        /**
         * 初始化任务，当任务对象从配置从创建出来之后就会调用改函数一次，确保所有的任务都被初始化。
         * @param manager 任务管理器对象
         * @return 是否初始化成功
         */
        virtual bool initTask(ProcessContext *processContext) = 0;

        /**
         * 每一个task的执行函数。
         * @param values 为当前taskmanager中存放的全部对象。folly::Synchronized提供了便捷的读写锁极致保证线程安全。
         */
        virtual void run(std::shared_ptr<ProcessContext> context) = 0;

        /**
         * 当前task id
         * @return
         */
        virtual std::string getID() = 0;

        /**
         * 名称
         * @return
         */
        virtual std::string getName() = 0;

        /**
         * 下一个task的id，当前task结束之后寻找下一个任务，如果找不到就视为Process结束。
         * @return 下一个task的id
         */
        virtual std::string getNextTaskID() = 0;

        virtual void setNextTaskID(std::string id, bool f = false) = 0;

        /**
         * 上一个task,目前只用于判断第一个startTask
         * @return 字符串
         */
        virtual std::string getPreTaskID() = 0;

        virtual void setPreTaskID(std::string id, bool f = false) = 0;

        virtual bool
        saveDomElement(xercesc::DOMElement *domElement, xercesc::DOMDocument *document) = 0;

        virtual bool loadDomElement(xercesc::DOMNode *domElement) = 0;

        virtual std::string getTaskType() = 0;

        virtual void addMessageEvent(MessageEvent event) = 0;

        virtual void notify(std::string message, boost::any any, std::string content) = 0;

        std::string _id;

        std::string _taskName;
        std::string _taskType;
        std::string _name;
        std::string _nextTaskID;
        std::string _preTaskID;
    RTTR_ENABLE()
    };
}


