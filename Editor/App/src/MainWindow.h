#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include "QDockWidget"
#include "tuple"
#include "TaskListView.h"

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
};

#endif // MAINWINDOW_H
