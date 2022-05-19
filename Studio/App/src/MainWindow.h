#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Process.h"
#include "ProcessEditor.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QTableView>
#include "QDockWidget"
#include "tuple"
#include "TaskListView.h"
#include "QRTTRTableModel.h"
#include "QRTTRVectorTableModel.h"
#include "PropertyViewer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    void init();

    TaskListModel *createTaskItemModel();

private:
    Ui::MainWindow *ui;
    QDockWidget *_taskDockerWidget;
    QDockWidget *_propertiesDockerWidget;
    std::shared_ptr<ProcessStudio> _processStudio;
    ProcessEditor *_processEditor;
    QTableView *_propertTableView;
    QTableView *_detailTableView;
    puppy::common::QRTTRTableModel *_tableModel;
    puppy::common::RTTRItemDelegate *_delegate;
    puppy::common::QRTTRVectorTableModel *_detailTableModel;
    puppy::common::RTTRVectorItemDelegate *_detailTableDelegate;
    PropertyViewer *_propertyViewer;
};

#endif // MAINWINDOW_H
