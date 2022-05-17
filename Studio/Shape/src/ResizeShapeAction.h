//
// Created by ubuntu on 5/13/22.
//

#ifndef PUPPY_RESIZESHAPEACTION_H
#define PUPPY_RESIZESHAPEACTION_H

#include "Action.h"

struct ControllerPoint {
    int _i;
    QPointF _pointF;

    ControllerPoint(int i, const QPointF &pointF);
};

class ResizeShapeAction : public Action {
public:
    bool startAction(QPointF pointF) override;

    bool doAction(QPointF pointF) override;

    bool endAction(QPointF pointF) override;

    std::string getActionType() override;

    std::vector<ControllerPoint> _controllers;

    QPointF _initPoint;
    Bound _initBound;
    QPointF _p1, _p2;
};


#endif //PUPPY_RESIZESHAPEACTION_H
