#include <ParallelGateway.h>
#include <SubProcessTask.h>
#include "rttr/registration.h"
#include "AbstractTask.h"
#include "ExclusiveGateway.h"
#include "TaskManager.h"
#include "StartTask.h"
#include "EndTask.h"
#include "EventGateway.h"

using namespace Process;

RTTR_REGISTRATION {
    rttr::registration::class_<AbstractTask>("AbstractTask")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )

            .property("id", &AbstractTask::_id)
            .property("name", &AbstractTask::_name)
            .property("NextTaskID", &AbstractTask::_nextTaskID)
            .property("PreTaskID", &AbstractTask::_preTaskID);

    rttr::registration::enumeration<Operator>("Operator")
            (
                    rttr::value(">", Operator::GREATER),
                    rttr::value("<", Operator::LESS),
                    rttr::value("=", Operator::EQUAL),
                    rttr::value(">=", Operator::GREATER_EUQAL),
                    rttr::value("<=", Operator::LESS_EUQAL)
            );

    rttr::registration::class_<ExclusiveRule>("ExclusiveRule")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_object
            )
            .property("ValueName", &ExclusiveRule::_valueName)
            .property("Operator", &ExclusiveRule::_operator)
            .property("Value", &ExclusiveRule::_value)
            .property("Type", &ExclusiveRule::_type)
            .property("TaskID", &ExclusiveRule::_taskID);

    rttr::registration::class_<ExclusiveGateway>("ExclusiveGateway")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("SubTasks", &ExclusiveGateway::_subTasks)
            ;

    rttr::registration::class_<ParallelGateway>("ParallelGateway")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("InTasks", &ParallelGateway::_inTasks)
            .property("OutTasks", &ParallelGateway::_outTasks);

    rttr::registration::class_<StartTask>("StartTask")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            );

    rttr::registration::class_<EndTask>("EndTask")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            );

    rttr::registration::class_<SubProcessTask>("SubProcessTask")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            );

    rttr::registration::class_<EventGateway>("EventGateway")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("EventRules",&EventGateway::_eventRules);
//            .property("SubTaskProcess", &SubProcessTask::_subTaskProcess);
}
