//
// Created by xuzhenhai on 2021/10/21.
//

#include "JSON.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "boost/lexical_cast.hpp"
#include "glog/logging.h"
#include "Process.h"
#include "ParallelGateway.h"
#include "TaskManager.h"
#include "rttr/registration.h"
#include "ParallelGateway.h"
#include "TestTask1.h"
#include "TestTask2.h"
#include "TestTask3.h"
#include "SubProcessTask.h"
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

std::string uuid() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return boost::lexical_cast<std::string>(uuid);
}

#include "XML.h"
#include "rttr/library.h"

std::string toStr(const XMLCh *xmlch) {
    if (xmlch) {
        char *temp = xercesc::XMLString::transcode(xmlch);
        std::string value(temp);
        xercesc::XMLString::release(&temp);
        return value;
    }
    return "        ";
}

bool testXML() {
    xercesc::XMLPlatformUtils::Initialize();
    std::string xmlContent = "<TaskManager xmlns=\"\">\n"
                             "\n"
                             "  <tasks>\n"
                             "    <TestTask1 NextTaskID=\"f5e6de95-9948-4a5d-9e12-775d11257bd5\" id=\"414c15f6-3030-452f-a251-61cee877e0db\" name=\"startTask\"/>\n"
                             "    <SubProcessTask NextTaskID=\"78b767ad-42ed-4b05-a5ff-779468cd9daa\" PreTaskID=\"414c15f6-3030-452f-a251-61cee877e0db\" id=\"f5e6de95-9948-4a5d-9e12-775d11257bd5\"/>\n"
                             "    <TestTask1 PreTaskID=\"f5e6de95-9948-4a5d-9e12-775d11257bd5\" id=\"78b767ad-42ed-4b05-a5ff-779468cd9daa\" name=\"endTask\"/>\n"
                             "  </tasks>\n"
                             "\n"
                             "</TaskManager>";
    xercesc::XercesDOMParser xercesDOMParser;
    std::shared_ptr<xercesc::MemBufInputSource> memBufIS = std::shared_ptr<xercesc::MemBufInputSource>(
            new xercesc::MemBufInputSource((const XMLByte *) xmlContent.data(), xmlContent.size(), ""));
    xercesDOMParser.parse(*memBufIS);
    auto document = xercesDOMParser.getDocument();
    auto root = document->getDocumentElement();
    auto taskElements = root->getElementsByTagName(XStr("tasks"));
    if (taskElements->getLength() == 1) {
        auto tasks = taskElements->item(0)->getChildNodes();
        LOG(INFO) << tasks->getLength();
        for (int i = 0; i < tasks->getLength(); ++i) {
            auto taskTypeName = toStr(tasks->item(i)->getNodeName());
            auto taskType = rttr::type::get_by_name(taskTypeName);
            LOG(INFO) << toStr(tasks->item(i)->getNodeName()) << " " << taskType.is_valid();
            if (taskType.is_valid()) {
                auto taskVariant = taskType.create();
                puppy::common::XML::parseInstance(tasks->item(i),taskVariant);
            }
        }
    }
    return false;
}

