//
// Created by ubuntu on 5/14/22.
//

#ifndef PUPPY_PARALLELGATEWAYSHAPE_H
#define PUPPY_PARALLELGATEWAYSHAPE_H

#include "TaskShape.h"

class ParallelGatewayShape : public TaskShape {
public:
    ParallelGatewayShape();

    xercesc::DOMElement *createElement(xercesc::DOMDocument *document) override;

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

    std::string checkActionAnchor(QPointF point, QPointF &target, double value) override;

    bool isContained(QPointF pointF) override;

    Bound _bound;
};


#endif //PUPPY_PARALLELGATEWAYSHAPE_H
