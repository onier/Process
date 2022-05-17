//
// Created by xuzhenhai on 2022/5/16.
//

#include "ProcessStudio.h"

ProcessStudio::ProcessStudio() {
    _graphics = std::make_shared<ProcessGraphics>();
    _graphics->addHandler([&](std::shared_ptr<Shape> shape, std::string msg) {
        _propertyMessageHandlers[msg](shape);
    });
    _propertyMessageHandlers.insert({"EdgeStartShapeRemove", [&](std::shared_ptr<Shape> shape) {
        std::shared_ptr<Edge> edge = std::dynamic_pointer_cast<Edge>(shape);
        if (edge) {
            auto startShape = edge->getStartShape();
            auto startTask = getTaskByShape(startShape);
            if (startTask.size() == 1) {
                startTask[0]->setNextTaskID(shape->_id,true);
            }
        }
    }});
    _propertyMessageHandlers.insert({"EdgeEndShapeRemove", [&](std::shared_ptr<Shape> shape) {
        std::shared_ptr<Edge> edge = std::dynamic_pointer_cast<Edge>(shape);
        if (edge) {
            auto endShape = edge->getEndShape();
            auto endTask = getTaskByShape(endShape);
            if (endTask.size() == 1) {
                endTask[0]->setNextTaskID(shape->_id,true);
            }
        }
    }});
    _propertyMessageHandlers.insert({"EdgeStartShapeChange", [&](std::shared_ptr<Shape> shape) {
        std::shared_ptr<Edge> edge = std::dynamic_pointer_cast<Edge>(shape);
        if (edge->getStartShape() == edge->getEndShape() && edge->getStartShape()) {
            _graphics->removeShape(shape);
        } else {
            updateTaskConnect(edge->getStartShape(), edge->getEndShape());
        }
    }});
    _propertyMessageHandlers.insert({"EdgeEndShapeChange", [&](std::shared_ptr<Shape> shape) {
        std::shared_ptr<Edge> edge = std::dynamic_pointer_cast<Edge>(shape);
        if (edge->getStartShape() == edge->getEndShape() && edge->getStartShape()) {
            _graphics->removeShape(shape);
        } else {
            updateTaskConnect(edge->getStartShape(), edge->getEndShape());
        }
    }});
}

void ProcessStudio::updateTaskConnect(std::shared_ptr<Shape> start, std::shared_ptr<Shape> end) {
    if (start && end) {
        auto startTask = getTaskByShape(start);
        auto endTask = getTaskByShape(end);
        if (startTask.size() == 1 && endTask.size() == 1) {
            startTask[0]->setNextTaskID(endTask[0]->_id);
            endTask[0]->setPreTaskID(startTask[0]->_id);
        }
        if (startTask.size() == 1 && endTask.size() != 1) {
            startTask[0]->setNextTaskID("");
        }

        if (startTask.size() != 1 && endTask.size() == 1) {
            startTask[0]->setNextTaskID("");
            endTask[0]->setPreTaskID("");
        }
    } else {
        LOG(ERROR) << " the shape must not null";
    }
}

std::vector<std::shared_ptr<Process::Task>> ProcessStudio::getTaskByShape(std::shared_ptr<Shape> shape) {
    std::vector<std::shared_ptr<Process::Task>> result;
    for (auto &pair: _taskShapes) {
        if (pair->_shape == shape) {
            result.push_back(pair->_task);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Shape>> ProcessStudio::getShapeByTask(std::shared_ptr<Process::Task> task) {
    std::vector<std::shared_ptr<Shape>> result;
    for (auto &pair: _taskShapes) {
        if (pair->_task == task) {
            result.push_back(pair->_shape);
        }
    }
    return result;
}

void ProcessStudio::addTaskShapeItem(std::shared_ptr<TaskShapeItem> item) {
    _taskShapes.push_back(item);
}

std::shared_ptr<ProcessGraphics> ProcessStudio::getProcessGraphics() {
    return _graphics;
}

void ProcessStudio::removeShape(std::shared_ptr<Shape> shape) {
    auto tasks = getTaskByShape(shape);
    for (auto &t: tasks) {
        removeTask(t);
    }
    _graphics->removeShape(shape);
}

void ProcessStudio::removeTask(std::shared_ptr<Process::Task> task) {
    auto iter = std::find_if(_tasks.begin(), _tasks.end(), [task](auto t) {
        return t == task;
    });
    if (iter != _tasks.end()) {
        _tasks.erase(iter);
    }

    auto iter1 = std::find_if(_taskShapes.begin(), _taskShapes.end(), [task](std::shared_ptr<TaskShapeItem> t) {
        return t->_task == task;
    });
    if (iter1 != _taskShapes.end()) {
        _taskShapes.erase(iter1);
    }
}

std::shared_ptr<Shape> ProcessStudio::getCurrentSelectShape() {
    return _currentSelectShape;
}

void ProcessStudio::setCurrentSelectShape(std::shared_ptr<Shape> shape) {
    _currentSelectShape = shape;
    notifySelectShape(this, shape);
}

void ProcessStudio::addTask(std::shared_ptr<Process::Task> task) {
    _tasks.push_back(task);
}

void ProcessStudio::addSelectEventHanlder(ProcessStudio::SelectEventHanlder hanlder) {
    _selectEventHanlders.push_back(hanlder);
}

std::shared_ptr<TaskShapeItem> ProcessStudio::getTaskShapeItemByShape(std::shared_ptr<Shape> shape) {
    for (auto &i: _taskShapes) {
        if (i->_shape == shape) {
            return i;
        }
    }
    return {};
}

void ProcessStudio::notifySelectShape(ProcessStudio *processStudio, std::shared_ptr<Shape> shape) {
    auto item = getTaskShapeItemByShape(shape);
    for (auto &h: _selectEventHanlders) {
        h(processStudio, item);
    }
}

TaskShapeItem::TaskShapeItem(const std::shared_ptr<Process::Task> &task, const std::shared_ptr<Shape> &shape,
                             const rttr::type &taskType, const rttr::type &shapeType, const rttr::variant &taskVar,
                             const rttr::variant &shapeVar) : _task(task), _shape(shape), _taskType(taskType),
                                                              _shapeType(shapeType), _taskVar(taskVar),
                                                              _shapeVar(shapeVar) {}

ProcesInfo::ProcesInfo() {
    _threadCount = 4;
}

void ProcessStudio::startProcess() {
    _process = std::make_shared<Process::Process>(_processInfo._threadCount);
    for (auto &t: _tasks) {
        _process->addTask(t);
    }
    _process->setName(_processInfo._name);
    _process->startProcess(nullptr);
}

void ProcessStudio::stopProcess() {
    if (_process) {
        _process->stopProcess();
    }
}

void ProcessStudio::addProperyMessageHandler(std::string message, std::function<void(std::shared_ptr<Shape>)> h) {
    _propertyMessageHandlers.insert({message, h});
}
