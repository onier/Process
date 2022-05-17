//
// Created by xuzhenhai on 2022/5/13.
//

#ifndef PUPPY_ACTION_H
#define PUPPY_ACTION_H

#include "ProcessGraphics.h"
#include "Datas.h"
#include "QPointF"
#include "Shape.h"

struct Action {
    ~Action() {};

    virtual bool startAction(QPointF pointF) = 0;

    virtual bool doAction(QPointF pointF) = 0;

    virtual bool endAction(QPointF pointF) = 0;

    virtual std::string getActionType() = 0;

    std::shared_ptr<ProcessGraphics> _processGraphics;
    std::shared_ptr<Shape> _sourceShape;
};


#endif //PUPPY_ACTION_H
