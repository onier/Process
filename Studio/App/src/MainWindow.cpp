#include "ProcessEditor.h"
#include "MainWindow.h"
#include "QListView"
#include <QLabel>
#include "ui_MainWindow.h"
#include "glog/logging.h"
#include "TaskListView.h"
#include "Library.h"
#include "Task.h"
#include "QRTTRTableModel.h"
#include "QHeaderView"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    _processEditor = new ProcessEditor;
    _processStudio = std::make_shared<ProcessStudio>();
    _processStudio->addProperyMessageHandler("Text",[&](auto task){
        _processEditor->update();
    });
    _processEditor->setProcessStudio(_processStudio);
    init();
    ui->toolBar->addAction("Start", [&]() {
        _processEditor->setEnableEdit(false);
        _processStudio->startProcess();
    });
    ui->toolBar->addAction("Stop", [&]() {
        _processEditor->setEnableEdit(true);
        _processStudio->stopProcess();
    });
    ui->toolBar->addAction("Pause", []() {

    });
}

TaskListModel *MainWindow::createTaskItemModel() {
    TaskListModel *taskListModel = new TaskListModel();
    auto tasks = puppy::common::library::get<Process::Task>("Task");
    for (auto task: tasks) {
        taskListModel->_taskItems.push_back({task->get_type().get_name().data()});
    }
    return taskListModel;
}

void MainWindow::init() {
    _taskDockerWidget = new QDockWidget(this);
    _taskDockerWidget->setWindowTitle("Tasks");
    TaskListView *listView = new TaskListView(_taskDockerWidget);
    listView->setTaskModel(createTaskItemModel());
    _taskDockerWidget->setWidget(listView);

    _propertiesDockerWidget = new QDockWidget();
    _propertTableView = new QTableView(_propertiesDockerWidget);
    _propertTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _propertiesDockerWidget->setWidget(_propertTableView);
    addDockWidget(Qt::LeftDockWidgetArea, _taskDockerWidget);
    addDockWidget(Qt::RightDockWidgetArea, _propertiesDockerWidget);
    setCentralWidget(_processEditor);

    _processStudio->addSelectEventHanlder([&](ProcessStudio *studio, std::shared_ptr<TaskShapeItem> item) {
        if (item) {
            if (_processStudio->getCurrentSelectShape()) {
                if (_tableModel) {
                    delete _tableModel;
                }
                _tableModel = new puppy::common::QRTTRTableModel(item->_taskVar);
                _tableModel->addValueChangeEvents([&, item](int r, int c) {
                    item->_shape->setText(item->_task->_name);
                });
                _tableModel->setType(item->_taskType);
                _propertTableView->setModel(_tableModel);
            }
        } else {
            _tableModel = new puppy::common::QRTTRTableModel(_processStudio->_processInfo);
            _propertTableView->setModel(_tableModel);
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
}
