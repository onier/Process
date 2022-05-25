//
// Created by xuzhenhai on 2022/5/12.
//

#include "EndTaskShape.h"
#include "QPen"
#include "QPainter"
#include "cmath"
#include "XML.h"

EndTaskShape::EndTaskShape() {
    _bColor = {255, 255, 255};
    _fColor = {0, 0, 0};
    _isSelected = false;
    _isShowAncher = false;
    _text = "End";
}

Bound EndTaskShape::getBound() {
    return _bound;
}

void EndTaskShape::paint(QPainter *painter) {
    auto pb = painter->brush();
    auto pp = painter->pen();
    paintShadow(painter);
    QPen pen;
    pen.setWidth(4);
    pen.setColor({_fColor._r, _fColor._g, _fColor._b});
    painter->setPen(pen);
    QBrush brush({{_bColor._r, _bColor._g, _bColor._b}});
    painter->setBrush(brush);
    painter->drawEllipse(_bound._x + 3, _bound._y + 3, _bound._w - 6, _bound._h - 6);
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

void EndTaskShape::paintShadow(QPainter *painter) {
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

std::vector<QPointF> EndTaskShape::getControlPoints() {
    return {{_bound._x,             _bound._y},
            {_bound._x + _bound._w, _bound._y},
            {_bound._x + _bound._w, _bound._y + _bound._h},
            {_bound._x,             _bound._y + _bound._h}};
}

void EndTaskShape::paintAnchorPoints(QPainter *painter) {
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

std::vector<QPointF> EndTaskShape::getAnchorPoints() {
    return {{_bound._x + _bound._w / 2, _bound._y},
            {_bound._x + _bound._w,     _bound._y + _bound._h / 2},
            {_bound._x + _bound._w / 2, _bound._y + _bound._h},
            {_bound._x,                 _bound._y + _bound._h / 2}};
}

void EndTaskShape::paintAxis(QPainter *painter) {

}

void EndTaskShape::setBound(Bound rectF) {
    _bound = rectF;
}

void EndTaskShape::transform(float x, float y) {
    _bound._x += x;
    _bound._y += y;
}

void EndTaskShape::paintControllPoints(QPainter *painter) {
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

bool EndTaskShape::getNearestAnchor(QPointF point, QPointF &value) {
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

std::string EndTaskShape::checkActionAnchor(QPointF point, QPointF &target, double value) {
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

bool EndTaskShape::isContained(QPointF pointF) {
    if (QRectF(_bound._x, _bound._y, _bound._w, _bound._h).contains(pointF)) {
        return true;
    }
    return false;
}

xercesc::DOMElement *EndTaskShape::createElement(xercesc::DOMDocument *document) {
    xercesc_3_2::DOMElement *endShapeElement = document->createElement(XStr("EndTaskShape"));
    endShapeElement->setAttribute(XStr("ID"), XStr(_id.data()));
    auto bound = document->createElement(XStr("Bound"));
    endShapeElement->appendChild(bound);
    puppy::common::XML::createElement(_bound, bound, document);
    return endShapeElement;
}

void EndTaskShape::loadDomElement(xercesc::DOMNode *element) {
    std::vector<xercesc::DOMNode *> bound;
    puppy::common::XML::getTagsByName("Bound", element, bound);
    if (bound.size() == 0) {
        puppy::common::XML::parseInstance(bound[0], _bound);
    }
    _id = puppy::common::XML::attributeValue(element->getAttributes(), "ID");
}
