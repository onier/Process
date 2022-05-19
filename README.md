# DummyProcess

#### 介绍
项目依赖于https://gitee.com/qq2820/puppy编译工程，目前只是一个简单的流程管理软件，项目依赖于folly，rttr等，目前已经提供了并行任务，串行任务，条件选择任务，子流程等任务。

#### 软件架构
Task为基础借口定义，以及通用的流程定义。

TaskImpl为几个测试的任务。

TaskJSON为从字符配置加载任务的测试代码,目前已经重构全部使用xml,TaskTest/src/目录下有个三个demo.xml文件可以参照。

TaskTest以代码方式构建流程，并保存为字符。
` <DummyProcess xmlns="">

    <TaskManager>
        <tasks>
            <TestTask1 NextTaskID="aba55168-f43f-41e1-87ae-576e3f527962" id="13fc91df-db76-4b9c-9179-3fde6c062aa0" name="startTask"/>
            <ExclusiveTask PreTaskID="13fc91df-db76-4b9c-9179-3fde6c062aa0" id="aba55168-f43f-41e1-87ae-576e3f527962" name="startExclusiveTask">
                <list name="SubTasks" type="ExclusiveRule">
                    <ExclusiveRule ExclusiveRuleOperator="=" TaskID="9bd60356-23be-4604-89fd-cd82ba0c3bc1" Value="1" ValueName="taskValue"/>
                    <ExclusiveRule ExclusiveRuleOperator="=" TaskID="79cc50ef-cda5-427c-b635-42987fcaf9ef" Value="2" ValueName="taskValue"/>
                    <ExclusiveRule ExclusiveRuleOperator="=" TaskID="3adf4316-2679-4311-bd18-2e1d6582c4ef" Value="3" ValueName="taskValue"/>
                </list>
            </ExclusiveTask>
            <ExclusiveTask NextTaskID="d88d4ad6-fb5f-4122-8ede-504481bc09d9" PreTaskID="13fc91df-db76-4b9c-9179-3fde6c062aa0" id="f0d24bb9-336f-4026-96ee-fffcc2146eda" name="endExclusiveTask"/>
            <TestTask1 NextTaskID="f0d24bb9-336f-4026-96ee-fffcc2146eda" PreTaskID="aba55168-f43f-41e1-87ae-576e3f527962" id="9bd60356-23be-4604-89fd-cd82ba0c3bc1" name="task1"/>
            <TestTask2 NextTaskID="f0d24bb9-336f-4026-96ee-fffcc2146eda" PreTaskID="aba55168-f43f-41e1-87ae-576e3f527962" id="79cc50ef-cda5-427c-b635-42987fcaf9ef" name="task2"/>
            <TestTask3 NextTaskID="f0d24bb9-336f-4026-96ee-fffcc2146eda" PreTaskID="aba55168-f43f-41e1-87ae-576e3f527962" id="3adf4316-2679-4311-bd18-2e1d6582c4ef" name="task3"/>
            <TestTask3 PreTaskID="f0d24bb9-336f-4026-96ee-fffcc2146eda" id="d88d4ad6-fb5f-4122-8ede-504481bc09d9" name="task after endExclusiveTask "/>
        </tasks>
    </TaskManager>

</DummyProcess>`

`<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<DummyProcess xmlns="">

    <TaskManager>
        <tasks>
            <TestTask1 NextTaskID="0c8858ec-df51-4fca-8d09-85b2bf27f0d7" id="9b823fca-56e9-4808-b8f8-4be2a10ceacf" name="startTask"/>
            <ParallelTask PreTaskID="9b823fca-56e9-4808-b8f8-4be2a10ceacf" id="0c8858ec-df51-4fca-8d09-85b2bf27f0d7" name="startParallelTask">
                <list name="OutTasks" type="std::string">
                    <std::string value="f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1"/>
                    <std::string value="f31eea0c-9d59-47b5-8c4f-8317de76453e"/>
                    <std::string value="f627851e-5e78-4788-af0b-4f1b5eab9b40"/>
                </list>
            </ParallelTask>
            <ParallelTask NextTaskID="d954d497-643a-4389-9f56-543732f473dc" PreTaskID="0c8858ec-df51-4fca-8d09-85b2bf27f0d7" id="496addbb-8d82-4ce1-b829-4c0e0b2b680f" name="endParallelTask">
                <list name="InTasks" type="std::string">
                    <std::string value="f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1"/>
                    <std::string value="f31eea0c-9d59-47b5-8c4f-8317de76453e"/>
                    <std::string value="f627851e-5e78-4788-af0b-4f1b5eab9b40"/>
                </list>
            </ParallelTask>
            <TestTask1 NextTaskID="496addbb-8d82-4ce1-b829-4c0e0b2b680f" PreTaskID="0c8858ec-df51-4fca-8d09-85b2bf27f0d7" id="f8fa1ff1-3d8d-42ee-a8f9-d00424a98ac1" name="task1"/>
            <TestTask2 NextTaskID="496addbb-8d82-4ce1-b829-4c0e0b2b680f" PreTaskID="0c8858ec-df51-4fca-8d09-85b2bf27f0d7" id="f31eea0c-9d59-47b5-8c4f-8317de76453e" name="task2"/>
            <TestTask3 NextTaskID="496addbb-8d82-4ce1-b829-4c0e0b2b680f" PreTaskID="0c8858ec-df51-4fca-8d09-85b2bf27f0d7" id="f627851e-5e78-4788-af0b-4f1b5eab9b40" name="task3"/>
            <TestTask3 PreTaskID="496addbb-8d82-4ce1-b829-4c0e0b2b680f" id="d954d497-643a-4389-9f56-543732f473dc" name="task after endParallelTask "/>
        </tasks>
    </TaskManager>

</DummyProcess>`


