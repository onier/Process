//
// Created by xuzhenhai on 2022/5/13.
//

#ifndef PUPPY_ADDEDGEACTION_H
#define PUPPY_ADDEDGEACTION_H

#include "ProcessGraphics.h"
#include "Shape.h"
#include "Datas.h"
#include "Action.h"
#include "Edge.h"

class AddEdgeAction : public Action {
public:
    AddEdgeAction();

    virtual bool startAction(QPointF pointF);

    virtual bool doAction(QPointF pointF);

    virtual bool endAction(QPointF pointF);

    ActionType getActionType() override;

    QPointF _startPoint;
    std::shared_ptr<Edge> _edge;
};


#endif //PUPPY_ADDEDGEACTION_H
