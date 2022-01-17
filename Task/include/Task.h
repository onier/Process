//
// Created by xuzhenhai on 2021/10/21.
//

#pragma once

#include <string>
#include "rttr/registration.h"
#include "folly/Synchronized.h"
#include "boost/any.hpp"
//#include "TaskManager.h"

namespace Process {
    class TaskManager;

    class TaskException : public std::exception {
        TaskException();

    public:
        TaskException(const std::string &type);

    private:
        ~TaskException();

    public:
        std::string _type;

        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
    };

    struct Task {
        Task();

        virtual ~Task();

        virtual bool initTask(std::shared_ptr<TaskManager> manager) = 0;

        virtual void run(folly::Synchronized<std::map<std::string, boost::any>> &values) = 0;

        virtual std::string getID() = 0;

        virtual std::string getName() = 0;

        virtual std::string getNextTaskID() = 0;

        virtual std::string getPreTaskID() = 0;
    };
}


