//
// Created by xuzhenhai on 2022/5/16.
//

#include "ProcessStudio.h"
#include "XML.h"
#include "boost/lexical_cast.hpp"
#include "xercesc/util/PlatformUtils.hpp"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/dom/DOMNodeFilter.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include "Edge.h"

thread_local std::shared_ptr<Shape> currentShape;

ProcessStudio::ProcessStudio() {
    _graphics = std::make_shared<ProcessGraphics>();
    _graphics->addHandler([&](std::shared_ptr<Shape> shape, std::string msg) {
        _propertyMessageHandlers[msg](shape);
    });
    _propertyMessageHandlers.insert({"EdgeStartShapeRemove", [&](std::shared_ptr<Shape> shape) {
        std::shared_ptr<Edge> edge = std::dynamic_pointer_cast<Edge>(shape);
        currentShape = edge;
        if (edge) {
            auto startShape = edge->getStartShape();
            auto startTask = getTaskByShape(startShape);
            if (startTask.size() == 1) {
                startTask[0]->setNextTaskID(shape->_id, true);
            }
        }
    }});
    _propertyMessageHandlers.insert({"EdgeEndShapeRemove", [&](std::shared_ptr<Shape> shape) {
        std::shared_ptr<Edge> edge = std::dynamic_pointer_cast<Edge>(shape);
        currentShape = edge;
        if (edge) {
            auto endShape = edge->getEndShape();
            auto endTask = getTaskByShape(endShape);
            if (endTask.size() == 1) {
                endTask[0]->setNextTaskID(shape->_id, true);
            }
        }
    }});
    _propertyMessageHandlers.insert({"EdgeStartShapeChange", [&](std::shared_ptr<Shape> shape) {
        std::shared_ptr<Edge> edge = std::dynamic_pointer_cast<Edge>(shape);
        currentShape = edge;
        if (edge->getStartShape() == edge->getEndShape() && edge->getStartShape()) {
            _graphics->removeShape(shape);
        } else {
            updateTaskConnect(edge->getStartShape(), edge->getEndShape());
        }
    }});
    _propertyMessageHandlers.insert({"EdgeEndShapeChange", [&](std::shared_ptr<Shape> shape) {
        std::shared_ptr<Edge> edge = std::dynamic_pointer_cast<Edge>(shape);
        currentShape = edge;
        if (edge->getStartShape() == edge->getEndShape() && edge->getStartShape()) {
            _graphics->removeShape(shape);
        } else {
            updateTaskConnect(edge->getStartShape(), edge->getEndShape());
        }
    }});
    _processInfoVariant = rttr::type::get_by_name("ProcesInfo").create();
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
    _graphics->addShape(item->_shape);
    addTask(item->_task);
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
    notifyTaskShapeItemSlectChange(this, shape);
    notifyRuleShapeItemSlectChange(this, shape);
}

void ProcessStudio::addTask(std::shared_ptr<Process::Task> task) {
    _tasks.push_back(task);
    task->addMessageEvent([&](std::string msg, boost::any any, std::string content) {
        if (msg == "addExclusiveRule") {
            RuleShapeItem ruleShapeItem;
            ruleShapeItem._shape = currentShape;
            currentShape->_id = content;
            ruleShapeItem._rule = boost::any_cast<rttr::variant>(any);
            _ruleShapeItems.push_back(std::make_shared<RuleShapeItem>(ruleShapeItem));
            currentShape = nullptr;
        }
    });
}

void ProcessStudio::addTaskShapeItemSelectEventHanlder(ProcessStudio::SelectTaskShapeItemEventHanlder hanlder) {
    _selectTaskShapeItemEventHanlders.push_back(hanlder);
}

void ProcessStudio::addRuleShapeItemSelectEventHanlder(ProcessStudio::SelectRuleShapeItemEventHanlder hanlder) {
    _selectRuleShapeItemEventHanlders.push_back(hanlder);
}

std::shared_ptr<TaskShapeItem> ProcessStudio::getTaskShapeItemByShape(std::shared_ptr<Shape> shape) {
    for (auto &i: _taskShapes) {
        if (i->_shape == shape) {
            return i;
        }
    }
    return {};
}

std::shared_ptr<RuleShapeItem> ProcessStudio::getRuleShapeItems(std::shared_ptr<Shape> shape) {
    for (auto &i: _ruleShapeItems) {
        if (i->_shape == shape) {
            return i;
        }
    }
    return {};
}