int main() {
    SubProcessTask subProcessTask1;
    std::vector<std::shared_ptr<Process::AbstractTask>> tasks;
    std::shared_ptr<Process::TaskManager> taskManager = std::make_shared<Process::TaskManager>();

    std::shared_ptr<TestTask1> startTask = std::make_shared<TestTask1>();
    tasks.push_back(startTask);
    startTask->_id = uuid();
    startTask->_name = "startTask";

    {
        std::string text = "{\n"
                           "  \"Tasks\": [\n"
                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask1 NextTaskID=\\\"a92c2f45-dd70-4440-85c7-58068ae460bc\\\" id=\\\"bc3045c9-2a63-4b82-8977-1e620519ed6b\\\" name=\\\"startTask\\\" xmlns=\\\"\\\"/>\\n\",\n"
                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<ParallelGateway PreTaskID=\\\"bc3045c9-2a63-4b82-8977-1e620519ed6b\\\" id=\\\"a92c2f45-dd70-4440-85c7-58068ae460bc\\\" name=\\\"startParallelTask\\\" xmlns=\\\"\\\">\\n\\n  <list name=\\\"OutTasks\\\" type=\\\"std::string\\\">\\n    <std::string value=\\\"57eccfb9-472a-4151-a3e1-56b235b11185\\\"/>\\n    <std::string value=\\\"b325ceed-441d-4fc8-ab95-7015c501aca5\\\"/>\\n    <std::string value=\\\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\\\"/>\\n  </list>\\n\\n</ParallelGateway>\\n\",\n"
                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<ParallelGateway NextTaskID=\\\"3d12eb05-ec40-4b40-983d-e08bea75e455\\\" PreTaskID=\\\"a92c2f45-dd70-4440-85c7-58068ae460bc\\\" id=\\\"5aa80f88-244c-4541-9856-2fdaf1c80942\\\" name=\\\"endParallelTask\\\" xmlns=\\\"\\\">\\n\\n  <list name=\\\"InTasks\\\" type=\\\"std::string\\\">\\n    <std::string value=\\\"57eccfb9-472a-4151-a3e1-56b235b11185\\\"/>\\n    <std::string value=\\\"b325ceed-441d-4fc8-ab95-7015c501aca5\\\"/>\\n    <std::string value=\\\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\\\"/>\\n  </list>\\n\\n</ParallelGateway>\\n\",\n"
                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask1 NextTaskID=\\\"5aa80f88-244c-4541-9856-2fdaf1c80942\\\" PreTaskID=\\\"a92c2f45-dd70-4440-85c7-58068ae460bc\\\" id=\\\"57eccfb9-472a-4151-a3e1-56b235b11185\\\" name=\\\"task1\\\" xmlns=\\\"\\\"/>\\n\",\n"
                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask2 NextTaskID=\\\"5aa80f88-244c-4541-9856-2fdaf1c80942\\\" PreTaskID=\\\"a92c2f45-dd70-4440-85c7-58068ae460bc\\\" id=\\\"b325ceed-441d-4fc8-ab95-7015c501aca5\\\" name=\\\"task2\\\" xmlns=\\\"\\\"/>\\n\",\n"
                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask3 NextTaskID=\\\"5aa80f88-244c-4541-9856-2fdaf1c80942\\\" PreTaskID=\\\"a92c2f45-dd70-4440-85c7-58068ae460bc\\\" id=\\\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\\\" name=\\\"task3\\\" xmlns=\\\"\\\"/>\\n\",\n"
                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask3 PreTaskID=\\\"5aa80f88-244c-4541-9856-2fdaf1c80942\\\" id=\\\"3d12eb05-ec40-4b40-983d-e08bea75e455\\\" name=\\\"task after endParallelTask \\\" xmlns=\\\"\\\"/>\\n\"\n"
                           "  ]\n"
                           "}";

//        std::string text = "{\n"
//                           "  \"Tasks\": [\n"
//                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask1 NextTaskID=\\\"c3cc4569-ae3c-4318-898d-bfa54ef0143f\\\" id=\\\"e201883b-64ff-46bc-a831-11f2d4aa9544\\\" name=\\\"task1\\\" xmlns=\\\"\\\"/>\\n\",\n"
//                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask2 NextTaskID=\\\"e2f41aa8-4c58-44eb-99bc-87573288b2b1\\\" PreTaskID=\\\"e201883b-64ff-46bc-a831-11f2d4aa9544\\\" id=\\\"c3cc4569-ae3c-4318-898d-bfa54ef0143f\\\" name=\\\"task2\\\" xmlns=\\\"\\\"/>\\n\",\n"
//                           "    \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask3 PreTaskID=\\\"c3cc4569-ae3c-4318-898d-bfa54ef0143f\\\" id=\\\"e2f41aa8-4c58-44eb-99bc-87573288b2b1\\\" name=\\\"task3\\\" xmlns=\\\"\\\"/>\\n\"\n"
//                           "  ]\n"
//                           "}";

        std::shared_ptr<SubProcessTask> subProcessTask = std::make_shared<SubProcessTask>();
        subProcessTask->_subTaskProcess = text;
        subProcessTask->initTask({});
        tasks.push_back(subProcessTask);
        subProcessTask->_id = uuid();
        startTask->_nextTaskID = subProcessTask->getID();
        subProcessTask->_preTaskID = startTask->getID();

        std::shared_ptr<TestTask1> endTask = std::make_shared<TestTask1>();
        tasks.push_back(endTask);
        endTask->_id = uuid();
        endTask->_name = "endTask";
        subProcessTask->_nextTaskID = endTask->getID();
        endTask->_preTaskID = subProcessTask->getID();
    }
    taskManager->_tasks = tasks;
    LOG(INFO) << taskManager->saveXML();
    Process::Process process(taskManager);
    process._processValues.wlock()->insert({"taskValue", 3.0});
    folly::Synchronized<std::map<std::string, boost::any>> values;
    process.startProcess(values);

    sleep(1111);
}

