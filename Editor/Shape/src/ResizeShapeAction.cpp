//
// Created by ubuntu on 5/13/22.
//

#include "ResizeShapeAction.h"

bool ResizeShapeAction::startAction(QPointF point) {
    auto controls = _sourceShape->getControlPoints();
    _controllers.clear();
    for (int i = 0; i < controls.size(); i++) {
        _controllers.push_back({i, controls[i]});
    }
    std::sort(_controllers.begin(), _controllers.end(), [point](ControllerPoint &p1, ControllerPoint &p2) {
        auto a1 = p1._pointF;
        auto a2 = p2._pointF;
        return std::pow(std::pow((a1.x() - point.x()), 2) + std::pow((a1.y() - point.y()), 2), 0.5)
               < std::pow(std::pow((a2.x() - point.x()), 2) + std::pow((a2.y() - point.y()), 2), 0.5);
    });
    _initPoint = point;
    _initBound = _sourceShape->getBound();
    QRectF rect(_initBound._x, _initBound._y, _initBound._w, _initBound._h);
    switch (_controllers[0]._i) {
        case 0:
        case 2:
            _p1 = rect.topLeft();
            _p2 = rect.bottomRight();
            break;;
        case 1:
        case 3:
            _p1 = rect.topRight();
            _p2 = rect.bottomLeft();
    }
    return false;
}

bool ResizeShapeAction::doAction(QPointF pointF) {
    switch (_controllers[0]._i) {
        case 0:
        case 1:
            _p1 = pointF;
            break;
        case 2:
        case 3:
            _p2 = pointF;
            break;
    }
    _sourceShape->setBound({_p1, _p2});
    return false;
}

bool ResizeShapeAction::endAction(QPointF pointF) {
    return false;
}

ActionType ResizeShapeAction::getActionType() {
    return EDIT_EDGE;
}

ControllerPoint::ControllerPoint(int i, const QPointF &pointF) : _i(i), _pointF(pointF) {}
