//
// Created by xuzhenhai on 2022/2/19.
//

#include "ProcessContext.h"
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
#include "XML.h"

using namespace Process;

ProcessContext::ProcessContext(int threadCount) {
    _executor = std::make_shared<folly::CPUThreadPoolExecutor>(threadCount);
    _processValues = std::make_shared<folly::Synchronized<std::map<std::string, boost::any>>>();
}

bool ProcessContext::saveDomElement(xercesc::DOMElement *domElement, std::shared_ptr<xercesc::DOMDocument> document) {
    auto tasks = document->createElement(XStr("tasks"));
    domElement->appendChild(tasks);
    for (auto &t: _tasks) {
        auto taskElement = document->createElement(XStr(t->get_type().get_name().data()));
        tasks->appendChild(taskElement);
        createElement(t, taskElement, document.get(), t);
        t->saveDomElement(taskElement, document);
    }
    return true;
}

bool ProcessContext::loadDomElement(xercesc::DOMNode *domElement) {
    auto taskManagerNode = domElement->getChildNodes();
    _taskName = puppy::common::XML::toStr(domElement->getAttributes()->getNamedItem(XStr("taskName"))->getNodeValue());
    for (int i = 0; i < taskManagerNode->getLength(); ++i) {
        auto name = puppy::common::XML::toStr(taskManagerNode->item(i)->getNodeName());
        if (name == "tasks") {
            auto tasks = taskManagerNode->item(i)->getChildNodes();
            for (int i = 0; i < tasks->getLength(); ++i) {
                auto taskTypeName = puppy::common::XML::toStr(tasks->item(i)->getNodeName());
                auto taskType = rttr::type::get_by_name(taskTypeName);
                if (taskType.is_valid()) {
                    auto taskVariant = taskType.create();
                    puppy::common::XML::parseInstance(tasks->item(i), taskVariant);
                    auto absPtr = taskVariant.get_value<std::shared_ptr<Process::Task>>();
                    absPtr->loadDomElement(tasks->item(i));
                    _tasks.push_back(absPtr);
                }
            }
        }
    }

    return true;
}

void ProcessContext::loadXML(std::string xml) {
    xercesc::XMLPlatformUtils::Initialize();
    xercesc::XercesDOMParser xercesDOMParser;
    std::shared_ptr<xercesc::MemBufInputSource> memBufIS = std::shared_ptr<xercesc::MemBufInputSource>(
            new xercesc::MemBufInputSource((const XMLByte *) xml.data(), xml.size(), ""));
    xercesDOMParser.parse(*memBufIS);
    auto document = xercesDOMParser.getDocument();
    auto root = document->getDocumentElement();
    auto process = root->getChildNodes();
    for (int i = 0; i < process->getLength(); ++i) {
        auto nodeName = puppy::common::XML::toStr(process->item(i)->getNodeName());
        if (nodeName == "TaskManager") {
            loadDomElement(process->item(i));
        }
    }
    for (auto &item:_tasks) {
        item->initTask(this);
        item->_taskName = _taskName;
    }
}

std::string ProcessContext::saveXML() {
    xercesc::XMLPlatformUtils::Initialize();
    xercesc::DOMImplementation *domImplementation = xercesc::DOMImplementationRegistry::getDOMImplementation(
            XStr("Core"));
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
    auto taskManager = document->createElement(XStr("TaskManager"));
    taskManager->setAttribute(XStr("taskName"), XStr(_taskName.data()));
    rootElement->appendChild(taskManager);
    saveDomElement(taskManager, document);
    document->normalizeDocument();
    serializer->write(document.get(), out);
    return std::string(reinterpret_cast<const char *>(formatTarget->getRawBuffer()));
}

std::vector<std::shared_ptr<Process::Task>> ProcessContext::getTasksByType(std::string type) {
    return std::vector<std::shared_ptr<Process::Task>>();
}

std::shared_ptr<Process::Task> ProcessContext::getStartTask() {
    std::vector<std::shared_ptr<Process::Task>> temps;
    for (auto &task: _tasks) {
        if (task->get_type().get_name() == "StartTask") {
            temps.push_back(task);
        }
    }
    if (temps.size() == 1) {
        return temps[0];
    } else {
        return nullptr;
    }
}

std::shared_ptr<Process::Task> ProcessContext::getTaskByID(std::string id) {
    for (auto &t: _tasks) {
        if (t->getID() == id) {
            return t;
        }
    }
    return nullptr;
}

std::shared_ptr<Process::Task> ProcessContext::getEndTask() {
    return std::shared_ptr<Process::Task>();
}

bool ProcessContext::initTasks() {
    return false;
}

