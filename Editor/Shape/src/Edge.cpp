//
// Created by xuzhenhai on 2022/5/12.
//

#include "Edge.h"
#include <math.h>
static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Bound Edge::getBound() {
    return Bound();
}

void Edge::paint(QPainter *painter) {
    float arrowSize = 9;
    QPointF sourcePoint{_start._x, _start._y};
    QPointF destPoint{_end._x, _end._y};
    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                  cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                  cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
//    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

void Edge::paintShadow(QPainter *painter) {

}

void Edge::paintAnchorPoints(QPainter *painter) {

}

void Edge::paintControllPoints(QPainter *painter) {

}

std::vector<QPointF> Edge::getAnchorPoints() {
    return std::vector<QPointF>();
}

std::vector<QPointF> Edge::getControlPoints() {
    return std::vector<QPointF>();
}

void Edge::paintAxis(QPainter *painter) {

}

void Edge::setBound(Bound rectF) {

}

void Edge::transform(float x, float y) {

}

bool Edge::getAnchor(QPointF point, QPointF &value) {
    return false;
}
