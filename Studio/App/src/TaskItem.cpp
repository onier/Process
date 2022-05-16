//
// Created by xuzhenhai on 2022/5/10.
//

#include <valarray>
#include <QPainter>
#include "TaskItem.h"
#include "glog/logging.h"
#include "QStaticText"

TaskItem::TaskItem() {
    setZValue(5);
}

void TaskItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->modifiers() == Qt::AltModifier && _isResizing) {
        _isResizing = false;
    } else if (event->modifiers() != Qt::ShiftModifier) {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void TaskItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::ShiftModifier) {
            LOG(INFO) << "Custom item left clicked with shift key.";
            // add the item to the selection
            setSelected(true);
            updateTaskItem(SELECTED);
        } else if (event->modifiers() == Qt::AltModifier) {
            LOG(INFO) << "Custom item left clicked with alt key.";
            // resize the item
            double radius = boundingRect().width() / 2.0;
            _center = QPointF(boundingRect().topLeft().x() + pos().x() + radius,
                              boundingRect().topLeft().y() + pos().y() + radius);
            QPointF pos = event->scenePos();
            double dist = sqrt(std::pow(_center.x() - pos.x(), 2) + pow(_center.y() - pos.y(), 2));
            if (dist / radius > 0.8) {
                LOG(INFO) << dist << radius << dist / radius;
                _isResizing = true;
            } else {
                _isResizing = false;
            }
        } else {
            LOG(INFO) << "Custom item left clicked.";
            QGraphicsItem::mousePressEvent(event);
            event->accept();
        }
    } else if (event->button() == Qt::RightButton) {
        LOG(INFO) << "Custom item right clicked.";
        event->ignore();
    }
}

void TaskItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->modifiers() == Qt::AltModifier && _isResizing) {
        QPointF pos = event->scenePos();
        double dist = sqrt(pow(_center.x() - pos.x(), 2) + pow(_center.y() - pos.y(), 2));
        setRect(_center.x() - this->pos().x() - dist, _center.y() - this->pos().y() - dist, dist * 2, dist * 2);
    } else if (event->modifiers() != Qt::AltModifier) {
        updateTaskItem(UPDATE);
        QGraphicsItem::mouseMoveEvent(event);
//        LOG(INFO) << "moved" << pos().x() << pos().y();
    }
}

int TaskItem::type() const {
    return UserType + 1;
}

QPointF TaskItem::getBestArch(QPointF point) {
    auto rect = getBoundRect();
    std::vector<QPointF> points;
    points.push_back(QPointF(rect.x() + rect.width() / 2, rect.y()));
    points.push_back(QPointF(rect.x() + rect.width(), rect.y() + rect.height() / 2));
    points.push_back(QPointF(rect.x() + rect.width() / 2, rect.y() + rect.height()));
    points.push_back(QPointF(rect.x(), rect.y() + rect.height() / 2));
    std::sort(points.begin(), points.end(), [point](auto &p1, auto &p2) {
        double powx1 = std::pow(point.x() - p1.x(), 2);
        double powy1 = std::pow(point.y() - p1.y(), 2);

        double powx2 = std::pow(point.x() - p2.x(), 2);
        double powy2 = std::pow(point.y() - p2.y(), 2);
        return std::pow((powx1 + powy1), 0.5) < std::pow((powx2 + powy2), 0.5);
    });
    return points[0];
}

QRect TaskItem::getBoundRect() {
    auto p = mapToScene(rect().topLeft());
//    auto p = rect().topLeft();
//    auto mp = mapToScene(p);
    int x = p.x(), y = p.y(), w = rect().width(), h = rect().height();
//    LOG(INFO) << _text.toStdString() << " " << x << "    " << y << "    " << w << "    " << h << "    ";
    return QRect(x, y, w - 6, h - 6);
}

void TaskItem::addTaskItemUpdateFunction(TaskItemUpdateFunction function) {
    _updateFunctions.push_back(function);
}

void TaskItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
//    QGraphicsRectItem::paint(painter, option, widget);
    QFontMetrics fm(painter->font());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    int pixelsWide = fm.width(_text) + 24;
    int x = rect().x(), y = rect().y(), w = rect().width(), h = rect().height();
    painter->setClipRect(x, y, w, h);
    painter->drawRoundRect(rect().x() + 6, rect().y() + 6, w - 12, h - 12, 5, 10);
    painter->drawStaticText(rect().x() + pixelsWide / 2, rect().y() + fm.height(), QStaticText{_text});
//    setRect(rect().x(), rect().y(), pixelsWide, rect().height());
//    LOG(INFO) << pos().x() << "  " << pos().y() << "  " << rect().width() << "  " << rect().height();
    if (isSelected()) {
        QPen pen(Qt::DashDotDotLine);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawRect(x, y, w, h);
    }
    getBoundRect();
}

void TaskItem::paintSelectRectangle(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

}

void TaskItem::setText(QString text) {
    _text = text;
}

void TaskItem::updateTaskItem(TaskItemState type) {
    for (auto &f: _updateFunctions) {
        f(this, type);
    }
}
