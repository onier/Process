#include "rttr/registration.h"
#include "Circle.h"
#include "Shape.h"

RTTR_REGISTRATION {
    rttr::registration::class_<Circle>("Circle")
            .property("center", &Circle::_center)
            .property("R", &Circle::_r);
    rttr::registration::class_<Point>("Point")
            .property("X", &Point::_x)
            .property("Y", &Point::_y);
}