void ProcessContext::createElement(rttr::instance obj2, xercesc::DOMElement *domElement, xercesc::DOMDocument *document,
                                   std::shared_ptr<Task> task) {
    rttr::instance variant = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;
    auto props = std::make_shared<rttr::array_range<rttr::property> >(variant.get_type().get_properties());
    if (task) {
        props = std::make_shared<rttr::array_range<rttr::property> >(task->get_type().get_properties());
    }
    for (auto prop:*props) {
        auto propName = prop.get_name();
        if (prop.get_type().is_sequential_container()) {
            auto vars = prop.get_value(variant);
            auto view = vars.create_sequential_view();
            if (view.get_size() > 0) {
                auto listElement = document->createElement(XStr("list"));
                listElement->setAttribute(XStr("type"),
                                          XStr(view.get_value(0).extract_wrapped_value().get_type().get_name().data()));
                listElement->setAttribute(XStr("name"), XStr(prop.get_name().data()));
                domElement->appendChild(listElement);
                for (auto index = 0; index < view.get_size(); index++) {
                    auto var = view.get_value(index);
                    if (var.operator!=(var.get_type().create())) {
                        auto element = document->createElement(
                                XStr(view.get_value(0).extract_wrapped_value().get_type().get_name().data()));
                        if (var.get_type().get_wrapped_type().get_name() == "std::string" ||
                            var.get_type().get_wrapped_type().is_arithmetic()) {
                            element->setAttribute(XStr("value"), XStr(var.to_string().data()));
                        } else {
                            createElement(var, element, document);
                        }
                        listElement->appendChild(element);
                    }
                }
            }
        } else if (prop.get_type().is_associative_container()) {
            auto vars = prop.get_value(variant);
            auto view = vars.create_associative_view();
            if (view.get_size() > 0) {
                auto mapElement = document->createElement(XStr("map"));
                mapElement->setAttribute(XStr("name"), XStr(prop.get_name().data()));
                auto it = view.begin();
                mapElement->setAttribute(XStr("keyType"),
                                         XStr(it.get_key().extract_wrapped_value().get_type().get_name().data()));
                mapElement->setAttribute(XStr("valueType"),
                                         XStr(it.get_value().extract_wrapped_value().get_type().get_name().data()));
                domElement->appendChild(mapElement);
                for (auto item:view) {
//                    LOG(INFO) << prop.get_name() << "  " << item.first.extract_wrapped_value().get_type().get_name()
//                              << "  " << item.first.to_string() << "  "
//                              << item.second.extract_wrapped_value().get_type().get_name() << "  "
//                              << item.second.to_string();
                    auto itemElement = document->createElement(XStr("item"));
                    auto keyElement = document->createElement(XStr("key"));
                    if (item.first.extract_wrapped_value().get_type().is_arithmetic() ||
                        item.first.extract_wrapped_value().get_type() == rttr::type::get<std::string>()) {
                        keyElement->setAttribute(XStr("type"),
                                                 XStr(item.first.extract_wrapped_value().get_type().get_name().data()));
                        keyElement->setAttribute(XStr("value"), XStr(item.first.to_string().data()));
                    } else {
                        createElement(item.first, keyElement, document);
                    }

                    auto valueElement = document->createElement(XStr("value"));
                    if (item.second.extract_wrapped_value().get_type().is_arithmetic() ||
                        item.second.extract_wrapped_value().get_type() == rttr::type::get<std::string>()) {
                        valueElement->setAttribute(XStr("type"),
                                                   XStr(item.second.extract_wrapped_value().get_type().get_name().data()));
                        valueElement->setAttribute(XStr("value"), XStr(item.second.to_string().data()));
                    } else {
                        createElement(item.second, valueElement, document);
                    }
                    itemElement->appendChild(keyElement);
                    itemElement->appendChild(valueElement);
                    mapElement->appendChild(itemElement);
                }
            }
        } else if (prop.get_name() == "value") {
            std::string string = prop.get_value(variant).to_string();
            if (!string.empty())
                domElement->setTextContent(XStr(prop.get_value(variant).to_string().data()));
        } else if (prop.get_type().get_raw_type().get_name() == "std::string" || prop.get_type().is_arithmetic() ||
                   prop.get_type().is_enumeration()) {
            auto value = prop.get_value(variant).to_string();
            if (!value.empty())
                domElement->setAttribute(XStr(prop.get_name().data()),
                                         XStr(value.data()));
        } else {
            auto var = prop.get_value(variant);
            if (var.operator!=(var.get_type().create())) {
                auto element = document->createElement(XStr(var.get_type().get_name().data()));
                domElement->appendChild(element);
                createElement(var, element, document);
            }
        }
    }
}
