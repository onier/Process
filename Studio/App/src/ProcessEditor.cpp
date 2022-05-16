//
// Created by xuzhenhai on 2022/5/12.
//

#include "ProcessEditor.h"
#include "Action.h"
#include "QMouseEvent"
#include "glog/logging.h"
#include "QMimeData"
#include "Edge.h"
#include "Library.h"

ProcessEditor::ProcessEditor(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    setAcceptDrops(true);
    _isEnableMove = false;
    _isEnableAction = false;
    initTaskAction();
}

void ProcessEditor::setProcessStudio(std::shared_ptr<ProcessStudio> ps) {
    _processStudio = ps;
}

void ProcessEditor::initTaskAction() {
    auto actions = puppy::common::library::get<Action>("ShapeAction");
    for (auto a: actions) {
        LOG(INFO) << "init action " << a->getActionType();
        _actions.insert({a->getActionType(), a});
    }
}

void ProcessEditor::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (auto shape: _processStudio->getProcessGraphics()->getShapes()) {
        shape->paint(&painter);
    }
    QWidget::paintEvent(event);
}

void ProcessEditor::mousePressEvent(QMouseEvent *event) {
    _mousePoint = event->posF();
    _pressPoint = event->posF();
    _status = 1;
    std::shared_ptr<ProcessGraphics> _graphics = _processStudio->getProcessGraphics();
    {
        std::shared_ptr<Shape> _currentSelectShape = _processStudio->getCurrentSelectShape();
        if (_currentSelectShape) {
            QPointF p;
            auto type = _currentSelectShape->checkActionAnchor(event->posF(), p);
            if (type == "INVALID") {
                _isEnableAction = false;
            } else {
                _actions[type]->_processGraphics = _graphics;
                _actions[type]->_sourceShape = _currentSelectShape;
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
            update();
        }
    } else {
        std::shared_ptr<ProcessGraphics> _graphics = _processStudio->getProcessGraphics();
        auto s = _graphics->getShape<Shape>(event->posF());
        if (!s) {
            _processStudio->setCurrentSelectShape(nullptr);
            _graphics->clearSelection();
        } else {
            if (s->isSelected()) {
                s->_isShowAncher = false;
            }
            s->setSelected(!s->isSelected());
            if (s->isSelected()) {
                _processStudio->setCurrentSelectShape(s);
            }
        }
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void ProcessEditor::mouseMoveEvent(QMouseEvent *event) {
    std::shared_ptr<ProcessGraphics> _graphics = _processStudio->getProcessGraphics();
    std::shared_ptr<Shape> _currentSelectShape = _processStudio->getCurrentSelectShape();
    {
        if (_status == 1) {
            if (_currentSelectShape) {
                QPointF p;
                auto d = Point(_pressPoint).distance(Point(event->posF()));
//                if ( d> 6) {
                auto type = _currentSelectShape->checkActionAnchor(event->posF(), p);
                if (type == "INVALID") {
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
            update();
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
        update();
    }
    _mousePoint = event->posF();
    QWidget::mouseMoveEvent(event);
}

void ProcessEditor::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/shape_icon")) {
        event->acceptProposedAction();
        LOG(INFO) << "enable drag";
    }
    QWidget::dragEnterEvent(event);
}

void ProcessEditor::dropEvent(QDropEvent *event) {
    std::shared_ptr<ProcessGraphics> _graphics = _processStudio->getProcessGraphics();
    std::shared_ptr<Shape> _currentSelectShape = _processStudio->getCurrentSelectShape();
    auto type = event->mimeData()->data("application/shape_icon").toStdString();
    auto taskType = rttr::type::get_by_name(type);
    LOG(INFO) << "add task type " << taskType.get_name() << "    " << taskType.get_properties().size();
    if (taskType.is_valid()) {
        auto taskVar = taskType.create();
        std::shared_ptr<Process::Task> task = taskVar.get_value<std::shared_ptr<Process::Task>>();
        rttr::type shapeType = rttr::type::get_by_name(type + "Shape");
        if (shapeType.is_valid()) {
            auto shapeVar = shapeType.create();
            std::shared_ptr<Shape> shape = shapeVar.get_value<std::shared_ptr<Shape>>();
            shape->setBound({(float) event->posF().x(), (float) event->posF().y(), 100, 100});
            _graphics->addShape(shape);
            _processStudio->addTask(task);
            _processStudio->addTaskShapePair(
                    std::make_shared<TaskShapeItem>(task, shape, taskType, shapeType, taskVar, shapeVar));
        } else {
            shapeType = rttr::type::get_by_name("UserTaskShape");
            auto shapeVar = shapeType.create();
            std::shared_ptr<Shape> shape = shapeVar.get_value<std::shared_ptr<Shape>>();
            shape->setBound({(float) event->posF().x(), (float) event->posF().y(), 100, 100});
            _graphics->addShape(shape);
            _processStudio->addTask(task);
            _processStudio->addTaskShapePair(
                    std::make_shared<TaskShapeItem>(task, shape, taskType, shapeType, taskVar, shapeVar));
        }
        update();
    }
    QWidget::dropEvent(event);
}

void ProcessEditor::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
}

