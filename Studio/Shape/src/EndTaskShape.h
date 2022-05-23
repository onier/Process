//
// Created by ubuntu on 5/15/22.
//

#ifndef PUPPY_ENDTASKSHAPE_H
#define PUPPY_ENDTASKSHAPE_H

#include "TaskShape.h"

class EndTaskShape : public TaskShape {
public:
    EndTaskShape();

    xercesc::DOMElement *createElement(xercesc::DOMDocument *document) override;

    Bound getBound();

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

    Point _center;
    float _r;
    Bound _bound;
    QPointF _startActionPos;
    Bound _old_bound;
};


#endif //PUPPY_ENDTASKSHAPE_H
