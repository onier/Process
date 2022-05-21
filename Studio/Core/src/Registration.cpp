#include "ProcessStudio.h"

#include "rttr/registration.h"

RTTR_PLUGIN_REGISTRATION {
    rttr::registration::enumeration<ParameterType>("ParameterType")
            (
                    rttr::value("INT", ParameterType::INT),
                    rttr::value("FLOAT", ParameterType::FLOAT),
                    rttr::value("DOUBLE", ParameterType::DOUBLE),
                    rttr::value("STRING", ParameterType::STRING)
            );

    rttr::registration::class_<Para>("Parameter")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_object
            )
            .property("ParameterName", &Para::_name)
            .property("ParameterValue", &Para::_value)
            .property("ParameterType", &Para::_type)
            .property("ParameterDescription", &Para::_description);

    rttr::registration::class_<ProcesInfo>("ProcesInfo")
            .

                    constructor<>()(
                    rttr::policy::ctor::as_std_shared_ptr
            )
            .property("ThreadCount", &ProcesInfo::_threadCount)
            .property("Name", &ProcesInfo::_name)
            .property("Parameters", &ProcesInfo::_parameters);

//    rttr::registration::class_<ProcesInfo>("ProcesInfoPtr")
//            .
//
//                    constructor<>()(
//                    rttr::policy::ctor::as_std_shared_ptr
//            )
//            .property("ThreadCount", &ProcesInfo::_threadCount)
//            .property("Name", &ProcesInfo::_name)
//            .property("Parameters", &ProcesInfo::_parameters);
}
