#include "rttr/registration.h"
#include "TestTask1.h"
#include "TestTask2.h"
#include "TestTask3.h"
#include "ExceptionTask.h"
#include "FakeTask.h"

using namespace Process;

RTTR_REGISTRATION {
    rttr::registration::class_<TestTask1>("TestTask1")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            );
    rttr::registration::class_<TestTask2>("TestTask2")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            );
    rttr::registration::class_<TestTask3>("TestTask3")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            );
    rttr::registration::class_<ExceptionTask>("ExceptionTask")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("ExceptionCounter", &ExceptionTask::_runTimeExceptionCounter);
    rttr::registration::class_<FakeTask>("FakeTask")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("Text", &FakeTask::_text)
            .property("Double", &FakeTask::_d)
            .property("Int", &FakeTask::_n)
            .property("EventName", &FakeTask::_eventName)
            .property("Values", &FakeTask::_values)
            .property("Long", &FakeTask::_l);
}
