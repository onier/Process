//
// Created by xuzhenhai on 2021/10/21.
//

#include "Process.h"
#include "ParallelGateway.h"
#include "SubProcessTask.h"
#include "EventGateway.h"

Process::Process::~Process() {

}

Process::Process::Process(int threadCount) {
    _processContext = std::make_shared<ProcessContext>(threadCount);
    _processContext->_status = 0;
}

Process::Process::Process(std::shared_ptr<ProcessContext> processContext) {
    _processContext = processContext;
    _processContext->_status = 0;
}

void Process::Process::processTask(std::shared_ptr<Task> task) {
    notityTaskEvent(task);
    auto state = getState();
    switch (state) {
        case State::STOPED:
            LOG(ERROR) << "process is stop";
            return;
        case State::SUSPEND:
            LOG(ERROR) << "process is suspend";
            _processContext->_suspendTasks->wlock()->push_back(task);
            return;
    }
    if (task->get_type().get_name() == "ParallelGateway") {
        processParallelTask(task);
    } else if (task->get_type().get_name() == "SubProcessTask") {
//        std::shared_ptr<SubProcessTask> subProcessTask = std::dynamic_pointer_cast<SubProcessTask>(task);
        processSubProcessTask(task);
    } else {
        processDefaultTask(task);
    }
}

void Process::Process::processSubProcessTask(std::shared_ptr<Task> task1) {
    std::shared_ptr<SubProcessTask> task = std::dynamic_pointer_cast<SubProcessTask>(task1);
    task->setTaskFinishFunction([&, task]() {
        auto t = _processContext->getTaskByID(task->getNextTaskID());
        if (t) {
            processTask(t);
        }
    });
    folly::via(_processContext->getExecutor().get(), std::bind([&, task]() {
        task->run(_processContext);
    }));
}

void Process::Process::processParallelTask(std::shared_ptr<Task> task1) {
    std::shared_ptr<ParallelGateway> task = std::dynamic_pointer_cast<ParallelGateway>(task1);
//    LOG(INFO) << "processParallelTask " << task->_name << "  " << task->_taskCount;
    folly::via(_processContext->getExecutor().get(), std::bind([&, task]() {
        task->run(_processContext);
    })).then(std::bind([&, task]() {
        std::lock_guard<std::mutex> gurad{task->_mutex};
        {
            task->_taskCount++;
//            if (task->_inTasks.size()<2) {//作为开始并发的任务直接启动并发任务
//                for (auto subTask: task->_outTasks) {
//                    auto t = _processContext->getTaskByID(subTask);
//                    processTask(t);
//                }
//            } else
            if (task->_taskCount >= task->_inTasks.size()) {//并发的任务已经全部结束继续往下执行
                task->_taskCount = 0;
                for (auto subTask: task->_outTasks) {
                    auto t = _processContext->getTaskByID(subTask);
                    processTask(t);
                }
                auto t = _processContext->getTaskByID(task->getNextTaskID());
                if (t) {
                    processTask(t);
                } else {
                    if (_processContext->_taskFinishFunction) {
                        _processContext->_taskFinishFunction();
                    }
                }
            }
        }
    })).thenError(folly::tag_t<std::exception>{}, [](std::exception const &e) {
        LOG(ERROR) << e.what();
    });
}

void Process::Process::startProcess(std::shared_ptr<ProcessContext> context, bool isSub) {
    setState(State::RUNNING);
    if (isSub) {
        _processContext->_processValues = context->_processValues;
        _processContext->_executor = context->_executor;
        _processContext->_eventHandler = context->_eventHandler;
    }
    _processContext->_status = 1;
    initEventHandlers();
    processTask(_processContext->getStartTask());
}

