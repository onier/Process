//
// Created by xuzhenhai on 2022/5/16.
//

#include "ProcessStudio.h"

ProcessStudio::ProcessStudio() {
    _graphics = std::make_shared<ProcessGraphics>();
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

void ProcessStudio::addTaskShapePair(std::shared_ptr<TaskShapeItem> item) {
    _taskShapes.push_back(item);
}

void ProcessStudio::setProcess(std::shared_ptr<Process::Process> process) {
    _process = process;
}

std::shared_ptr<ProcessGraphics> ProcessStudio::getProcessGraphics() {
    return _graphics;
}

std::shared_ptr<Shape> ProcessStudio::getCurrentSelectShape() {
    return _currentSelectShape;
}

void ProcessStudio::setCurrentSelectShape(std::shared_ptr<Shape> shape) {
    _currentSelectShape = shape;
    notifySelectShape(this, shape);
}

void ProcessStudio::addTask(std::shared_ptr<Process::Task> task) {
    _process->addTask(task);
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
