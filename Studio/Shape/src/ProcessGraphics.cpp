//
// Created by xuzhenhai on 2022/5/12.
//

#include "ProcessGraphics.h"
#include "glog/logging.h"

ProcessGraphics::ProcessGraphics() {
}

std::vector<std::shared_ptr<Shape>> ProcessGraphics::getShapes() {
    return _shapes;
}

void ProcessGraphics::addShape(std::shared_ptr<Shape> shape) {
    _shapes.push_back(shape);
    shape->addPropertyEventHanlder([&, shape](std::string msg) {
        notify(shape, msg);
    });
}

void ProcessGraphics::removeShape(std::shared_ptr<Shape> shape) {
    auto iter = std::remove_if(_shapes.begin(), _shapes.end(), [shape](std::shared_ptr<Shape> s) {
        return s == shape;
    });
    if (iter != _shapes.end())
        _shapes.erase(iter);
}

void ProcessGraphics::addHandler(ShapeEventHandler handler) {
    _handlers.push_back(handler);
}

void ProcessGraphics::notify(std::shared_ptr<Shape> shape, std::string status) {
    for (auto &h: _handlers) {
        h(shape, status);
    }
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

void ProcessGraphics::selectAll() {
    for (auto &s: _shapes) {
        s->setSelected(true);
    }
}
