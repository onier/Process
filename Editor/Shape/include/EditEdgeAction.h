//
// Created by ubuntu on 5/13/22.
//

#ifndef PUPPY_EDITEDGEACTION_H
#define PUPPY_EDITEDGEACTION_H

#include "Action.h"
#include "Edge.h"

class EditEdgeAction : public Action {
public:
    bool startAction(QPointF pointF) override;

    bool doAction(QPointF pointF) override;

    bool endAction(QPointF pointF) override;

    ActionType getActionType() override;

    bool _isStart;
    Edge *_edge;
};


#endif //PUPPY_EDITEDGEACTION_H
