//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_STARTTASKSHAPE_H
#define PUPPY_STARTTASKSHAPE_H

#include "TaskShape.h"
#include "rttr/registration.h"

class StartTaskShape : public TaskShape {
public:
    StartTaskShape();

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

    std::string checkActionAnchor(QPointF point, QPointF &target, double value) override;

    bool isContained(QPointF pointF) override;

    Point _center;
    float _r;
    Bound _bound;
    QPointF _startActionPos;
    Bound _old_bound;
RTTR_ENABLE()
};


#endif //PUPPY_STARTTASKSHAPE_H
