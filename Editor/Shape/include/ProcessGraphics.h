//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_PROCESSGRAPHICS_H
#define PUPPY_PROCESSGRAPHICS_H

#include "vector"
#include "memory"
#include "Shape.h"
#include "Edge.h"

enum ShapeStatus {

};
typedef std::function<void(std::shared_ptr<Shape>, ShapeStatus)> ShapeEventHandler;

class ProcessGraphics {
public:
    ProcessGraphics();

    std::vector<std::shared_ptr<Shape>> getShapes();

    void addShape(std::shared_ptr<Shape> shape);

    void addEdge(std::shared_ptr<Edge> edge);

    void addHandler(ShapeEventHandler handler);

    void notify(std::shared_ptr<Shape> shape, ShapeStatus status);

    void clearSelection();

    template<class T>
    std::shared_ptr<T> getShape(QPointF pointF) {
        for (auto &s: _shapes) {
            std::shared_ptr<T> tshared = std::dynamic_pointer_cast<T>(s);
            if (tshared) {
                if (tshared->isContained(pointF)) {
                    return tshared;
                }
            }
        }
        return nullptr;
    };

    std::vector<std::shared_ptr<Shape>> getSelectShapes();

private:
    std::vector<std::shared_ptr<Shape>> _shapes;
    std::vector<std::shared_ptr<Edge>> _edges;
    std::vector<ShapeEventHandler> _handlers;
};


#endif //PUPPY_PROCESSGRAPHICS_H
