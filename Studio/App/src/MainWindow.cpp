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
    _processStudio->addProperyMessageHandler("Text", [&](auto task) {
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
    _delegate = 0;
    _tableModel = 0;

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
    _propertyViewer = new PropertyViewer(_propertiesDockerWidget);
    _propertiesDockerWidget->setWidget(_propertyViewer);
    _propertTableView = _propertyViewer->_propertyTableView;
//    connect(_propertTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
//            [&](const QItemSelection &selected, const QItemSelection &deselected) {
//        LOG(INFO)<<selected.indexes()[0].row();
//    });
    connect(
            _propertTableView,
            &QTableView::doubleClicked,
            [&]() {
                LOG(INFO) << "double click";
                int row = _propertTableView->selectionModel()->selectedIndexes().at(0).row();
                auto property = _tableModel->getProperties().at(row);
                if (property.get_type().is_sequential_container()) {

                    _detailTableModel = new puppy::common::QRTTRVectorTableModel(
                            std::make_shared<rttr::variant>(_tableModel->getVariantValue(property)),
                            std::make_shared<rttr::type>(property.get_type()));
                    _detailTableView->setModel(_detailTableModel);
                    _detailTableDelegate = new  puppy::common::RTTRVectorItemDelegate(_detailTableModel);
                    _detailTableView->setItemDelegate(_detailTableDelegate);
                } else {
                    _detailTableView->setModel(nullptr);
                }
            }
    );

    _detailTableView = _propertyViewer->_detailTableView;
    _propertTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    addDockWidget(Qt::LeftDockWidgetArea, _taskDockerWidget);
    addDockWidget(Qt::RightDockWidgetArea, _propertiesDockerWidget);
    setCentralWidget(_processEditor);

    _processStudio->addTaskShapeItemSelectEventHanlder([&](ProcessStudio *studio, std::shared_ptr<TaskShapeItem> item) {
        if (item) {
            if (_processStudio->getCurrentSelectShape()) {
                if (_delegate) {
                    delete _delegate;
                }
                if (_tableModel) {
                    delete _tableModel;
                }
                _tableModel = new puppy::common::QRTTRTableModel(item->_taskVar);
                _delegate = new puppy::common::RTTRItemDelegate(_tableModel);
                _propertTableView->setItemDelegate(_delegate);
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

    _processStudio->addRuleShapeItemSelectEventHanlder([&](ProcessStudio *studio, std::shared_ptr<RuleShapeItem> item) {
        if (item) {
            if (_delegate) {
                delete _delegate;
            }
            if (_tableModel) {
                delete _tableModel;
            }
            _tableModel = new puppy::common::QRTTRTableModel(item->_rule);
            _delegate = new puppy::common::RTTRItemDelegate(_tableModel);
            _propertTableView->setItemDelegate(_delegate);
            _tableModel->addValueChangeEvents([&, item](int r, int c) {
                item->_shape->setText(item->_rule->get_type().get_property("Text").get_value(*item->_rule).to_string());
            });
//            _tableModel->setType(item->_taskType);
            _propertTableView->setModel(_tableModel);
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
}
