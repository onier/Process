#include "ProcessEditor.h"
#include "MainWindow.h"
#include "QListView"
#include <QLabel>
#include "ui_MainWindow.h"
#include "glog/logging.h"
#include "TaskListView.h"
#include "Library.h"
#include "Task.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
//    ui->toolBar->addAction("text", []() {
//        LOG(INFO)<<"test";
//    });
    init();
}

TaskListModel *MainWindow::createTaskItemModel() {
    TaskListModel *taskListModel = new TaskListModel();
    auto tasks = puppy::common::library::get<Process::Task>("Task");
    for (auto task:tasks) {
        taskListModel->_taskItems.push_back({task->get_type().get_name().data()});
    }
//
//    taskListModel->_taskItems.push_back({"Rect", RECT});
//    taskListModel->_taskItems.push_back({"ExclusiveGateway", ExclusiveGateway});
//    taskListModel->_taskItems.push_back({"ParallelShape", ParallelGateway});
//    taskListModel->_taskItems.push_back({"EventShape", EventGateway});
    return taskListModel;
}

void MainWindow::init() {
    _taskDockerWidget = new QDockWidget(this);
    _taskDockerWidget->setWindowTitle("Tasks");
    TaskListView *listView = new TaskListView(_taskDockerWidget);
    listView->setTaskModel(createTaskItemModel());
    _taskDockerWidget->setWidget(listView);

    _propertiesDockerWidget = new QDockWidget();
    addDockWidget(Qt::LeftDockWidgetArea, _taskDockerWidget);
    addDockWidget(Qt::RightDockWidgetArea, _propertiesDockerWidget);
    setCentralWidget(new ProcessEditor());
}

MainWindow::~MainWindow() {
    delete ui;
}
