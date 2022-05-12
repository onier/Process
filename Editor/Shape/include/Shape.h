//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_SHAPE_H
#define PUPPY_SHAPE_H

#include "QRectF"
#include "QPainter"
#include "QPointF"
#include "rttr/registration.h"

struct Point {
    float _x;
    float _y;

    Point();

    Point(float x, float y) : _x(x), _y(y) {}

RTTR_ENABLE(Point)
};

struct Bound {
    float _x;
    float _y;
    float _w;
    float _h;

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
    virtual bool getAnchor(QPointF point,QPointF & value) = 0;
    bool _isSelected;
    bool _isShowAncher;
    std::string _id;
    std::string _preID;
    std::string _nextID;
};


#endif //PUPPY_SHAPE_H
