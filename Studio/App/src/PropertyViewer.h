#ifndef PROPERTYVIEWER_H
#define PROPERTYVIEWER_H

#include <QWidget>
#include <QTableView>

namespace Ui {
class PropertyViewer;
}

class PropertyViewer : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyViewer(QWidget *parent = 0);
    ~PropertyViewer();
    QTableView * _propertyTableView;
    QTableView * _detailTableView;
    Ui::PropertyViewer *ui;
};

#endif // PROPERTYVIEWER_H
