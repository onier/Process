//
// Created by xuzhenhai on 2022/5/12.
//

#include "ProcessEditor.h"
#include "Circle.h"
#include "QMouseEvent"
#include "glog/logging.h"
#include "QMimeData"
#include "Edge.h"
#include "AddEdgeAction.h"

ProcessEditor::ProcessEditor(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    _actions.insert({ADD_EDGE, std::make_shared<AddEdgeAction>()});
    _graphics = std::make_shared<ProcessGraphics>();
    std::shared_ptr<Circle> circle1 = std::make_shared<Circle>();
    circle1->setBound({10, 10, 100, 100});
    _graphics->addShape(circle1);

    std::shared_ptr<Circle> circle2 = std::make_shared<Circle>();
    circle2->setBound({170, 170, 100, 100});
    _graphics->addShape(circle2);
//    std::shared_ptr<Edge> edge = std::make_shared<Edge>();
//    edge->_startShape = circle1;
//    edge->_endShape = circle2;
//    edge->_isShowAncher = false;
//    edge->setSelected(false);
//    _graphics->addShape(edge);
    setAcceptDrops(true);
    _isEnableMove = false;
    _isEnableAction = false;
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
    _mousePoint = event->posF();
    _pressPoint = event->posF();
    _status = 1;
    {
        if (_currentSelectShape) {
            QPointF p;
            auto type = _currentSelectShape->checkActionAnchor(event->posF(), p);
            if (type == INVALID) {
                _isEnableAction = false;
            } else {
                _actions[type]->_processGraphics = _graphics;
                _actions[type]->_sourceShape=_currentSelectShape;
                _currentAction = _actions[type];
                _currentAction->startAction(event->posF());
                _isEnableAction = true;
                _isEnableMove = false;
            }
        }
    }
    QWidget::mousePressEvent(event);
}

void ProcessEditor::mouseReleaseEvent(QMouseEvent *event) {
    _status = 3;
    if (_isEnableMove) {
        _isEnableMove = false;
        return;
    } else if (_isEnableAction) {
        _isEnableAction = false;
        if (_currentAction) {
            _currentAction->endAction(event->posF());
            _currentAction = nullptr;
            repaint();
        }
        _currentSelectShape = 0;
    } else {
        auto s = _graphics->getShape(event->posF());
        if (!s) {
            _currentSelectShape = nullptr;
            _graphics->clearSelection();
        } else {
            if (s->isSelected()) {
                s->_isShowAncher = false;
            }
            s->setSelected(!s->isSelected());
            if (s->isSelected()) {
                _currentSelectShape = s;
            }
        }
        repaint();
    }
    QWidget::mouseReleaseEvent(event);
}

void ProcessEditor::mouseMoveEvent(QMouseEvent *event) {
    {
        if (_status == 1) {
            if (_currentSelectShape) {
                QPointF p;
                auto d = Point(_pressPoint).distance(Point(event->posF()));
//                if ( d> 6) {
                    auto type = _currentSelectShape->checkActionAnchor(event->posF(), p);
                    if (type == INVALID) {
                        _isEnableAction = false;
                        _isEnableMove = true;
                    }
//                }
            }
        }
    }
    _status = 2;
    if (_isEnableAction) {
        if (_currentSelectShape) {
            _currentAction->doAction(event->posF());
            repaint();
        }
    } else if (_isEnableMove) {
        float x = event->posF().x() - _mousePoint.x();
        float y = event->posF().y() - _mousePoint.y();
        auto ss = _graphics->getSelectShapes();
        for (auto &s: ss) {
            Bound b = s->getBound();
            b._x = b._x + x;
            b._y = b._y + y;
            s->setBound(b);
        }
        repaint();
    }
    _mousePoint = event->posF();
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
