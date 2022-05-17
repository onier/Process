//
// Created by xuzhenhai on 2022/5/16.
//

#ifndef PUPPY_PROCESSSTUDIO_H
#define PUPPY_PROCESSSTUDIO_H

#include "Process.h"
#include "memory"
#include "Shape.h"
#include "ProcessGraphics.h"
#include "Edge.h"
#include "Action.h"

struct ProcesInfo {
    std::string _name;
    int _threadCount;

    ProcesInfo();

RTTR_ENABLE()
};

struct TaskShapeItem {
    std::shared_ptr<Process::Task> _task;
    std::shared_ptr<Shape> _shape;
    rttr::type _taskType;
    rttr::type _shapeType;
    rttr::variant _taskVar;
    rttr::variant _shapeVar;

    TaskShapeItem(const std::shared_ptr<Process::Task> &task, const std::shared_ptr<Shape> &shape,
                  const rttr::type &taskType, const rttr::type &shapeType, const rttr::variant &taskVar,
                  const rttr::variant &shapeVar);
};

typedef std::vector<std::shared_ptr<TaskShapeItem>> TaskShpeItems;

class ProcessStudio {
public:
    typedef std::function<void(ProcessStudio *, std::shared_ptr<TaskShapeItem>)> SelectEventHanlder;

    ProcessStudio();

    std::vector<std::shared_ptr<Process::Task>> getTaskByShape(std::shared_ptr<Shape> shape);

    std::vector<std::shared_ptr<Shape>> getShapeByTask(std::shared_ptr<Process::Task> task);

    std::shared_ptr<TaskShapeItem> getTaskShapeItemByShape(std::shared_ptr<Shape> shape);

    void addTaskShapeItem(std::shared_ptr<TaskShapeItem> item);

    void addTask(std::shared_ptr<Process::Task> task);

    void removeTask(std::shared_ptr<Process::Task> task);

    void removeShape(std::shared_ptr<Shape> shape);

    std::shared_ptr<ProcessGraphics> getProcessGraphics();

    std::shared_ptr<Shape> getCurrentSelectShape();

    void setCurrentSelectShape(std::shared_ptr<Shape> shape);

    void addSelectEventHanlder(SelectEventHanlder hanlder);

    void addProperyMessageHandler(std::string message, std::function<void(std::shared_ptr<Shape>)> h);

    void startProcess();

    void stopProcess();

    ProcesInfo _processInfo;
protected:
    void notifySelectShape(ProcessStudio *processStudio, std::shared_ptr<Shape> shape);

    void updateTaskConnect(std::shared_ptr<Shape> start, std::shared_ptr<Shape> end);

private:
    std::map<std::string, std::function<void(std::shared_ptr<Shape>)>> _propertyMessageHandlers;
    std::shared_ptr<Shape> _currentSelectShape;
    std::vector<std::shared_ptr<Process::Task>> _tasks;
    std::shared_ptr<Process::Process> _process;
    std::shared_ptr<ProcessGraphics> _graphics;
    TaskShpeItems _taskShapes;
    std::vector<SelectEventHanlder> _selectEventHanlders;
};


#endif //PUPPY_PROCESSSTUDIO_H
