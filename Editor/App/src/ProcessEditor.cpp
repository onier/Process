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
    std::shared_ptr<Circle> circle1 = std::make_shared<Circle>();
    circle1->setBound({10, 10, 100, 100});
    _graphics->addShape(circle1);

    std::shared_ptr<Circle> circle2 = std::make_shared<Circle>();
    circle2->setBound({170, 170, 100, 100});
    _graphics->addShape(circle2);
    std::shared_ptr<Edge> edge = std::make_shared<Edge>();
    edge->_startShape = circle1;
    edge->_endShape = circle2;
    edge->_isShowAncher= false;
    edge->setSelected(false);
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
    {
        if(_currentSelectShape){
            QPointF p;
            if(_currentSelectShape->checkNearAnchor(event->posF(),p)){
                LOG(INFO)<<"xxxxxxxxxxxxxx ";
            }
        }
    }
    _isPress = true;
    _pressPoint = event->posF();
    auto s = _graphics->getShape(event->posF());
    if (s) {
        if (s->isSelected() && !isAtAnchor(event->posF())) {
//            s->setSelected(!s->isSelected());_current
        } else {
            _currentSelectShape = s;
            s->setSelected(true);
        }
    } else {
        if (!checkAddEdge(event))
            _graphics->clearSelection();
    }
    repaint();
    QWidget::mousePressEvent(event);
}

void ProcessEditor::mouseReleaseEvent(QMouseEvent *event) {
    _isPress = false;
    auto s = _graphics->getShape(event->posF());
    if (!s) {
        _currentSelectShape = nullptr;
        _graphics->clearSelection();
        repaint();
    }
    if (_currentEdge) {
        _currentEdge->_isShowAncher = false;
        _currentEdge->setSelected(false);
        if (_currentEdge->_endShape)
            _currentEdge->_endShape->_isShowAncher = false;
        _currentEdge = nullptr;
        repaint();
    }
    QWidget::mouseReleaseEvent(event);
}

bool ProcessEditor::isAtAnchor(QPointF pointF) {
    auto selects = _graphics->getSelectShapes();
    for (auto &s: selects) {
        QPointF target;
        if (s->checkNearAnchor(pointF, target)) {
            return true;
        }
    }
    return false;
}

bool ProcessEditor::checkAddEdge(QMouseEvent *event) {
    auto selects = _graphics->getSelectShapes();
    for (auto &s: selects) {
        QPointF target;
        if (s->checkNearAnchor(event->posF(), target)) {
            _currentEdge = std::make_shared<Edge>();
            _currentEdge->_startShape = s;
            _currentEdge->_start = {(float) target.x(), (float) target.y()};
            _currentEdge->_end = {(float) event->posF().x(), (float) event->posF().y()};
            _graphics->addShape(_currentEdge);
            return true;
        }
    }
    return false;
}

void ProcessEditor::mouseMoveEvent(QMouseEvent *event) {
    if (_isPress) {
        if (_currentEdge) {
            std::shared_ptr<Shape> target = _graphics->getShape(event->posF(), 1);
            if (target && (target != _currentEdge->_startShape)) {
                _currentEdge->_endShape = target;
                target->_isShowAncher = true;
            } else {
                _currentEdge->_endShape = nullptr;
                _currentEdge->_end = {(float) event->posF().x(), (float) event->posF().y()};
            }
        } else {
            if (!checkAddEdge(event)) {
                int x = event->posF().x() - _pressPoint.x();
                int y = event->posF().y() - _pressPoint.y();
                auto selects = _graphics->getSelectShapes();
                for (auto &s: selects) {
                    s->transform(x, y);
                }
                _pressPoint = event->posF();
            }
        }
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
