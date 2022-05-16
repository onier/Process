//
// Created by ubuntu on 5/13/22.
//

#include "EditEdgeAction.h"
#include "Edge.h"

bool EditEdgeAction::startAction(QPointF pointF) {
    _edge = dynamic_cast<Edge *>(_sourceShape.get());
    if (_edge->_start.distance({(float) pointF.x(), (float) pointF.y()}) >
        _edge->_end.distance({(float) pointF.x(), (float) pointF.y()})) {
        _isStart = false;
    } else {
        _isStart = true;
    }
    return false;
}

bool EditEdgeAction::doAction(QPointF pointF) {
    auto shape = _processGraphics->getShape<Edge>(pointF);
    if (shape) {
        if (_isStart) {
            _edge->setStartShape(shape);
        } else {
            _edge->setEndShape(shape);
        }
    } else {
        if (_isStart) {
            _edge->setStartShape(nullptr);
            _edge->_start = {(float) pointF.x(), (float) pointF.y()};
        } else {
            _edge->setEndShape(nullptr);
            _edge->_end = {(float) pointF.x(), (float) pointF.y()};
        }
    }
    return false;
}

bool EditEdgeAction::endAction(QPointF pointF) {
    return false;
}

std::string EditEdgeAction::getActionType() {
    return "EditEdgeAction";
}
