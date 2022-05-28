//
// Created by xuzhenhai on 2022/5/12.
//

#include "Edge.h"
#include <math.h>
#include "glog/logging.h"
#include "XML.h"
#include "boost/lexical_cast.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge() {
    _bColor = {255, 255, 255};
    _fColor = {0, 0, 0};
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    _id = boost::lexical_cast<std::string>(uuid);
}

Bound Edge::getBound() {
    return Bound();
}

void Edge::paint(QPainter *painter) {
    auto pb = painter->brush();
    auto pp = painter->pen();
    recompute();
    paintShadow(painter);
    paintAnchorPoints(painter);
    int x = std::min(_start._x, _end._x);
    int y = std::min(_start._y, _end._y);
    int w = std::abs(_start._x - _end._x);
    int h = std::abs(_start._y - _end._y);
    QFontMetrics fm(painter->font());
    int fw = fm.width(_text.data());
    painter->drawText(x + w / 2 - fw / 2, y + h / 2, _text.data());
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
    painter->setPen(pp);
    painter->setBrush(pb);
}

void Edge::paintShadow(QPainter *painter) {
//    if (isSelected()) {
//    auto pb = painter->brush();
//    auto pp = painter->pen();
//    recompute();
//    float arrowSize = 9;
//    QPointF sourcePoint{_start._x, _start._y};
//    QPointF destPoint{_end._x, _end._y};
//    QLineF line(sourcePoint, destPoint);
//    if (qFuzzyCompare(line.length(), qreal(0.)))
//        return;
//
//    // Draw the line itself
//    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter->drawLine(line);
//
//    // Draw the arrows
//    double angle = ::acos(line.dx() / line.length());
//    if (line.dy() >= 0)
//        angle = TwoPi - angle;
//
//    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
//                                                  cos(angle + Pi / 3) * arrowSize);
//    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
//                                                  cos(angle + Pi - Pi / 3) * arrowSize);
//    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
//                                              cos(angle - Pi / 3) * arrowSize);
//    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
//                                              cos(angle - Pi + Pi / 3) * arrowSize);
//
////    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
//    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
//    painter->setPen(pp);
//    painter->setBrush(pb);
//    }
}

void Edge::paintAnchorPoints(QPainter *painter) {
    if (_isShowAncher || _isSelected) {
        auto anchors = getAnchorPoints();
        auto pb = painter->brush();
        auto pp = painter->pen();
        for (auto &archor: anchors) {
            QPen pen;
            pen.setColor({_fColor._r, _fColor._g, _fColor._b});
            painter->setPen(pen);
            QBrush brush({{0, 255, 0}});
            painter->setBrush(brush);
            painter->drawEllipse(archor.x() - 5, archor.y() - 5, 10, 10);
        }
        painter->setPen(pp);
        painter->setBrush(pb);
    }
}

void Edge::paintControllPoints(QPainter *painter) {

}

