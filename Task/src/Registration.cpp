#include <ParallelGateway.h>
#include <SubProcessTask.h>
#include "rttr/registration.h"
#include "AbstractTask.h"
#include "ExclusiveGateway.h"
#include "StartTask.h"
#include "EndTask.h"
#include "EventGateway.h"
#include "WaitForTask.h"
#include "SubProcessTask.h"

using namespace Process;

RTTR_PLUGIN_REGISTRATION {
    rttr::registration::class_<AbstractTask>("AbstractTask")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )

            .property("id", &AbstractTask::_id)
            .property("name", &AbstractTask::_name)
            .property("NextTaskID", &AbstractTask::_nextTaskID)
            .property("PreTaskID", &AbstractTask::_preTaskID);

    rttr::registration::enumeration<ExclusiveRuleOperator>("Operator")
            (
                    rttr::value(">", ExclusiveRuleOperator::GREATER),
                    rttr::value("<", ExclusiveRuleOperator::LESS),
                    rttr::value("=", ExclusiveRuleOperator::EQUAL),
                    rttr::value(">=", ExclusiveRuleOperator::GREATER_EUQAL),
                    rttr::value("<=", ExclusiveRuleOperator::LESS_EUQAL)
            );

    rttr::registration::enumeration<ExclusiveRuleType>("ExclusiveRuleType")
            (
                    rttr::value("Double", ExclusiveRuleType::DOUBLE),
                    rttr::value("String", ExclusiveRuleType::STRING)
            );

    rttr::registration::class_<ExclusiveRulePtr>("ExclusiveRulePtr")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("ValueName", &ExclusiveRulePtr::_valueName)
            .property("Text", &ExclusiveRulePtr::_text)
            .property("Operator", &ExclusiveRulePtr::_operator)
            .property("Value", &ExclusiveRulePtr::_value)
            .property("Type", &ExclusiveRulePtr::_type)
            .property("TaskID", &ExclusiveRulePtr::_taskID);

    rttr::registration::class_<ExclusiveRule>("ExclusiveRule")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_object
            )
            .property("ValueName", &ExclusiveRule::_valueName)
            .property("Text", &ExclusiveRule::_text)
            .property("Operator", &ExclusiveRule::_operator)
            .property("Value", &ExclusiveRule::_value)
            .property("Type", &ExclusiveRule::_type)
            .property("TaskID", &ExclusiveRule::_taskID);

    rttr::registration::class_<ExclusiveGateway>("ExclusiveGateway")
            (
                    rttr::metadata("key", "Task")
            )
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("SubTasks", &ExclusiveGateway::_subTasks);

    rttr::registration::class_<ParallelGateway>("ParallelGateway")
            (
                    rttr::metadata("key", "Task")
            )
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("InTasks", &ParallelGateway::_inTasks)
            .property("OutTasks", &ParallelGateway::_outTasks);

    rttr::registration::class_<StartTask>("StartTask")
            (
                    rttr::metadata("key", "Task")
            )
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            );

    rttr::registration::class_<EndTask>("EndTask")
            (
                    rttr::metadata("key", "Task")
            )
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            );

    rttr::registration::class_<SubProcessTask>("SubProcessTask")
            (
                    rttr::metadata("key", "Task")
            )
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("ProcessFile", &SubProcessTask::_processFile);

    rttr::registration::class_<WaitForTask>("WaitForTask")
            (
                    rttr::metadata("key", "Task")
            )
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("ValueName", &WaitForTask::_valueName)
            .property("Value", &WaitForTask::_value);

    rttr::registration::class_<EventGateway>("EventGateway")
            (
                    rttr::metadata("key", "Task")
            )
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("EventRules", &EventGateway::_eventRules);
//            .property("SubTaskProcess", &SubProcessTask::_subTaskProcess);
}
