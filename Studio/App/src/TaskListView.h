//
// Created by xuzhenhai on 2022/5/11.
//

#ifndef PUPPY_TASKLISTVIEW_H
#define PUPPY_TASKLISTVIEW_H


#include <tuple>
#include <QString>
#include <QAbstractListModel>
#include <QListView>
#include "Task.h"
struct TaskItemIcon {
    QString _text;
    QImage _image;

    TaskItemIcon(const QString &text);

    TaskItemIcon(const QImage &image);
};

class TaskListModel : public QAbstractListModel {
Q_OBJECT
public:

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;


    std::vector<TaskItemIcon> _taskItems;
};

class TaskListView : public QListView {
Q_OBJECT
public:
    TaskListView(QWidget *parent);

    void setTaskModel(TaskListModel *taskListModel);

protected:
    void mouseMoveEvent(QMouseEvent *e) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    bool _isPress;
    TaskListModel *_taskListModel;
};


#endif //PUPPY_TASKLISTVIEW_H
