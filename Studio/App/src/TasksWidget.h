#ifndef TASKSWIDGET_H
#define TASKSWIDGET_H

#include <QWidget>
#include "TaskListView.h"

namespace Ui {
    class TasksWidget;
}

class TasksWidget : public QWidget {
Q_OBJECT

public:
    explicit TasksWidget(QWidget *parent = nullptr);

    ~TasksWidget();

    void setModel(TaskListModel *taskListModel);

private:
    Ui::TasksWidget *ui;
};

#endif // TASKSWIDGET_H
