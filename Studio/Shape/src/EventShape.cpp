//
// Created by ubuntu on 5/14/22.
//

#include "EventGatewayShape.h"
#include "XML.h"

EventGatewayShape::EventGatewayShape() {
    _bColor = {255, 255, 255};
    _fColor = {0, 0, 0};
    _isSelected = false;
    _isShowAncher = false;
    _text = "";
}

Bound EventGatewayShape::getBound() {
    return _bound;
}

void EventGatewayShape::paint(QPainter *painter) {
    auto pb = painter->brush();
    auto pp = painter->pen();
    paintShadow(painter);
    QPen pen;
    pen.setColor({_fColor._r, _fColor._g, _fColor._b});
    painter->setPen(pen);
    QBrush brush({{_bColor._r, _bColor._g, _bColor._b}});
    painter->setBrush(brush);
    QPolygonF polygonF;
    auto ps = getAnchorPoints();
    for (auto &p: ps) {
        polygonF.push_back(p);
    }
    painter->drawPolygon(polygonF);
    auto center = _bound.center();
    auto n = _bound._w / 8;
    polygonF.clear();
    pen.setWidth(6);
    painter->setPen(pen);
    painter->drawEllipse(center.x() - 2 * n, center.y() - 2 * n, 4 * n, 4 * n);
    paintAnchorPoints(painter);
    paintControllPoints(painter);
    int x = _bound._x;
    int y = _bound._y;
    int w = _bound._w;
    int h = _bound._h;
    QFontMetrics fm(painter->font());
    int fw = fm.width(_text.data());
    painter->drawText(x + w / 2 - fw / 2, y + h / 2 + fm.height() / 4, _text.data());
    painter->setPen(pp);
    painter->setBrush(pb);
}

void EventGatewayShape::paintShadow(QPainter *painter) {
    auto pb = painter->brush();
    auto pp = painter->pen();
    if (_isSelected) {
        QPen pen(Qt::DashDotDotLine);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawRect(_bound._x, _bound._y, _bound._w, _bound._h);
    }
    painter->setPen(pp);
    painter->setBrush(pb);
}

void EventGatewayShape::paintAnchorPoints(QPainter *painter) {
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

void EventGatewayShape::paintControllPoints(QPainter *painter) {
    if (_isSelected) {
        auto anchors = getControlPoints();
        auto pb = painter->brush();
        auto pp = painter->pen();
        for (auto &archor: anchors) {
            QPen pen;
            pen.setColor({_fColor._r, _fColor._g, _fColor._b});
            painter->setPen(pen);
            QBrush brush({{0, 255, 255}});
            painter->setBrush(brush);
            painter->drawEllipse(archor.x() - 5, archor.y() - 5, 10, 10);
        }
        painter->setPen(pp);
        painter->setBrush(pb);
    }
}

std::vector<QPointF> EventGatewayShape::getAnchorPoints() {
    return {{_bound._x + _bound._w / 2, _bound._y},
            {_bound._x + _bound._w,     _bound._y + _bound._h / 2},
            {_bound._x + _bound._w / 2, _bound._y + _bound._h},
            {_bound._x,                 _bound._y + _bound._h / 2}};
}

std::vector<QPointF> EventGatewayShape::getControlPoints() {
    return {{_bound._x,             _bound._y},
            {_bound._x + _bound._w, _bound._y},
            {_bound._x + _bound._w, _bound._y + _bound._h},
            {_bound._x,             _bound._y + _bound._h}};
}

void EventGatewayShape::paintAxis(QPainter *painter) {

}

void EventGatewayShape::setBound(Bound rectF) {
    _bound = rectF;
}

void EventGatewayShape::transform(float x, float y) {

}

bool EventGatewayShape::getNearestAnchor(QPointF point, QPointF &value) {
    auto as = getAnchorPoints();
    std::sort(as.begin(), as.end(), [point](QPointF &a1, QPointF &a2) {
        return std::pow(std::pow((a1.x() - point.x()), 2) + std::pow((a1.y() - point.y()), 2), 0.5)
               < std::pow(std::pow((a2.x() - point.x()), 2) + std::pow((a2.y() - point.y()), 2), 0.5);
    });
    if (as.empty())
        return false;
    else
        value = as[0];
    return true;
}

std::string EventGatewayShape::checkActionAnchor(QPointF point, QPointF &target, double value) {
    auto as = getAnchorPoints();
    for (auto &a: as) {
        if (std::pow(std::pow((a.x() - point.x()), 2) + std::pow((a.y() - point.y()), 2), 0.5) < value) {
            target = a;
            return "AddEdgeAction";
        }
    }

    as = getControlPoints();
    for (auto &a: as) {
        if (std::pow(std::pow((a.x() - point.x()), 2) + std::pow((a.y() - point.y()), 2), 0.5) < value) {
            target = a;
            return "ResizeShapeAction";
        }
    }
    return "INVALID";
}

bool EventGatewayShape::isContained(QPointF pointF) {
    if (QRectF(_bound._x, _bound._y, _bound._w, _bound._h).contains(pointF)) {
        return true;
    }
    return false;
}

#include "boost/lexical_cast.hpp"

xercesc::DOMElement *EventGatewayShape::createElement(xercesc::DOMDocument *document) {
    auto eventGatewayShape = document->createElement(XStr("EventGatewayShape"));
    auto bound = document->createElement(XStr("Bound"));
    bound->setAttribute(XStr("X"), XStr(boost::lexical_cast<std::string>(_bound._x).data()));
    bound->setAttribute(XStr("Y"), XStr(boost::lexical_cast<std::string>(_bound._y).data()));
    bound->setAttribute(XStr("W"), XStr(boost::lexical_cast<std::string>(_bound._w).data()));
    bound->setAttribute(XStr("H"), XStr(boost::lexical_cast<std::string>(_bound._h).data()));
    eventGatewayShape->appendChild(bound);
    return eventGatewayShape;
}
