//
// Created by ubuntu on 5/15/22.
//

#include "TaskShape.h"

Bound TaskShape::getBound() {
    return Bound();
}

void TaskShape::paint(QPainter *painter) {

}

void TaskShape::paintShadow(QPainter *painter) {

}

void TaskShape::paintAnchorPoints(QPainter *painter) {

}

void TaskShape::paintControllPoints(QPainter *painter) {

}

std::vector<QPointF> TaskShape::getAnchorPoints() {
    return std::vector<QPointF>();
}

std::vector<QPointF> TaskShape::getControlPoints() {
    return std::vector<QPointF>();
}

void TaskShape::paintAxis(QPainter *painter) {

}

void TaskShape::setBound(Bound rectF) {

}

void TaskShape::transform(float x, float y) {

}

bool TaskShape::getNearestAnchor(QPointF point, QPointF &value) {
    return false;
}

std::string TaskShape::checkActionAnchor(QPointF point, QPointF &target, double value) {
   return "";
}

bool TaskShape::isContained(QPointF pointF) {
    return false;
}
