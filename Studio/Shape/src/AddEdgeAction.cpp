//
// Created by xuzhenhai on 2022/5/13.
//

#include <TaskShape.h>
#include "AddEdgeAction.h"
#include "glog/logging.h"

AddEdgeAction::AddEdgeAction() {
}

bool AddEdgeAction::startAction(QPointF pointF) {
    _startPoint = pointF;
    if (_sourceShape) {
        return true;
    } else {
        return false;
    }
}

bool AddEdgeAction::doAction(QPointF pointF) {
    if (!_edge) {
        _edge = std::make_shared<Edge>();
        _edge->_start = {(float) _startPoint.x(), (float) _startPoint.y()};
        _edge->setStartShape(_sourceShape);
        _edge->_end = {(float) pointF.x(), (float) pointF.y()};
        _edge->setSelected(true);
        _edge->_isShowAncher = true;
        _processGraphics->addShape(_edge);
    } else {
        const std::shared_ptr<Shape> &target = _processGraphics->getShape<TaskShape>(pointF);
        if (target && target!=_edge->getStartShape()) {
            _edge->setEndShape(target);
        } else {
            _edge->_end = {(float) pointF.x(), (float) pointF.y()};
        }

    }
    return false;
}

bool AddEdgeAction::endAction(QPointF pointF) {
    _edge->_isShowAncher = false;
    _edge = nullptr;
    return false;
}

std::string AddEdgeAction::getActionType() {
    return "AddEdgeAction";
}