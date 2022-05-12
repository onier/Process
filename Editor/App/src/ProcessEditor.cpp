//
// Created by xuzhenhai on 2022/5/12.
//

#include "ProcessEditor.h"
#include "Circle.h"
#include "QMouseEvent"
#include "glog/logging.h"
#include "QMimeData"
#include "Edge.h"

ProcessEditor::ProcessEditor(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    _graphics = std::make_shared<ProcessGraphics>();
//    std::shared_ptr<Circle> circle = std::make_shared<Circle>();
//    circle->setBound({10, 10, 100, 100});
//    _graphics->addShape(circle);
    std::shared_ptr<Edge> edge = std::make_shared<Edge>();
    edge->_start = {10, 10};
    edge->_end = {100, 100};
    _graphics->addShape(edge);
    setAcceptDrops(true);
}

void ProcessEditor::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (auto shape: _graphics->getShapes()) {
        shape->paint(&painter);
    }
    QWidget::paintEvent(event);
}

void ProcessEditor::mousePressEvent(QMouseEvent *event) {
    _isPress = true;
    _pressPoint = event->posF();
    auto s = _graphics->getShape(event->posF());
    if (s) {
        if (s->_isSelected) {

        } else {
            s->_isSelected = true;
            repaint();
        }
    } else {
        _graphics->clearSelection();
        repaint();
    }
    QWidget::mousePressEvent(event);
}

void ProcessEditor::mouseReleaseEvent(QMouseEvent *event) {
    _isPress = false;
    QWidget::mouseReleaseEvent(event);
}

void ProcessEditor::mouseMoveEvent(QMouseEvent *event) {
    if (_isPress) {
        int x = event->posF().x() - _pressPoint.x();
        int y = event->posF().y() - _pressPoint.y();
        auto selects = _graphics->getSelectShapes();
        LOG(INFO) << "  x " << x << "   y " << y;
        for (auto &s: selects) {
            s->transform(x, y);
        }
        _pressPoint = event->posF();
        repaint();
    }
    QWidget::mouseMoveEvent(event);
}

void ProcessEditor::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
    QWidget::dragEnterEvent(event);
}

void ProcessEditor::dropEvent(QDropEvent *event) {
    LOG(INFO) << event->mimeData()->text().toStdString();
    std::shared_ptr<Circle> circle = std::make_shared<Circle>();
    circle->setBound({(float) event->posF().x(), (float) event->posF().y(), 100, 100});
    _graphics->addShape(circle);
    repaint();
    QWidget::dropEvent(event);
}