std::vector<QPointF> Edge::getAnchorPoints() {
    return {{_start._x, _start._y},
            {_end._x,   _end._y}};
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

bool Edge::getNearestAnchor(QPointF point, QPointF &value) {
    return false;
}

void Edge::recompute() {
    if (_startShape && _endShape) {
        QPointF temp;
        if (_startShape->getNearestAnchor(_endShape->getBound().center(), temp)) {
            _start = {(float) temp.x(), (float) temp.y()};
        }

        if (_endShape->getNearestAnchor(_startShape->getBound().center(), temp)) {
            _end = {(float) temp.x(), (float) temp.y()};
        }
    }
}

std::string Edge::checkActionAnchor(QPointF point, QPointF &target, double value) {
    auto as = getAnchorPoints();
    for (auto &a: as) {
        if (std::pow(std::pow((a.x() - point.x()), 2) + std::pow((a.y() - point.y()), 2), 0.5) < value) {
            target = a;
            return "EditEdgeAction";
        }
    }
    return "INVALID";
}

bool Edge::isContained(QPointF pointF) {
    float lineL = _start.distance(_end);
    float l1 = _start.distance({(float) pointF.x(), (float) pointF.y()});
    float l2 = _end.distance({(float) pointF.x(), (float) pointF.y()});
    if (std::abs(lineL - l1 - l2) < 1) {
        return true;
    }
    return false;
}

void Edge::setStartShape(std::shared_ptr<Shape> shape) {
    if (!shape) {
        notifyPropertyEvents("EdgeStartShapeRemove");
    }
    _startShape = shape;
    if (shape) {
        notifyPropertyEvents("EdgeStartShapeChange");
    }
}

void Edge::setEndShape(std::shared_ptr<Shape> shape) {
    if (!shape) {
        notifyPropertyEvents("EdgeEndShapeRemove");
    }
    _endShape = shape;
    if (shape) {
        notifyPropertyEvents("EdgeEndShapeChange");
    }
}

std::shared_ptr<Shape> Edge::getStartShape() {
    return _startShape;
}

std::shared_ptr<Shape> Edge::getEndShape() {
    return _endShape;
}

xercesc::DOMElement *Edge::createElement(xercesc::DOMDocument *document) {
    xercesc_3_2::DOMElement *edgeElement = document->createElement(XStr("Edge"));
    edgeElement->setAttribute(XStr("ID"), XStr(_id.data()));
    auto startPoint = document->createElement(XStr("StartPoint"));
    edgeElement->appendChild(startPoint);
    startPoint->setAttribute(XStr("X"), XStr(boost::lexical_cast<std::string>(_start._x).data()));
    startPoint->setAttribute(XStr("Y"), XStr(boost::lexical_cast<std::string>(_start._y).data()));

    auto endPoint = document->createElement(XStr("EndPoint"));
    edgeElement->appendChild(endPoint);
    endPoint->setAttribute(XStr("X"), XStr(boost::lexical_cast<std::string>(_end._x).data()));
    endPoint->setAttribute(XStr("Y"), XStr(boost::lexical_cast<std::string>(_end._y).data()));

    if (_startShape) {
        auto startShape = document->createElement(XStr("StartShape"));
        edgeElement->appendChild(startShape);
        startShape->setAttribute(XStr("ID"), XStr(boost::lexical_cast<std::string>(_startShape->_id).data()));
    }
    if (_endShape) {
        auto endShape = document->createElement(XStr("EndShape"));
        edgeElement->appendChild(endShape);
        endShape->setAttribute(XStr("ID"), XStr(boost::lexical_cast<std::string>(_endShape->_id).data()));
    }
    return edgeElement;
}

void Edge::loadDomElement(xercesc::DOMNode *element) {
    {
        std::vector<xercesc::DOMNode *> startPoint;
        puppy::common::XML::getTagsByName("StartPoint", element, startPoint);
        if (startPoint.size() == 1) {
            _start._x = boost::lexical_cast<float>(
                    puppy::common::XML::attributeValue(startPoint[0]->getAttributes(), "X"));
            _start._y = boost::lexical_cast<float>(
                    puppy::common::XML::attributeValue(startPoint[0]->getAttributes(), "Y"));
        }
    }
    {
        std::vector<xercesc::DOMNode *> endPoint;
        puppy::common::XML::getTagsByName("EndPoint", element, endPoint);
        if (endPoint.size() == 1) {
            _end._x = boost::lexical_cast<float>(
                    puppy::common::XML::attributeValue(endPoint[0]->getAttributes(), "X"));
            _end._y = boost::lexical_cast<float>(
                    puppy::common::XML::attributeValue(endPoint[0]->getAttributes(), "Y"));
        }
    }
    {
        std::vector<xercesc::DOMNode *> startShape;
        puppy::common::XML::getTagsByName("StartShape", element, startShape);
        if (startShape.size() == 1) {
            _startShapeID = puppy::common::XML::attributeValue(startShape[0]->getAttributes(), "ID");
        }
    }
    {
        std::vector<xercesc::DOMNode *> endShape;
        puppy::common::XML::getTagsByName("EndShape", element, endShape);
        if (endShape.size() == 1) {
            _endShapeID = puppy::common::XML::attributeValue(endShape[0]->getAttributes(), "ID");
        }
    }
}
