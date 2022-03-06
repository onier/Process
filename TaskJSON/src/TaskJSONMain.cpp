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
#include "TestTask1.h"
std::string uuid() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return boost::lexical_cast<std::string>(uuid);
}

#include "XML.h"
#include "rttr/library.h"
#include "EventGateway.h"
int main2() {
//    EventGateway  eventGateway;
//    eventGateway._eventRules.insert({"A","A"});
//    eventGateway._eventRules.insert({"B","B"});
//    LOG(INFO)<<puppy::common::XML::toXMLString(eventGateway);
//    return 1;
//    std::shared_ptr<ParallelGateway> parallelTask = std::make_shared<ParallelGateway>();
//    std::shared_ptr<Process::TaskManager> taskManager = std::make_shared<Process::TaskManager>();
/**
 *                      startTask
 *                          |
 *                     ExceptionTask
 *                          |
 *                      EventGateway
 *                         /\
 *                EventA  /  \EventB
 *                     Test1 test2
 *                        |
 *                        |
 *                      EndTask
 */
 std::string text ="<Process xmlns=\"\">\n"
                   "\n"
                   "    <TaskManager taskName=\"mainTask\">\n"
                   "        <tasks>\n"
                   "            <StartTask NextTaskID=\"0c8858ec-df51-4fca-8d09-85b2bf27f0d7\" id=\"9b823fca-56e9-4808-b8f8-4be2a10ceacf\" name=\"startTask\"/>\n"
                   "            <ExceptionTask id=\"0c8858ec-df51-4fca-8d09-85b2bf27f0d7\" name=\"exceptionTask\" NextTaskID=\"9b823fca-56e9-4808-b8f8-4be2a10ceaff\"/>\n"
                   "            <EventGateway id=\"9b823fca-56e9-4808-b8f8-4be2a10ceaff\" name=\"exceptionTask\">\n"
                   "\n"
                   "                <map keyType=\"std::string\" name=\"EventRules\" valueType=\"std::string\">\n"
                   "                    <item>\n"
                   "                        <key type=\"std::string\" value=\"EventA\"/>\n"
                   "                        <value type=\"std::string\" value=\"f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1\"/>\n"
                   "                    </item>\n"
                   "                    <item>\n"
                   "                        <key type=\"std::string\" value=\"EventB\"/>\n"
                   "                        <value type=\"std::string\" value=\"f31eea0c-9d59-47b5-8c4f-8317de76453e\"/>\n"
                   "                    </item>\n"
                   "                </map>\n"
                   "\n"
                   "            </EventGateway>\n"
                   "<!--            <TestTask1 NextTaskID=\"496addbb-8d82-4ce1-b829-4c0e0b2b680f\" PreTaskID=\"9b823fca-56e9-4808-b8f8-4be2a10ceacf\" id=\"f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1\" name=\"task1\"/>-->\n"
                   "            <SubProcessTask NextTaskID=\"496addbb-8d82-4ce1-b829-4c0e0b2b680f\" PreTaskID=\"9b823fca-56e9-4808-b8f8-4be2a10ceacf\" id=\"f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1\" name=\"subProcessTask\">\n"
                   "                <SubTaskManager taskName=\"subTask\">\n"
                   "                    <tasks>\n"
                   "                        <StartTask NextTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"bc3045c9-2a63-4b82-8977-1e620519ed6b\" name=\"startTask\"/>\n"
                   "                        <ParallelGateway PreTaskID=\"bc3045c9-2a63-4b82-8977-1e620519ed6b\" id=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" name=\"startParallelTask\">\n"
                   "                            <list name=\"OutTasks\" type=\"std::string\">\n"
                   "                                <std::string value=\"57eccfb9-472a-4151-a3e1-56b235b11185\"/>\n"
                   "                                <std::string value=\"b325ceed-441d-4fc8-ab95-7015c501aca5\"/>\n"
                   "                                <std::string value=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\"/>\n"
                   "                            </list>\n"
                   "                        </ParallelGateway>\n"
                   "                        <ParallelGateway NextTaskID=\"3d12eb05-ec40-4b40-983d-e08bea75e455\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" name=\"endParallelTask\">\n"
                   "                            <list name=\"InTasks\" type=\"std::string\">\n"
                   "                                <std::string value=\"57eccfb9-472a-4151-a3e1-56b235b11185\"/>\n"
                   "                                <std::string value=\"b325ceed-441d-4fc8-ab95-7015c501aca5\"/>\n"
                   "                                <std::string value=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\"/>\n"
                   "                            </list>\n"
                   "                        </ParallelGateway>\n"
                   "                        <TestTask1 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"57eccfb9-472a-4151-a3e1-56b235b11185\" name=\"task1\"/>\n"
                   "                        <TestTask2 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"b325ceed-441d-4fc8-ab95-7015c501aca5\" name=\"task2\"/>\n"
                   "                        <TestTask3 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\" name=\"task3\"/>\n"
                   "                        <EndTask PreTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" id=\"3d12eb05-ec40-4b40-983d-e08bea75e455\" name=\"task after endParallelTask \"/>\n"
                   "                    </tasks>\n"
                   "                </SubTaskManager>\n"
                   "            </SubProcessTask>\n"
                   "            <TestTask2 NextTaskID=\"f627851e-5e78-4788-af0b-4f1b5eab9b40\" PreTaskID=\"9b823fca-56e9-4808-b8f8-4be2a10ceacf\" id=\"f31eea0c-9d59-47b5-8c4f-8317de76453e\" name=\"task2\"/>\n"
                   "            <EndTask NextTaskID=\"\" PreTaskID=\"f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1\" id=\"f627851e-5e78-4788-af0b-4f1b5eab9b40\" name=\"task3\"/>\n"
                   "        </tasks>\n"
                   "    </TaskManager>\n"
                   "\n"
                   "</Process>";
    /*   排他  {"taskValue", 2.0} 流程结构图  选择条件为1-->task1  2-->task2  3-->task3
 *                      startTask
 *                          |
 *                          |
 *                     ExclusiveGateway
 *                         / | \
 *                       /   |  \
 *                     /     |   \
 *                   task1  task2 task3
 *                     \     |      /
 *                      \    |     /
 *                       \   |    /
 *                     ExclusiveGateway
 *                           |
 *                 task after endExclusiveTask
 */
//    std::string text =  "<Process xmlns=\"\">\n"
//                        "\n"
//                        "    <TaskManager taskName=\"mainTask\">\n"
//                        "        <tasks>\n"
//                        "            <StartTask NextTaskID=\"aba55168-f43f-41e1-87ae-576e3f527962\" id=\"13fc91df-db76-4b9c-9179-3fde6c062aa0\" name=\"startTask\"/>\n"
//                        "            <ExclusiveGateway PreTaskID=\"13fc91df-db76-4b9c-9179-3fde6c062aa0\" id=\"aba55168-f43f-41e1-87ae-576e3f527962\" name=\"startExclusiveTask\">\n"
//                        "                <list name=\"SubTasks\" type=\"ExclusiveRule\">\n"
//                        "                    <ExclusiveRule Operator=\"=\" TaskID=\"9bd60356-23be-4604-89fd-cd82ba0c3bc1\" Value=\"1\" ValueName=\"taskValue\"/>\n"
//                        "                    <ExclusiveRule Operator=\"=\" TaskID=\"79cc50ef-cda5-427c-b635-42987fcaf9ef\" Value=\"2\" ValueName=\"taskValue\"/>\n"
//                        "                    <ExclusiveRule Operator=\"=\" TaskID=\"3adf4316-2679-4311-bd18-2e1d6582c4ef\" Value=\"3\" ValueName=\"taskValue\"/>\n"
//                        "                </list>\n"
//                        "            </ExclusiveGateway>\n"
//                        "            <ExclusiveGateway NextTaskID=\"d88d4ad6-fb5f-4122-8ede-504481bc09d9\" PreTaskID=\"13fc91df-db76-4b9c-9179-3fde6c062aa0\" id=\"f0d24bb9-336f-4026-96ee-fffcc2146eda\" name=\"endExclusiveTask\"/>\n"
//                        "            <TestTask1 NextTaskID=\"f0d24bb9-336f-4026-96ee-fffcc2146eda\" PreTaskID=\"aba55168-f43f-41e1-87ae-576e3f527962\" id=\"9bd60356-23be-4604-89fd-cd82ba0c3bc1\" name=\"task1\"/>\n"
//                        "            <ExceptionTask NextTaskID=\"f0d24bb9-336f-4026-96ee-fffcc2146eda\" PreTaskID=\"aba55168-f43f-41e1-87ae-576e3f527962\" id=\"79cc50ef-cda5-427c-b635-42987fcaf9ef\" name=\"task2\"/>\n"
//                        "            <TestTask3 NextTaskID=\"f0d24bb9-336f-4026-96ee-fffcc2146eda\" PreTaskID=\"aba55168-f43f-41e1-87ae-576e3f527962\" id=\"3adf4316-2679-4311-bd18-2e1d6582c4ef\" name=\"task3\"/>\n"
//                        "            <EndTask PreTaskID=\"f0d24bb9-336f-4026-96ee-fffcc2146eda\" id=\"d88d4ad6-fb5f-4122-8ede-504481bc09d9\" name=\"task after endExclusiveTask \"/>\n"
//                        "        </tasks>\n"
//                        "    </TaskManager>\n"
//                        "\n"
//                        "</Process>";

/*   并行流程结构图
 *                      startTask
 *                          |
 *                          |
 *                     ParallelGateway
 *                         / | \
 *                       /   |  \
 *                     /     |   \
 *                   task1  task2 task3
 *                     \     |      /
 *                      \    |     /
 *                       \   |    /
 *                     ParallelGateway
 *                           |
 *                 task after endParallelTask
 */
//    std::string text = "<Process xmlns=\"\">\n"
//                       "\n"
//                       "<TaskManager  taskName=\"mainTask\">\n"
//                       "    <tasks>\n"
//                       "        <StartTask NextTaskID=\"0c8858ec-df51-4fca-8d09-85b2bf27f0d7\" id=\"9b823fca-56e9-4808-b8f8-4be2a10ceacf\" name=\"startTask\"/>\n"
//                       "        <ParallelGateway PreTaskID=\"9b823fca-56e9-4808-b8f8-4be2a10ceacf\" id=\"0c8858ec-df51-4fca-8d09-85b2bf27f0d7\" name=\"startParallelTask\">\n"
//                       "            <list name=\"OutTasks\" type=\"std::string\">\n"
//                       "                <std::string value=\"f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1\"/>\n"
//                       "                <std::string value=\"f31eea0c-9d59-47b5-8c4f-8317de76453e\"/>\n"
//                       "                <std::string value=\"f627851e-5e78-4788-af0b-4f1b5eab9b40\"/>\n"
//                       "            </list>\n"
//                       "        </ParallelGateway>\n"
//                       "        <TestTask1 NextTaskID=\"496addbb-8d82-4ce1-b829-4c0e0b2b680f\" PreTaskID=\"0c8858ec-df51-4fca-8d09-85b2bf27f0d7\" id=\"f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1\" name=\"task1\"/>\n"
//                       "        <TestTask2 NextTaskID=\"496addbb-8d82-4ce1-b829-4c0e0b2b680f\" PreTaskID=\"0c8858ec-df51-4fca-8d09-85b2bf27f0d7\" id=\"f31eea0c-9d59-47b5-8c4f-8317de76453e\" name=\"task2\"/>\n"
//                       "        <TestTask3 NextTaskID=\"d954d497-643a-4389-9f56-543732f473dc\" PreTaskID=\"0c8858ec-df51-4fca-8d09-85b2bf27f0d7\" id=\"f627851e-5e78-4788-af0b-4f1b5eab9b40\" name=\"task3\"/>\n"
//                       "        <EndTask PreTaskID=\"496addbb-8d82-4ce1-b829-4c0e0b2b680f\" id=\"d954d497-643a-4389-9f56-543732f473dc\" name=\"task after endParallelTask \"/>\n"
//                       "    </tasks>\n"
//                       "</TaskManager>\n"
//                       "\n"
//                       "</Process>";

/** 流程如下中间为一个subprocessTask包含一个并行子进程
 *                      startTask
 *                          |
 *          -----------------------------------
 *          |            startTask             |
 *          |               |                  |
 *          |                |                 |
 *          |          ParallelGateway            |
 *          |               / | \              |
 *          |             /   |  \             |
 *          |           /     |   \            |
 *          |         task1  task2 task3       |
 *          |           \     |      /         |
 *          |            \    |     /          |
 *          |             \   |    /           |
 *          |           ParallelGateway           |
 *          |                 |                |
 *          |       task after endParallelTask |
 *          ------------------------------------
 *                            |
 *                        endTask
 */
//    std::string text = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
//                       "<Process xmlns=\"\">\n"
//                       "\n"
//                       "    <TaskManager taskName=\"mainTask\">\n"
//                       "        <tasks>\n"
//                       "            <StartTask NextTaskID=\"07673c72-f698-4741-b98f-f2ff6ae59f47\" id=\"07673c72-f698-4741-b98f-f2ff6ae59f48\" name=\"startTask\"/>\n"
//                       "            <SubProcessTask NextTaskID=\"647da05a-9cfd-40b5-b483-22f2b14e4a9c\" PreTaskID=\"07673c72-f698-4741-b98f-f2ff6ae59f47\" id=\"07673c72-f698-4741-b98f-f2ff6ae59f47\" name=\"subProcessTask\">\n"
//                       "                <SubTaskManager taskName=\"subTask\">\n"
//                       "                    <tasks>\n"
//                       "                        <StartTask NextTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"bc3045c9-2a63-4b82-8977-1e620519ed6b\" name=\"startTask\"/>\n"
//                       "                        <ParallelGateway PreTaskID=\"bc3045c9-2a63-4b82-8977-1e620519ed6b\" id=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" name=\"startParallelTask\">\n"
//                       "                            <list name=\"OutTasks\" type=\"std::string\">\n"
//                       "                                <std::string value=\"57eccfb9-472a-4151-a3e1-56b235b11185\"/>\n"
//                       "                                <std::string value=\"b325ceed-441d-4fc8-ab95-7015c501aca5\"/>\n"
//                       "                                <std::string value=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\"/>\n"
//                       "                            </list>\n"
//                       "                        </ParallelGateway>\n"
//                       "                        <ParallelGateway NextTaskID=\"3d12eb05-ec40-4b40-983d-e08bea75e455\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" name=\"endParallelTask\">\n"
//                       "                            <list name=\"InTasks\" type=\"std::string\">\n"
//                       "                                <std::string value=\"57eccfb9-472a-4151-a3e1-56b235b11185\"/>\n"
//                       "                                <std::string value=\"b325ceed-441d-4fc8-ab95-7015c501aca5\"/>\n"
//                       "                                <std::string value=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\"/>\n"
//                       "                            </list>\n"
//                       "                        </ParallelGateway>\n"
//                       "                        <TestTask1 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"57eccfb9-472a-4151-a3e1-56b235b11185\" name=\"task1\"/>\n"
//                       "                        <TestTask2 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"b325ceed-441d-4fc8-ab95-7015c501aca5\" name=\"task2\"/>\n"
//                       "                        <TestTask3 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\" name=\"task3\"/>\n"
//                       "                        <EndTask PreTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" id=\"3d12eb05-ec40-4b40-983d-e08bea75e455\" name=\"task after endParallelTask \"/>\n"
//                       "                    </tasks>\n"
//                       "                </SubTaskManager>\n"
//                       "            </SubProcessTask>\n"
//                       "            <EndTask PreTaskID=\"07673c72-f698-4741-b98f-f2ff6ae59f47\" id=\"647da05a-9cfd-40b5-b483-22f2b14e4a9c\" name=\"endTask\"/>\n"
//                       "        </tasks>\n"
//                       "    </TaskManager>\n"
//                       "\n"
//                       "</Process>";
//    taskManager->loadXML(text);
    LOG(INFO)<<text;
    Process::Process process(1);
    process.loadXML(text);
    process.initProcessValues({{"taskValue", 2.0}});
//    folly::Synchronized<std::map<std::string, boost::any>> values;
//    values.wlock()->insert({"taskValue", 2.0});
    process.startProcess(nullptr);
//    LOG(INFO)<<process.saveXML();
//    auto t = rttr::type::get_by_name("TestTask1");
//    LOG(INFO) << puppy::common::XML::toXMLString(t);
    sleep(1111);
//    folly::Synchronized<std::map<std::string, boost::any>> values;
//    LOG(INFO) << puppy::common::JSON::toJSONString(*task1);
//    auto t1 = rttr::type::get_by_name("TestTask1").create().get_value<std::shared_ptr<TestTask1>>();
//    t1->run(values);
}
#include "fstream"

