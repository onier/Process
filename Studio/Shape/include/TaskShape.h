//
// Created by ubuntu on 5/15/22.
//

#ifndef PUPPY_TASKSHAPE_H
#define PUPPY_TASKSHAPE_H

#include "Shape.h"

struct TaskShape : public Shape {
public:
    virtual ~TaskShape() {};

    virtual Bound getBound();

    virtual void paint(QPainter *painter);

    virtual void paintShadow(QPainter *painter);

    virtual void paintAnchorPoints(QPainter *painter);

    virtual void paintControllPoints(QPainter *painter);

    virtual std::vector<QPointF> getAnchorPoints();

    virtual std::vector<QPointF> getControlPoints();

    virtual void paintAxis(QPainter *painter);

    virtual void setBound(Bound rectF);

    virtual void transform(float x, float y);

    bool getNearestAnchor(QPointF point, QPointF &value);

    std::string checkActionAnchor(QPointF point, QPointF &target, double value);

    bool isContained(QPointF pointF);
};


#endif //PUPPY_TASKSHAPE_H