void Process::Process::processDefaultTask(std::shared_ptr<Task> task) {
//    LOG(INFO)<<task->getName() <<"  "<<task->get_type().get_name();
    if (task) {
        folly::via(_processContext->getExecutor().get(), std::bind([&, task]() {
            try {
                task->run(_processContext);
                _processContext->_status = 1;
            } catch (TaskRuntimeException &ex) {
                _processContext->_status = 2;
                LOG(ERROR) << task->getName() << " " << task->getID() << " retry again";
                processTask(task);
            } catch (TaskError &err) {
                _processContext->_status = 3;
                LOG(ERROR) << task->getName() << " " << task->getID() << " restart process";
                processTask(_processContext->getStartTask());
            } catch (UserDefinedExeception &ex) {
                boost::any any = _processContext->_processValues->rlock()->find(ex._type);
                ExceptionHandler exceptionHandler = boost::any_cast<ExceptionHandler>(any);
                if (exceptionHandler) {
                    exceptionHandler();
                }
            }
        })).then(std::bind([&, task]() {
            if (_processContext->_status == 1) {
//            LOG(INFO) << "processDefaultTask then " << task->_name <<" "<<task->getNextTaskID();
                auto t = _processContext->getTaskByID(task->getNextTaskID());
                if (t) {
                    processTask(t);
                } else if (task->get_type().get_name() == "EndTask") {
                    if (_processContext->_taskFinishFunction) {
                        _processContext->_taskFinishFunction();
                    }
                } else {
                    LOG(WARNING) << "the task has no next task and is not endtask ";
                }
            }
        })).thenError(folly::tag_t<TaskRuntimeException>{}, [](TaskRuntimeException const &e) {
            LOG(ERROR) << e.what();
        });
    } else {
        LOG(ERROR) << " can not process null task";
    }
}

std::shared_ptr<Process::ProcessContext> Process::Process::getProcessContext() {
    return _processContext;
}

void Process::Process::loadXML(std::string xml) {
    _processContext->loadXML(xml);
}

void Process::Process::loadFile(std::string filePath) {
    std::lock_guard<std::mutex> guard{_mutex};
    _processContext->loadFile(filePath);
}

std::string Process::Process::saveXML() {
    return _processContext->saveXML();
}

void Process::Process::stopProcess() {
    setState(State::STOPED);
}

void Process::Process::suspend() {
    setState(State::SUSPEND);
}

void Process::Process::restore() {
    setState(State::RUNNING);
    for (auto &t: _processContext->_tasks) {
        if (t->get_type().get_name() == "SubProcessTask") {
            std::shared_ptr<SubProcessTask> subProcess = std::dynamic_pointer_cast<SubProcessTask>(t);
            subProcess->_subProcess->restore();
        }
    }
    auto taskRlock = _processContext->_suspendTasks->wlock();
    for (int i = 0; i < taskRlock->size(); i++) {
        processTask(taskRlock->at(i));
    }
    taskRlock->clear();
}

void Process::Process::setState(State state) {
    _processContext->getProcessValues()->wlock()->operator[]("ProcessRunState") = state;
}

Process::State Process::Process::getState() {
    auto rlock = _processContext->getProcessValues()->rlock();
    if (rlock->find("ProcessRunState") != rlock->end()) {
        return boost::any_cast<State>(rlock->at("ProcessRunState"));
    }
    return State::UNKNOW;
}

void Process::Process::initProcessValues(std::map<std::string, boost::any> values) {
    auto lock = _processContext->getProcessValues()->wlock();
    for (auto &pair: values) {
        lock->insert(pair);
    }
}

void Process::Process::initEventHandlers() {
    auto events = _processContext->getTasksByType("EventGateway");
    for (auto &event: events) {
        auto eventGateway = std::dynamic_pointer_cast<EventGateway>(event);
        auto eventTyes = eventGateway->_eventRules;
        for (auto &et: eventTyes) {
            auto task = _processContext->getTaskByID(et.second);
            _processContext->_eventHandler->insert({et.first, [&, task]() {
                folly::via(_processContext->_executor.get(), [&, task]() { processDefaultTask(task); });
            }});
        }
    }
}

void Process::Process::addTask(std::shared_ptr<Task> task) {
    getProcessContext()->addTask(task);
}

void Process::Process::setName(std::string name) {
    if (_processContext) {
        _processContext->setTaskName(name);
    }
}

void Process::Process::addTaskEventHandler(TaskEventHandler handler) {
    _taskEventHandlers.push_back(handler);
}

void Process::Process::notityTaskEvent(std::shared_ptr<Task> task) {
    folly::via(_processContext->getExecutor().get(), std::bind([&, task]() {
        for (auto t:_taskEventHandlers) {
            t(task);
        }
    }));
}
