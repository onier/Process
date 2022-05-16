//
// Created by xuzhenhai on 2022/5/10.
//

#ifndef PUPPY_TASKITEM_H
#define PUPPY_TASKITEM_H

#include "QGraphicsEllipseItem"
#include "QGraphicsSceneMouseEvent"

class TaskItem;

enum TaskItemState {
    UPDATE = 0x1,
    SELECTED = 0x02
};
typedef std::function<void(TaskItem *, TaskItemState type)> TaskItemUpdateFunction;

class TaskItem : public QGraphicsRectItem {
public:
    TaskItem();

    void setText(QString text);

    QRect getBoundRect();

    void addTaskItemUpdateFunction(TaskItemUpdateFunction function);

    void updateTaskItem(TaskItemState state);

    QPointF getBestArch(QPointF pointF);
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    int type() const override;

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void paintSelectRectangle(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:
    QPointF _center;
    bool _isResizing;
    QString _text;
    std::vector<TaskItemUpdateFunction> _updateFunctions;
};


#endif //PUPPY_TASKITEM_H
