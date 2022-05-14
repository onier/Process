//
// Created by ubuntu on 5/14/22.
//

#ifndef PUPPY_EXCLUSIVESHAPE_H
#define PUPPY_EXCLUSIVESHAPE_H

#include "Shape.h"

class ExclusiveShape : public Shape {
public:
    ExclusiveShape();

    Bound getBound() override;

    void paint(QPainter *painter) override;

    void paintShadow(QPainter *painter) override;

    void paintAnchorPoints(QPainter *painter) override;

    void paintControllPoints(QPainter *painter) override;

    std::vector<QPointF> getAnchorPoints() override;

    std::vector<QPointF> getControlPoints() override;

    void paintAxis(QPainter *painter) override;

    void setBound(Bound rectF) override;

    void transform(float x, float y) override;

    bool getNearestAnchor(QPointF point, QPointF &value) override;

    ActionType checkActionAnchor(QPointF point, QPointF &target, double value) override;

    bool isContained(QPointF pointF) override;

    Bound _bound;
};


#endif //PUPPY_EXCLUSIVESHAPE_H
