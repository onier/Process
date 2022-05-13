//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_PROCESSGRAPHICS_H
#define PUPPY_PROCESSGRAPHICS_H

#include "vector"
#include "memory"
#include "Shape.h"

enum ShapeStatus {

};
typedef std::function<void(std::shared_ptr<Shape>, ShapeStatus)> ShapeEventHandler;

class ProcessGraphics {
public:
    ProcessGraphics();

    std::vector<std::shared_ptr<Shape>> getShapes();

    void addShape(std::shared_ptr<Shape> shape);

    void addHandler(ShapeEventHandler handler);

    void notify(std::shared_ptr<Shape> shape, ShapeStatus status);

    void clearSelection();

    std::shared_ptr<Shape> getShape(QPointF pointF, int type = 0);

    std::vector<std::shared_ptr<Shape>> getSelectShapes();

private:
    std::vector<std::shared_ptr<Shape>> _shapes;
    std::vector<ShapeEventHandler> _handlers;
};


#endif //PUPPY_PROCESSGRAPHICS_H
