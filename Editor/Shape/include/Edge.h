//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_EDGE_H
#define PUPPY_EDGE_H

#include "Shape.h"

class Edge : public Shape {
public:
    Edge();

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

    bool checkNearAnchor(QPointF point, QPointF &target, double value) override;

    bool isContained(QPointF pointF) override;

    Point _start, _end;
    std::shared_ptr<Shape> _startShape, _endShape;

private:
    void recompute();
};


#endif //PUPPY_EDGE_H