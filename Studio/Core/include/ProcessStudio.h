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

typedef std::vector<std::shared_ptr<TaskShapeItem>> TaskShpePairs;

class ProcessStudio {
public:
    typedef std::function<void(ProcessStudio *, std::shared_ptr<TaskShapeItem>)> SelectEventHanlder;

    ProcessStudio();

    std::vector<std::shared_ptr<Process::Task>> getTaskByShape(std::shared_ptr<Shape> shape);

    std::vector<std::shared_ptr<Shape>> getShapeByTask(std::shared_ptr<Process::Task> task);

    std::shared_ptr<TaskShapeItem> getTaskShapeItemByShape(std::shared_ptr<Shape> shape);

    void addTaskShapePair(std::shared_ptr<TaskShapeItem> item);

    void setProcess(std::shared_ptr<Process::Process> process);

    void addTask(std::shared_ptr<Process::Task> task);

    std::shared_ptr<ProcessGraphics> getProcessGraphics();

    std::shared_ptr<Shape> getCurrentSelectShape();

    void setCurrentSelectShape(std::shared_ptr<Shape> shape);

    void addSelectEventHanlder(SelectEventHanlder hanlder);

    ProcesInfo _processInfo;
protected:
    void notifySelectShape(ProcessStudio *processStudio, std::shared_ptr<Shape> shape);

private:
    std::shared_ptr<Shape> _currentSelectShape;
    std::shared_ptr<Process::Process> _process;
    std::shared_ptr<ProcessGraphics> _graphics;
    TaskShpePairs _taskShapes;
    std::vector<SelectEventHanlder> _selectEventHanlders;
};


#endif //PUPPY_PROCESSSTUDIO_H
