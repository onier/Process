#include "AddEdgeAction.h"
#include "EditEdgeAction.h"
#include "ResizeShapeAction.h"
#include "rttr/registration.h"
#include "StartTaskShape.h"
#include "Shape.h"
#include "EndTaskShape.h"
#include "Edge.h"
#include "ExclusiveGatewayShape.h"
#include "ParallelGatewayShape.h"
#include "SubProcessTaskShape.h"
#include "UserTaskShape.h"

RTTR_REGISTRATION {
    rttr::registration::class_<Point>("Point")
            .property("X", &Point::_x)
            .property("Y", &Point::_y);

    rttr::registration::class_<StartTaskShape>("StartTaskShape")
            (
                    rttr::metadata("key", "Shape")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    )
            .property("center", &StartTaskShape::_center)
            .property("R", &StartTaskShape::_r);

    rttr::registration::class_<EndTaskShape>("EndTaskShape")
            (
                    rttr::metadata("key", "Shape")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    )
            .property("center", &StartTaskShape::_center)
            .property("R", &StartTaskShape::_r);

    rttr::registration::class_<Edge>("Edge")
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    )
            .property("Start", &Edge::_start)
            .property("End", &Edge::_end);

    rttr::registration::class_<ExclusiveGatewayShape>("ExclusiveGatewayShape")
            (
                    rttr::metadata("key", "Shape")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    )
            .property("Bound", &ExclusiveGatewayShape::_bound);

    rttr::registration::class_<ParallelGatewayShape>("ParallelGatewayShape")
            (
                    rttr::metadata("key", "Shape")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    )
            .property("Bound", &ParallelGatewayShape::_bound);

    rttr::registration::class_<SubProcessTaskShape>("SubProcessTaskShape")
            (
                    rttr::metadata("key", "Shape")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    )
            .property("Bound", &SubProcessTaskShape::_bound);

    rttr::registration::class_<UserTaskShape>("UserTaskShape")
            (
                    rttr::metadata("key", "Shape")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    )
            .property("Bound", &UserTaskShape::_bound);

    rttr::registration::class_<UserTaskShape>("UserTaskShape")
            (
                    rttr::metadata("key", "Shape")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    )
            .property("Bound", &UserTaskShape::_bound);

    rttr::registration::class_<AddEdgeAction>("AddEdgeAction")
            (
                    rttr::metadata("key", "ShapeAction")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    );

    rttr::registration::class_<EditEdgeAction>("EditEdgeAction")
            (
                    rttr::metadata("key", "ShapeAction")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    );

    rttr::registration::class_<ResizeShapeAction>("ResizeShapeAction")
            (
                    rttr::metadata("key", "ShapeAction")
            )
            .constructor<>()
                    (
                            rttr::policy::ctor::as_std_shared_ptr
                    );

}
