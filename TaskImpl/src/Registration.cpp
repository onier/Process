#include "rttr/registration.h"
#include "TestTask1.h"
#include "TestTask2.h"
#include "TestTask3.h"
#include "ExceptionTask.h"

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
            .property("ValueName", &ExceptionTask::_exceptionCounter);
}
