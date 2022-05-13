//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_CIRCLE_H
#define PUPPY_CIRCLE_H

#include "Shape.h"
#include "rttr/registration.h"

class Circle : public Shape {
public:
    Circle();

    Bound getBound() override;

    void paint(QPainter *painter) override;

    void paintShadow(QPainter *painter) override;

    void paintAnchorPoints(QPainter *painter) override;

    std::vector<QPointF> getAnchorPoints() override;

    std::vector<QPointF> getControlPoints() override;

    void paintAxis(QPainter *painter) override;

    void paintControllPoints(QPainter *painter) override;

    void setBound(Bound rectF) override;

    void transform(float x, float y) override;

    bool getNearestAnchor(QPointF point, QPointF &value) override;

    bool checkNearAnchor(QPointF point, QPointF &target, double value) override;

    bool isContained(QPointF pointF) override;

    Point _center;
    float _r;
    Bound _bound;

RTTR_ENABLE(Circle)
};


#endif //PUPPY_CIRCLE_H
