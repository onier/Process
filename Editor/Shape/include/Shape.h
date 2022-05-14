//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_SHAPE_H
#define PUPPY_SHAPE_H

#include "QRectF"
#include "QPainter"
#include "QPointF"
#include "rttr/registration.h"
#include "cmath"
#include "Datas.h"

struct Point {
    float _x;
    float _y;

    float distance(Point point) {
        return std::pow(std::pow(_x - point._x, 2) + std::pow(_y - point._y, 2), 0.5);
    }

    Point(QPointF pointF) : _x(pointF.x()), _y(pointF.y()) {

    }

    Point();

    Point(float x, float y) : _x(x), _y(y) {}

RTTR_ENABLE(Point)
};

struct Bound {
    float _x;
    float _y;
    float _w;
    float _h;

    Bound(QPointF p1, QPointF p2) {
        _x = std::min(p1.x(), p2.x());
        _y = std::min(p1.y(), p2.y());
        _w = std::abs(p1.x() - p2.x());
        _h = std::abs(p1.y() - p2.y());
    }

    QPointF center() {
        return {_x + _w / 2, _y + _h / 2};
    }

    Bound();

    Bound(float x, float y, float w, float h) : _x(x), _y(y), _w(w), _h(h) {}

RTTR_ENABLE(Bound)
};

struct Color {
    int _r;
    int _g;
    int _b;

    Color();

    Color(int r, int g, int b) : _r(r), _g(g), _b(b) {}

RTTR_ENABLE(Color)
};

struct Shape {
    virtual ~Shape() {};

    virtual Bound getBound() = 0;

    virtual void paint(QPainter *painter) = 0;

    virtual void paintShadow(QPainter *painter) = 0;

    virtual void paintAnchorPoints(QPainter *painter) = 0;

    virtual void paintControllPoints(QPainter *painter) = 0;

    virtual std::vector<QPointF> getAnchorPoints() = 0;

    virtual std::vector<QPointF> getControlPoints() = 0;

    virtual void paintAxis(QPainter *painter) = 0;

    virtual void setBound(Bound rectF) = 0;

    virtual void transform(float x, float y) = 0;

    virtual bool getNearestAnchor(QPointF point, QPointF &value) = 0;

    virtual ActionType checkActionAnchor(QPointF point, QPointF &target, double value = 5) = 0;

    virtual bool isContained(QPointF pointF) = 0;

    virtual bool isSelected() {
        return _isSelected;
    }

    virtual void setSelected(bool selected) {
        _isSelected = selected;
    }

    bool _isShowAncher;
    std::string _id;
    std::string _preID;
    std::string _nextID;
    std::string _text;
    Color _bColor;
    Color _fColor;
    ShapeType _type;
protected:
    bool _isSelected;

};


#endif //PUPPY_SHAPE_H
