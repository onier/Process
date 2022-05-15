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

void ProcessGraphics::addEdge(std::shared_ptr<Edge> edge) {
    _edges.push_back(edge);
}

void ProcessGraphics::addHandler(ShapeEventHandler handler) {
    _handlers.push_back(handler);
}

void ProcessGraphics::notify(std::shared_ptr<Shape> shape, ShapeStatus status) {
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