int main1() {
    std::vector<std::shared_ptr<Process::AbstractTask>> tasks;

    std::shared_ptr<TestTask1> startTask = std::make_shared<TestTask1>();
    tasks.push_back(startTask);
    startTask->_id = uuid();
    startTask->_name = "startTask";

    std::shared_ptr<ParallelGateway> startParallelTask = std::make_shared<ParallelGateway>();
    std::shared_ptr<ParallelGateway> endParallelTask = std::make_shared<ParallelGateway>();
    tasks.push_back(startParallelTask);
    tasks.push_back(endParallelTask);
    {
        endParallelTask->_id = uuid();
        endParallelTask->_name = "endParallelTask";
    }
    {
        startParallelTask->_id = uuid();
        startParallelTask->_name = "startParallelTask";
        startParallelTask->_preTaskID = startTask->_id;
        startTask->_nextTaskID = startParallelTask->_id;
    }

    std::shared_ptr<TestTask1> task1 = std::make_shared<TestTask1>();
    tasks.push_back(task1);
    task1->_id = uuid();
    task1->_name = "task1";
    task1->_preTaskID = startParallelTask->getID();
    task1->_nextTaskID = endParallelTask->getID();

    std::shared_ptr<TestTask2> task2 = std::make_shared<TestTask2>();
    tasks.push_back(task2);
    task2->_id = uuid();
    task2->_name = "task2";
    task2->_preTaskID = startParallelTask->getID();
    task2->_nextTaskID = endParallelTask->getID();

    std::shared_ptr<TestTask3> task3 = std::make_shared<TestTask3>();
    tasks.push_back(task3);
    task3->_id = uuid();
    task3->_name = "task3";
    task3->_preTaskID = startParallelTask->getID();
    task3->_nextTaskID = endParallelTask->getID();

    startParallelTask->_outTasks.push_back(task1->getID());
    startParallelTask->_outTasks.push_back(task2->getID());
    startParallelTask->_outTasks.push_back(task3->getID());

    endParallelTask->_inTasks.push_back(task1->getID());
    endParallelTask->_inTasks.push_back(task2->getID());
    endParallelTask->_inTasks.push_back(task3->getID());

    std::shared_ptr<TestTask3> task4 = std::make_shared<TestTask3>();
    tasks.push_back(task4);
    task4->_id = uuid();
    task4->_name = "task after endParallelTask ";
    task4->_preTaskID = endParallelTask->getID();
    endParallelTask->_nextTaskID = task4->getID();
    endParallelTask->_preTaskID = startParallelTask->getID();

    std::shared_ptr<Process::TaskManager> taskManager = std::make_shared<Process::TaskManager>();
    taskManager->_tasks = tasks;
    LOG(INFO) << taskManager->saveXML();
//    std::shared_ptr<ParallelGateway> parallelTask = std::make_shared<ParallelGateway>();
//    std::shared_ptr<Process::TaskManager> taskManager = std::make_shared<Process::TaskManager>();
//    std::string text = "{\n"
//                       "    \"Tasks\": [\n"
//                       "        \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask1 NextTaskID=\\\"c3cc4569-ae3c-4318-898d-bfa54ef0143f\\\" id=\\\"e201883b-64ff-46bc-a831-11f2d4aa9544\\\" name=\\\"task1\\\" xmlns=\\\"\\\"/>\\n\",\n"
//                       "        \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask2 NextTaskID=\\\"e2f41aa8-4c58-44eb-99bc-87573288b2b1\\\" PreTaskID=\\\"e201883b-64ff-46bc-a831-11f2d4aa9544\\\" id=\\\"c3cc4569-ae3c-4318-898d-bfa54ef0143f\\\" name=\\\"task2\\\" xmlns=\\\"\\\"/>\\n\",\n"
//                       "        \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"no\\\" ?>\\n<TestTask3 PreTaskID=\\\"c3cc4569-ae3c-4318-898d-bfa54ef0143f\\\" id=\\\"e2f41aa8-4c58-44eb-99bc-87573288b2b1\\\" name=\\\"task2\\\" xmlns=\\\"\\\"/>\\n\"\n"
//                       "    ]\n"
//                       "}";
//    LOG(INFO)<<text;
//    taskManager->load(text);
    Process::Process process(taskManager);
    process._processValues.wlock()->insert({"taskValue", 3.0});
    folly::Synchronized<std::map<std::string, boost::any>> values;
    process.startProcess(values);
//    auto t = rttr::type::get_by_name("TestTask1");
//    LOG(INFO) << puppy::common::XML::toXMLString(t);

    sleep(1111);
//    folly::Synchronized<std::map<std::string, boost::any>> values;
//    LOG(INFO) << puppy::common::JSON::toJSONString(*task1);
//    auto t1 = rttr::type::get_by_name("TestTask1").create().get_value<std::shared_ptr<TestTask1>>();
//    t1->run(values);
}