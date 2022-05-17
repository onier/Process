#include "ProcessStudio.h"

#include "rttr/registration.h"

RTTR_REGISTRATION {
    rttr::registration::class_<ProcesInfo>("ProcesInfo")
            .property("ThreadCount", &ProcesInfo::_threadCount)
            .property("Name", &ProcesInfo::_name);
}
