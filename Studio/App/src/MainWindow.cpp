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
    ui->toolBar->addAction("Save", [&]() {
        _processStudio->saveToXML();
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
    _propertTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    addDockWidget(Qt::LeftDockWidgetArea, _taskDockerWidget);
    addDockWidget(Qt::RightDockWidgetArea, _propertiesDockerWidget);
    setCentralWidget(_processEditor);
    _tableModel = new puppy::common::QRTTRTableModel(_processStudio->_processInfoVariant);
    _delegate = new puppy::common::RTTRItemDelegate(_tableModel);
    _propertTableView->setItemDelegate(_delegate);
    _propertTableView->setModel(_tableModel);


    _detailTableView = _propertyViewer->_detailTableView;
    _detailTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    auto property = _processStudio->_processInfoVariant.get_type().get_wrapped_type().get_property("Parameters");
    _detailTableModel = new puppy::common::QRTTRVectorTableModel(_processStudio->_processInfoVariant, property);
    _detailTableDelegate = new puppy::common::RTTRVectorItemDelegate(_detailTableModel);
    _detailTableView->setModel(_detailTableModel);
    _detailTableView->setItemDelegate(_detailTableDelegate);
    connect(
            _propertTableView,
            &QTableView::doubleClicked,
            [tableModel = _tableModel, propertTableView = _propertTableView, processStudio = _processStudio]() {
                LOG(INFO) << "double click";
//                int row = propertTableView->selectionModel()->selectedIndexes().at(0).row();
//                auto property = tableModel->getProperties().at(row);
//                if (property.get_type().is_sequential_container()) {
//                    rttr::variant variant = tableModel->getVariantValue(property);
//                    LOG(INFO) << property.get_name() << "  " << variant.create_sequential_view().get_size();
//                    _detailTableModel = new puppy::common::QRTTRVectorTableModel(variant, property);
//                    _detailTableModel->addValueChangeEvents([&, property](int row, int col) {
//                        property.set_value(_tableModel->_variant, _detailTableModel->_containerVarinat);
//                    });
//                    _detailTableView->setModel(_detailTableModel);
//                    _detailTableDelegate = new puppy::common::RTTRVectorItemDelegate(_detailTableModel);
//                    _detailTableView->setItemDelegate(_detailTableDelegate);
//                } else {
//                    _detailTableView->setModel(nullptr);
//                }
            }
    );

    _processStudio->addTaskShapeItemSelectEventHanlder(
            [tableModel = _tableModel, propertTableView = _propertTableView, processStudio = _processStudio](
                    ProcessStudio *studio, std::shared_ptr<TaskShapeItem> item)mutable {
                if (item) {
                    if (processStudio->getCurrentSelectShape()) {
                        tableModel->_valueChangeEvents.clear();
                        tableModel->addValueChangeEvents([&, item](int r, int c) {
                            item->_shape->setText(item->_task->_name);
                        });
                        tableModel->setVariant(item->_taskVar);
                    }
                } else {
                    tableModel->setVariant(processStudio->_processInfoVariant);
                }
            });

    _processStudio->addRuleShapeItemSelectEventHanlder(
            [tableModel = _tableModel, propertTableView = _propertTableView, processStudio = _processStudio](
                    ProcessStudio *studio, std::shared_ptr<RuleShapeItem> item) {
                if (item) {
                    tableModel->_valueChangeEvents.clear();
                    tableModel->addValueChangeEvents([&, item](int r, int c) {
                        item->_shape->setText(
                                item->_rule.get_type().get_property("Text").get_value(item->_rule).to_string());
                    });
                    tableModel->setVariant(item->_rule);
                }
            });

}

MainWindow::~MainWindow() {
    delete ui;
}
