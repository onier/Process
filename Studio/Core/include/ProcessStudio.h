//
// Created by xuzhenhai on 2022/5/16.
//

#ifndef PUPPY_PROCESSSTUDIO_H
#define PUPPY_PROCESSSTUDIO_H

#include <ostream>
#include "Process.h"
#include "memory"
#include "Shape.h"
#include "ProcessGraphics.h"
#include "Edge.h"
#include "Action.h"

enum class ParameterType {
    INT = 0x0001,
    FLOAT = 0x0002,
    DOUBLE = 0x0004,
    STRING = 0x0008
};

struct Para {
    std::string _name;
    std::string _value;
    ParameterType _type;
    std::string _description;

    friend std::ostream &operator<<(std::ostream &os, const Para &para);

    Para();

RTTR_ENABLE()
};

struct ProcesInfo {
    std::string _name;
    int _threadCount;

    std::vector<Para> _parameters;

    ProcesInfo();

RTTR_ENABLE()
};

struct RuleShapeItem {
    std::shared_ptr<Shape> _shape;
    rttr::variant _rule;
};

struct DumyTaskItem {
    std::shared_ptr<Process::Task> _task;
    rttr::type _taskType;
    rttr::variant _taskVar;

    DumyTaskItem();

    DumyTaskItem(const std::shared_ptr<Process::Task> &task, const rttr::type &taskType, const rttr::variant &taskVar);
};

struct DumyShapeItem {
    std::shared_ptr<Shape> _shape;
    rttr::type _shapeType;
    rttr::variant _shapeVar;

    DumyShapeItem();

    DumyShapeItem(const std::shared_ptr<Shape> &shape, const rttr::type &shapeType, const rttr::variant &shapeVar);
};

struct TaskShapeItem {
    std::shared_ptr<Process::Task> _task;
    std::shared_ptr<Shape> _shape;
    rttr::type _taskType;
    rttr::type _shapeType;
    rttr::variant _taskVar;
    rttr::variant _shapeVar;

    TaskShapeItem(std::shared_ptr<DumyTaskItem> taskItem, std::shared_ptr<DumyShapeItem> shapeItem);

    TaskShapeItem(const std::shared_ptr<Process::Task> &task, const std::shared_ptr<Shape> &shape,
                  const rttr::type &taskType, const rttr::type &shapeType, const rttr::variant &taskVar,
                  const rttr::variant &shapeVar);
};


typedef std::vector<std::shared_ptr<TaskShapeItem>> TaskShpeItems;
typedef std::vector<std::shared_ptr<RuleShapeItem>> RuleShapeItems;

class ProcessStudio {
public:
    typedef std::function<void(ProcessStudio *, std::shared_ptr<TaskShapeItem>)> SelectTaskShapeItemEventHanlder;
    typedef std::function<void(ProcessStudio *, std::shared_ptr<RuleShapeItem>)> SelectRuleShapeItemEventHanlder;
    typedef std::function<void(std::string)> MessageHandler;

    ProcessStudio();

    std::vector<std::shared_ptr<Process::Task>> getTaskByShape(std::shared_ptr<Shape> shape);

    std::vector<std::shared_ptr<Shape>> getShapeByTask(std::shared_ptr<Process::Task> task);

    std::shared_ptr<TaskShapeItem> getTaskShapeItemByShape(std::shared_ptr<Shape> shape);

    std::shared_ptr<RuleShapeItem> getRuleShapeItems(std::shared_ptr<Shape> shape);

    void addTaskShapeItem(std::shared_ptr<TaskShapeItem> item);

    void addTask(std::shared_ptr<Process::Task> task);

    void removeTask(std::shared_ptr<Process::Task> task);

    void removeShape(std::shared_ptr<Shape> shape);

    std::shared_ptr<ProcessGraphics> getProcessGraphics();

    std::shared_ptr<Shape> getCurrentSelectShape();

    void setCurrentSelectShape(std::shared_ptr<Shape> shape);

    void addTaskShapeItemSelectEventHanlder(SelectTaskShapeItemEventHanlder hanlder);

    void addRuleShapeItemSelectEventHanlder(SelectRuleShapeItemEventHanlder hanlder);

    void addProperyMessageHandler(std::string message, std::function<void(std::shared_ptr<Shape>)> h);

    void startProcess();

    void stopProcess();

    void addMessageHandler(MessageHandler handler);

    rttr::variant _processInfoVariant;

    std::string saveToXML();

    void loadFromXML(std::string xml);

protected:

    void saveTaskManager(xercesc::DOMElement *domElement, xercesc::DOMDocument *document);

    void notifyTaskShapeItemSlectChange(ProcessStudio *processStudio, std::shared_ptr<Shape> shape);

    void notifyRuleShapeItemSlectChange(ProcessStudio *processStudio, std::shared_ptr<Shape> shape);

    void updateTaskConnect(std::shared_ptr<Shape> start, std::shared_ptr<Shape> end);

    void updateMessageHandler(std::string msg);

    void clear();

private:
    std::vector<Para> parseParameters(xercesc::DOMNode *parameters);

    std::map<std::string, std::shared_ptr<DumyTaskItem>> parseTasks(xercesc::DOMNode *tasks);

    std::map<std::string, std::shared_ptr<DumyShapeItem>> parseShapes(xercesc::DOMNode *shapes);

    Para parseParameter(xercesc::DOMNode *parameter);

    void initEdge();

private:
    std::map<std::string, std::function<void(std::shared_ptr<Shape>)>> _propertyMessageHandlers;
    std::shared_ptr<Shape> _currentSelectShape;
    std::vector<std::shared_ptr<Process::Task>> _tasks;
    std::shared_ptr<Process::Process> _process;
    std::shared_ptr<ProcessGraphics> _graphics;
    TaskShpeItems _taskShapes;
    /**
     * gate way rule
     */
    RuleShapeItems _ruleShapeItems;
    std::vector<SelectTaskShapeItemEventHanlder> _selectTaskShapeItemEventHanlders;
    std::vector<SelectRuleShapeItemEventHanlder> _selectRuleShapeItemEventHanlders;
    std::vector<MessageHandler> _messageHandlers;
};


#endif //PUPPY_PROCESSSTUDIO_H
