//
// Created by xuzhenhai on 2021/10/21.
//

#include "Process.h"
#include "ParallelGateway.h"
#include "SubProcessTask.h"

Process::Process::~Process() {

}

Process::Process::Process(const std::shared_ptr<TaskManager> &taskManager) : _taskManager(taskManager) {
    _status = 0;
}

void Process::Process::processTask(std::shared_ptr<AbstractTask> task) {
    if (task->get_type().get_name() == "ParallelGateway") {
        processParallelTask(task);
    } else if (task->get_type().get_name() == "SubProcessTask") {
//        std::shared_ptr<SubProcessTask> subProcessTask = std::dynamic_pointer_cast<SubProcessTask>(task);
        processSubProcessTask(task);
    } else {
        processDefaultTask(task);
    }
}

void Process::Process::processSubProcessTask(std::shared_ptr<AbstractTask> task1) {
    std::shared_ptr<SubProcessTask> task = std::dynamic_pointer_cast<SubProcessTask>(task1);
    task->setTaskFinishFunction([&, task]() {
        auto t = _taskManager->getTaskByID(task->getNextTaskID());
        if (t) {
            processTask(t);
        }
    });
    folly::via(&executor, std::bind([&, task]() {
        task->run(_processValues);
    }));
}

void Process::Process::processParallelTask(std::shared_ptr<AbstractTask> task1) {
    std::shared_ptr<ParallelGateway> task = std::dynamic_pointer_cast<ParallelGateway>(task1);
//    LOG(INFO) << "processParallelTask " << task->_name << "  " << task->_taskCount;
    folly::via(&executor, std::bind([&, task]() {
        task->run(_processValues);
    })).then(std::bind([&, task]() {
        std::lock_guard<std::mutex> gurad{task->_mutex};
        {
            task->_taskCount++;
//            LOG(INFO) << "processParallelTask  then " << task->_name << "  " << task->_taskCount << " intask "
//                      << task->_inTasks.size();
            if (task->_inTasks.empty()) {//作为开始并发的任务直接启动并发任务
                for (auto subTask: task->_outTasks) {
                    auto t = _taskManager->getTaskByID(subTask);
                    processTask(t);
                }
            } else if (task->_taskCount == task->_inTasks.size()) {//并发的任务已经全部结束继续往下执行
                auto t = _taskManager->getTaskByID(task->getNextTaskID());
//                LOG(INFO) << "start ParallelGateway next task  " << task->_name << " " << task->_taskCount;
                if (t) {
                    processTask(t);
                } else {
                    LOG(INFO) << "  ParallelGateway can not find next task";
                    if (_taskFinishFunction) {
                        _taskFinishFunction();
                    }
                }
            }
        }
    })).thenError(folly::tag_t<std::exception>{}, [](std::exception const &e) {
        LOG(ERROR) << e.what();
    });
}

void Process::Process::startProcess(folly::Synchronized<std::map<std::string, boost::any>> &values) {
    _processValues = values;
    _status = 1;
    processTask(_taskManager->getStartTask());
}

void Process::Process::processDefaultTask(std::shared_ptr<AbstractTask> task) {
//    LOG(INFO)<<task->getName() <<"  "<<task->get_type().get_name();
    if (task) {
        folly::via(&executor, std::bind([&, task]() {
            try {
                task->run(_processValues);
                _status = 1;
            } catch (TaskRuntimeException &ex) {
                _status = 2;
                LOG(ERROR) << task->getName() << " " << task->getID() << " retry again";
                processTask(task);
            } catch (TaskError &err) {
                _status = 3;
                LOG(ERROR) << task->getName() << " " << task->getID() << " restart process";
                processTask(_taskManager->getStartTask());
            } catch (UserDefinedExeception &ex) {
                boost::any any = _processValues.rlock()->find(ex._type);
                ExceptionHandler exceptionHandler = boost::any_cast<ExceptionHandler>(any);
                if (exceptionHandler) {
                    exceptionHandler();
                }
            }
        })).then(std::bind([&, task]() {
            if (_status == 1) {
//            LOG(INFO) << "processDefaultTask then " << task->_name <<" "<<task->getNextTaskID();
                auto t = _taskManager->getTaskByID(task->getNextTaskID());
                if (t) {
                    processTask(t);
                } else if (task->get_type().get_name() == "EndTask") {
                    if (_taskFinishFunction) {
                        _taskFinishFunction();
                    }
                } else {
                    LOG(ERROR) << "error this should not happend";
                }
            }
        })).thenError(folly::tag_t<TaskRuntimeException>{}, [](TaskRuntimeException const &e) {
            LOG(ERROR) << e.what();
        });
    } else {
        LOG(ERROR) << " can not process null task";
    }
}

