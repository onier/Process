//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_PROCESSEDITOR_H
#define PUPPY_PROCESSEDITOR_H

#include "QWidget"
#include "memory"
#include "Shape.h"
#include "ProcessGraphics.h"
#include "Edge.h"

class ProcessEditor : public QWidget {
Q_OBJECT
public:
    ProcessEditor(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    std::shared_ptr<ProcessGraphics> _graphics;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    bool checkAddEdge(QMouseEvent *event);

    bool isAtAnchor(QPointF pointF);

    bool _isPress;
    QPointF _pressPoint;
    std::shared_ptr<Edge> _currentEdge;
    std::shared_ptr<Shape> _currentSelectShape;
};


#endif //PUPPY_PROCESSEDITOR_H
