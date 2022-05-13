//
// Created by xuzhenhai on 2022/5/12.
//

#include "ProcessGraphics.h"

ProcessGraphics::ProcessGraphics() {

}

std::vector<std::shared_ptr<Shape>> ProcessGraphics::getShapes() {
    return _shapes;
}

void ProcessGraphics::addShape(std::shared_ptr<Shape> shape) {
    _shapes.push_back(shape);
}

void ProcessGraphics::addHandler(ShapeEventHandler handler) {
    _handlers.push_back(handler);
}

void ProcessGraphics::notify(std::shared_ptr<Shape> shape, ShapeStatus status) {
    for (auto &h: _handlers) {
        h(shape, status);
    }
}

std::shared_ptr<Shape> ProcessGraphics::getShape(QPointF pointF, int type) {
    for (auto &s: _shapes) {
        auto b = s->getBound();
//        if (QRectF(b._x, b._y, b._w, b._h).contains(pointF)) {
//            return s;
//        }
        if (s->isContained(pointF) && s->_type != type) {
            return s;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Shape>> ProcessGraphics::getSelectShapes() {
    std::vector<std::shared_ptr<Shape>> temp;
    for (auto &s: _shapes) {
        if (s->isSelected()) {
            temp.push_back(s);
        }
    }
    return temp;
}

void ProcessGraphics::clearSelection() {
    for (auto &s: _shapes) {
        s->setSelected(false);
    }
}
