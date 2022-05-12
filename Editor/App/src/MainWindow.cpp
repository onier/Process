#include <QLabel>
#include "MainWindow.h"
#include "QListView"
#include "ui_MainWindow.h"
#include "glog/logging.h"
#include "TaskListView.h"
#include "ProcessEditor.h"

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
    taskListModel->_taskItems.push_back({"Test"});
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