void ProcessStudio::notifyTaskShapeItemSlectChange(ProcessStudio *processStudio, std::shared_ptr<Shape> shape) {
    auto item = getTaskShapeItemByShape(shape);
    for (auto &h: _selectTaskShapeItemEventHanlders) {
        h(processStudio, item);
    }
}

void ProcessStudio::notifyRuleShapeItemSlectChange(ProcessStudio *processStudio, std::shared_ptr<Shape> shape) {
    auto item = getRuleShapeItems(shape);
    for (auto &h: _selectRuleShapeItemEventHanlders) {
        h(processStudio, item);
    }
}

TaskShapeItem::TaskShapeItem(std::shared_ptr<DumyTaskItem> taskItem, std::shared_ptr<DumyShapeItem> shapeItem) : _task(
        taskItem->_task), _shape(shapeItem->_shape), _taskType(taskItem->_taskType), _shapeType(
        shapeItem->_shapeType), _taskVar(taskItem->_taskVar), _shapeVar(shapeItem->_shapeVar) {

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
    auto _processInfo = _processInfoVariant.get_value<std::shared_ptr<ProcesInfo>>();
    _process = std::make_shared<Process::Process>(_processInfo->_threadCount);
    _process->addTaskEventHandler([&](std::shared_ptr<Process::Task> task) {
        _graphics->clearSelection();
        auto shapes = getShapeByTask(task);
        for (auto s: shapes) {
            s->setSelected(true);
        }
        updateMessageHandler("repaint");
    });
    for (auto &t: _tasks) {
        _process->addTask(t);
    }
//    _process->getProcessContext()->_processValues
    {
        auto w = _process->getProcessContext()->_processValues->wlock();
        for (auto p: _processInfo->_parameters) {
            switch (p._type) {
                case ParameterType::INT:
                    try {
                        w->insert({p._name, boost::lexical_cast<int>(p._value)});
                    } catch (...) {}
                    break;
                case ParameterType::FLOAT:
                    try {
                        w->insert({p._name, boost::lexical_cast<float>(p._value)});
                    } catch (...) {}
                    break;
                case ParameterType::DOUBLE:
                    try {
                        w->insert({p._name, boost::lexical_cast<double>(p._value)});
                    } catch (...) {}
                    break;
                case ParameterType::STRING:
                    try {
                        w->insert({p._name, boost::lexical_cast<std::string>(p._value)});
                    } catch (...) {}
                    break;
            }
        }
    }
    _process->setName(_processInfo->_name);
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

Para::Para() {
    _type = ParameterType::DOUBLE;
}

std::ostream &operator<<(std::ostream &os, const Para &para) {
    os << "_name: " << para._name << " _value: " << para._value << " _type: " << (int) para._type << " _description: "
       << para._description;
    return os;
}

void ProcessStudio::addMessageHandler(ProcessStudio::MessageHandler handler) {
    _messageHandlers.push_back(handler);
}

void ProcessStudio::updateMessageHandler(std::string msg) {
    for (auto h: _messageHandlers) {
        h(msg);
    }
}

std::string ProcessStudio::saveToXML() {
    xercesc::XMLPlatformUtils::Initialize();
    xercesc::DOMImplementation *domImplementation =
            xercesc::DOMImplementationRegistry::getDOMImplementation(XStr("Core"));
    std::shared_ptr<xercesc::DOMLSSerializer> serializer = std::shared_ptr<xercesc::DOMLSSerializer>(
            domImplementation->createLSSerializer());
    auto config = serializer->getDomConfig();
    config->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
    auto out = domImplementation->createLSOutput();
    std::shared_ptr<xercesc::MemBufFormatTarget> formatTarget = std::shared_ptr<xercesc::MemBufFormatTarget>(
            new xercesc::MemBufFormatTarget());
    out->setByteStream(formatTarget.get());
    std::shared_ptr<xercesc::DOMDocument> document = std::shared_ptr<xercesc::DOMDocument>(
            domImplementation->createDocument(0, XStr("Process"), 0));
    auto rootElement = document->getDocumentElement();
    saveTaskManager(rootElement, document.get());
//    auto taskManager = document->createElement(XStr("TaskManager"));
//    taskManager->setAttribute(XStr("taskName"), XStr(_taskName.data()));
//    taskManager->setAttribute(XStr("threadCount"), XStr(boost::lexical_cast<std::string>(_threadCount).data()));
//    rootElement->appendChild(taskManager);
    document->normalizeDocument();
    serializer->write(document.get(), out);
    return std::string(reinterpret_cast<const char *>(formatTarget->getRawBuffer()));
}

void ProcessStudio::loadFromXML(std::string xml) {
    clear();
    xercesc::XMLPlatformUtils::Initialize();
    xercesc::XercesDOMParser xercesDOMParser;
    std::shared_ptr<xercesc::MemBufInputSource> memBufIS = std::shared_ptr<xercesc::MemBufInputSource>(
            new xercesc::MemBufInputSource((const XMLByte *) xml.data(), xml.size(), ""));
    xercesDOMParser.parse(*memBufIS);
    auto document = xercesDOMParser.getDocument();
    auto process = document->getElementsByTagName(XStr("Process"));
    LOG(INFO) << process->getLength();
    for (int i = 0; i < process->getLength(); i++) {
        auto p = process->item(i);
        std::vector<xercesc::DOMNode *> managers;
        puppy::common::XML::getTagsByName("TaskManager", p, managers);
        for (auto manager: managers) {
            std::vector<xercesc::DOMNode *> parametersNodes;
            std::vector<xercesc::DOMNode *> tasksNodes;
            std::vector<xercesc::DOMNode *> shapesNodes;
            puppy::common::XML::getTagsByName("Parameters", manager, parametersNodes);
            puppy::common::XML::getTagsByName("tasks", manager, tasksNodes);
            puppy::common::XML::getTagsByName("shapes", manager, shapesNodes);
            if (parametersNodes.size() == 1) {
                auto paras = parseParameters(parametersNodes[0]);
                rttr::type::get_by_name("ProcesInfo").get_property("Parameters").set_value(_processInfoVariant, paras);
            }
            std::map<std::string, std::shared_ptr<DumyTaskItem>> tasks;
            std::map<std::string, std::shared_ptr<DumyShapeItem>> shapes;
            if (tasksNodes.size() == 1) {
                tasks = parseTasks(tasksNodes[0]);
                for (auto t: tasks) {
                    addTask(t.second->_task);
                }
            }
            if (shapesNodes.size() == 1) {
                shapes = parseShapes(shapesNodes[0]);
                for (auto s: shapes) {
                    _graphics->addShape(s.second->_shape);
                }
            }
            for (auto shape: shapes) {
                if (tasks.count(shape.first) > 0) {
                    _taskShapes.push_back(std::make_shared<TaskShapeItem>(tasks[shape.first], shape.second));
                }
            }
        }
    }
    initEdge();
}

std::vector<Para> ProcessStudio::parseParameters(xercesc::DOMNode *parameters) {
    std::vector<xercesc::DOMNode *> ps;
    puppy::common::XML::getTagsByName("Parameters", parameters, ps);
    std::vector<Para> paras;
    for (auto p: ps) {
        Para para = parseParameter(p);
        if (para._name.empty()) {
            paras.push_back(para);
        }
    }
    return paras;
}

std::map<std::string, std::shared_ptr<DumyTaskItem>> ProcessStudio::parseTasks(xercesc::DOMNode *tasks) {
    auto nodes = tasks->getChildNodes();
    std::map<std::string, std::shared_ptr<DumyTaskItem>> results;
    for (int i = 0; i < nodes->getLength(); i++) {
        auto task = nodes->item(i);
        auto taskTypeName = puppy::common::XML::toStr(task->getNodeName());
        auto taskType = rttr::type::get_by_name(taskTypeName);
        if (taskType.is_valid()) {
            auto taskVariant = taskType.create();
            puppy::common::XML::parseInstance(task, taskVariant);
            auto absPtr = taskVariant.get_value<std::shared_ptr<Process::Task>>();
            absPtr->loadDomElement(task);
            results.insert({absPtr->_id, std::make_shared<DumyTaskItem>(absPtr, taskType, taskVariant)});
        }
    }
    return results;
}

std::map<std::string, std::shared_ptr<DumyShapeItem>> ProcessStudio::parseShapes(xercesc::DOMNode *shapes) {
    auto nodes = shapes->getChildNodes();
    std::map<std::string, std::shared_ptr<DumyShapeItem>> results;
    for (int i = 0; i < nodes->getLength(); i++) {
        auto shape = nodes->item(i);
        std::string taskName = puppy::common::XML::toStr(shape->getNodeName());
        auto type = rttr::type::get_by_name(taskName.data());
        if (type.is_valid()) {
            auto taskVariant = type.create();
            auto taskPtr = taskVariant.get_value<std::shared_ptr<Shape>>();
            puppy::common::XML::parseInstance(shape, taskVariant);
            taskPtr->loadDomElement(shape);
            results.insert({taskPtr->_id, std::make_shared<DumyShapeItem>(taskPtr, type, taskVariant)});
        }
    }
    return results;
}

Para ProcessStudio::parseParameter(xercesc::DOMNode *parameter) {
    std::string name, type, value, des;
    name = puppy::common::XML::attributeValue(parameter->getAttributes(), "name");
    type = puppy::common::XML::attributeValue(parameter->getAttributes(), "type");
    value = puppy::common::XML::attributeValue(parameter->getAttributes(), "value");
    value = puppy::common::XML::attributeValue(parameter->getAttributes(), "des");
    Para para;
    para._name = name;
    if (type == "Int") {
        para._type = ParameterType::INT;
    } else if (type == "Float") {
        para._type = ParameterType::FLOAT;
    } else if (type == "Double") {
        para._type = ParameterType::DOUBLE;
    } else if (type == "String") {
        para._type = ParameterType::STRING;
    }
    para._value = value;
    para._description = des;
    return para;
}

void ProcessStudio::saveTaskManager(xercesc::DOMElement *domElement, xercesc::DOMDocument *document) {
    auto rootElement = document->getDocumentElement();
    auto _processInfo = _processInfoVariant.get_value<std::shared_ptr<ProcesInfo>>();
    auto taskManager = document->createElement(XStr("TaskManager"));
    taskManager->setAttribute(XStr("taskName"), XStr(_processInfo->_name.data()));
    taskManager->setAttribute(XStr("threadCount"),
                              XStr(boost::lexical_cast<std::string>(_processInfo->_threadCount).data()));
    rootElement->appendChild(taskManager);

    {
        auto parameters = document->createElement(XStr("Parameters"));
        taskManager->appendChild(parameters);
        for (auto p: _processInfo->_parameters) {
            xercesc::DOMElement *paraEle = document->createElement(XStr("Parameter"));
            parameters->appendChild(paraEle);
            paraEle->setAttribute(XStr("name"), XStr(p._name.data()));
            paraEle->setAttribute(XStr("value"), XStr(p._value.data()));
            paraEle->setAttribute(XStr("des"), XStr(p._description.data()));
            switch (p._type) {
                case ParameterType::INT:
                    paraEle->setAttribute(XStr("type"), XStr("Int"));
                    break;
                case ParameterType::FLOAT:
                    paraEle->setAttribute(XStr("type"), XStr("Float"));
                    break;
                case ParameterType::DOUBLE:
                    paraEle->setAttribute(XStr("type"), XStr("Double"));
                    break;
                case ParameterType::STRING:
                    paraEle->setAttribute(XStr("type"), XStr("String"));
                    break;
            }

        }
    }
    {
        auto tasks = document->createElement(XStr("tasks"));
        taskManager->appendChild(tasks);
        for (auto t: _tasks) {
            puppy::common::XML::createElement<Process::Task>(t, tasks, document, t);
        }
    }
    {
        auto shapes = document->createElement(XStr("shapes"));
        taskManager->appendChild(shapes);
        for (auto shape: _graphics->getShapes()) {
            xercesc::DOMElement *element = shape->createElement(document);
            if (element) {
                shapes->appendChild(element);
            }
        }
    }
}

void ProcessStudio::initEdge() {
    std::vector<std::shared_ptr<Edge>> shapes = _graphics->getShapes<Edge>();
    for (auto shape: shapes) {
        if (!shape->_startShapeID.empty()) {
            shape->setStartShape(_graphics->getShapeByID(shape->_startShapeID));
        }
        if (!shape->_endShapeID.empty()) {
            shape->setEndShape(_graphics->getShapeByID(shape->_endShapeID));
        }
    }
}

void ProcessStudio::clear() {
    _currentSelectShape = nullptr;
    _tasks.clear();
    _graphics->clear();
    _process = nullptr;
    _taskShapes.clear();
    _ruleShapeItems.clear();
    _processInfoVariant = rttr::type::get_by_name("ProcesInfo").create();
}

DumyTaskItem::DumyTaskItem(const std::shared_ptr<Process::Task> &task, const rttr::type &taskType,
                           const rttr::variant &taskVar) : _task(task), _taskType(taskType), _taskVar(taskVar) {}

DumyShapeItem::DumyShapeItem(const std::shared_ptr<Shape> &shape, const rttr::type &shapeType,
                             const rttr::variant &shapeVar) : _shape(shape), _shapeType(shapeType),
                                                              _shapeVar(shapeVar) {}
