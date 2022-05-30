#include "TasksWidget.h"
#include "TaskListView.h"
#include "ui_TasksWidget.h"

TasksWidget::TasksWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::TasksWidget) {
    ui->setupUi(this);
}

void TasksWidget::setModel(TaskListModel *taskListModel) {
    ui->QListView->setTaskModel(taskListModel);
}

TasksWidget::~TasksWidget() {
    delete ui;
}