std::string readFile(std::string path) {
    std::string line, content;
    std::ifstream myfile(path);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            content += line + '\n';
        }
        myfile.close();
    }
    return content;
}
#include "FakeTask.h"
#include "ExclusiveGateway.h"

int main(){
    FakeTask fakeTask;
    fakeTask._values.insert({"A","A"});
    LOG(INFO)<<puppy::common::XML::toXMLString(fakeTask);
//    ExclusiveGateway exclusiveGateway;
//    exclusiveGateway._rules.insert({"A","B"});
//    exclusiveGateway._rules.insert({"C","C"});
//    LOG(INFO)<<puppy::common::XML::toXMLString(exclusiveGateway);
    std::string text = readFile("/home/tian/fuxios/Process/TaskJSON/src/slave.xml");
    Process::Process process(1);
    process.loadXML(text);
    process.initProcessValues({{"taskValue", 2.0}});
    //    folly::Synchronized<std::map<std::string, boost::any>> values;
    //    values.wlock()->insert({"taskValue", 2.0});
    process.startProcess(nullptr);
    //    LOG(INFO)<<process.saveXML();
    //    auto t = rttr::type::get_by_name("TestTask1");
    //    LOG(INFO) << puppy::common::XML::toXMLString(t);
    sleep(1111);
}
//int main1(){
//    std::string text = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
//                       "<Process xmlns=\"\">\n"
//                       "\n"
//                       "  <TaskManager>\n"
//                       "    <tasks>\n"
//                       "      <TestTask1 NextTaskID=\"07673c72-f698-4741-b98f-f2ff6ae59f47\" id=\"07673c72-f698-4741-b98f-f2ff6ae59f48\" name=\"startTask\"/>\n"
//                       "      <SubProcessTask NextTaskID=\"647da05a-9cfd-40b5-b483-22f2b14e4a9c\" PreTaskID=\"07673c72-f698-4741-b98f-f2ff6ae59f47\" id=\"07673c72-f698-4741-b98f-f2ff6ae59f47\" name=\"subProcessTask\">\n"
//                       "        <SubTaskManager>\n"
//                       "          <tasks>\n"
//                       "            <TestTask1 NextTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"bc3045c9-2a63-4b82-8977-1e620519ed6b\" name=\"startTask\"/>\n"
//                       "            <ParallelGateway PreTaskID=\"bc3045c9-2a63-4b82-8977-1e620519ed6b\" id=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" name=\"startParallelTask\">\n"
//                       "              <list name=\"OutTasks\" type=\"std::string\">\n"
//                       "                <std::string value=\"57eccfb9-472a-4151-a3e1-56b235b11185\"/>\n"
//                       "                <std::string value=\"b325ceed-441d-4fc8-ab95-7015c501aca5\"/>\n"
//                       "                <std::string value=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\"/>\n"
//                       "              </list>\n"
//                       "            </ParallelGateway>\n"
//                       "            <ParallelGateway NextTaskID=\"3d12eb05-ec40-4b40-983d-e08bea75e455\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" name=\"endParallelTask\">\n"
//                       "              <list name=\"InTasks\" type=\"std::string\">\n"
//                       "                <std::string value=\"57eccfb9-472a-4151-a3e1-56b235b11185\"/>\n"
//                       "                <std::string value=\"b325ceed-441d-4fc8-ab95-7015c501aca5\"/>\n"
//                       "                <std::string value=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\"/>\n"
//                       "              </list>\n"
//                       "            </ParallelGateway>\n"
//                       "            <TestTask1 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"57eccfb9-472a-4151-a3e1-56b235b11185\" name=\"task1\"/>\n"
//                       "            <TestTask2 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"b325ceed-441d-4fc8-ab95-7015c501aca5\" name=\"task2\"/>\n"
//                       "            <TestTask3 NextTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" PreTaskID=\"a92c2f45-dd70-4440-85c7-58068ae460bc\" id=\"46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1\" name=\"task3\"/>\n"
//                       "            <TestTask3 PreTaskID=\"5aa80f88-244c-4541-9856-2fdaf1c80942\" id=\"3d12eb05-ec40-4b40-983d-e08bea75e455\" name=\"task after endParallelTask \"/>\n"
//                       "          </tasks>\n"
//                       "        </SubTaskManager>\n"
//                       "      </SubProcessTask>\n"
//                       "      <TestTask1 PreTaskID=\"07673c72-f698-4741-b98f-f2ff6ae59f47\" id=\"647da05a-9cfd-40b5-b483-22f2b14e4a9c\" name=\"endTask\"/>\n"
//                       "    </tasks>\n"
//                       "  </TaskManager>\n"
//                       "\n"
//                       "</Process>";
//    std::shared_ptr<Process::TaskManager> taskManager = std::make_shared<Process::TaskManager>();
//    taskManager->loadXML(text);
//    Process::Process process(taskManager);
//    process._processValues.wlock()->insert({"taskValue", 2.0});
//    folly::Synchronized<std::map<std::string, boost::any>> values;
//    process.startProcess(values);
//    LOG(INFO)<<taskManager->saveXML();
////    auto t = rttr::type::get_by_name("TestTask1");
////    LOG(INFO) << puppy::common::XML::toXMLString(t);
//    sleep(1111);
//}