`<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<DummyProcess xmlns="">

    <TaskManager>
        <tasks>
            <TestTask1 NextTaskID="07673c72-f698-4741-b98f-f2ff6ae59f47" id="07673c72-f698-4741-b98f-f2ff6ae59f48" name="startTask"/>
            <SubProcessTask NextTaskID="647da05a-9cfd-40b5-b483-22f2b14e4a9c" PreTaskID="07673c72-f698-4741-b98f-f2ff6ae59f47" id="07673c72-f698-4741-b98f-f2ff6ae59f47" name="subProcessTask">
                <SubTaskManager>
                    <tasks>
                        <TestTask1 NextTaskID="a92c2f45-dd70-4440-85c7-58068ae460bc" id="bc3045c9-2a63-4b82-8977-1e620519ed6b" name="startTask"/>
                        <ParallelTask PreTaskID="bc3045c9-2a63-4b82-8977-1e620519ed6b" id="a92c2f45-dd70-4440-85c7-58068ae460bc" name="startParallelTask">
                            <list name="OutTasks" type="std::string">
                                <std::string value="57eccfb9-472a-4151-a3e1-56b235b11185"/>
                                <std::string value="b325ceed-441d-4fc8-ab95-7015c501aca5"/>
                                <std::string value="46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1"/>
                            </list>
                        </ParallelTask>
                        <ParallelTask NextTaskID="3d12eb05-ec40-4b40-983d-e08bea75e455" PreTaskID="a92c2f45-dd70-4440-85c7-58068ae460bc" id="5aa80f88-244c-4541-9856-2fdaf1c80942" name="endParallelTask">
                            <list name="InTasks" type="std::string">
                                <std::string value="57eccfb9-472a-4151-a3e1-56b235b11185"/>
                                <std::string value="b325ceed-441d-4fc8-ab95-7015c501aca5"/>
                                <std::string value="46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1"/>
                            </list>
                        </ParallelTask>
                        <TestTask1 NextTaskID="5aa80f88-244c-4541-9856-2fdaf1c80942" PreTaskID="a92c2f45-dd70-4440-85c7-58068ae460bc" id="57eccfb9-472a-4151-a3e1-56b235b11185" name="task1"/>
                        <TestTask2 NextTaskID="5aa80f88-244c-4541-9856-2fdaf1c80942" PreTaskID="a92c2f45-dd70-4440-85c7-58068ae460bc" id="b325ceed-441d-4fc8-ab95-7015c501aca5" name="task2"/>
                        <TestTask3 NextTaskID="5aa80f88-244c-4541-9856-2fdaf1c80942" PreTaskID="a92c2f45-dd70-4440-85c7-58068ae460bc" id="46b2a86a-8b64-4cbe-8c63-1b9d9ad8a3a1" name="task3"/>
                        <TestTask3 PreTaskID="5aa80f88-244c-4541-9856-2fdaf1c80942" id="3d12eb05-ec40-4b40-983d-e08bea75e455" name="task after endParallelTask "/>
                    </tasks>
                </SubTaskManager>
            </SubProcessTask>
            <TestTask1 PreTaskID="07673c72-f698-4741-b98f-f2ff6ae59f47" id="647da05a-9cfd-40b5-b483-22f2b14e4a9c" name="endTask"/>
        </tasks>
    </TaskManager>

</DummyProcess>`