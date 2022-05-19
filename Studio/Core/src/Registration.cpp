#include "ProcessStudio.h"

#include "rttr/registration.h"

RTTR_REGISTRATION {
    rttr::registration::class_<Parameter>("Parameter")
            .property("ParameterName", &Parameter::_name)
            .property("ParameterValue", &Parameter::_value)
            .property("ParameterType", &Parameter::_type)
            .property("ParameterDescription", &Parameter::_description);

    rttr::registration::class_<ProcesInfo>("ProcesInfo")
            .property("ThreadCount", &ProcesInfo::_threadCount)
            .property("Name", &ProcesInfo::_name)
            .property("Parameters", &ProcesInfo::_parameters);
}
