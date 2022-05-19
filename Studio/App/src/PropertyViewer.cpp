#include "PropertyViewer.h"
#include "ui_PropertyViewer.h"
#include "glog/logging.h"

PropertyViewer::PropertyViewer(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PropertyViewer) {
    ui->setupUi(this);
    _propertyTableView = ui->_propertyTableView;
    _detailTableView = ui->_detailPropertyView;

}

PropertyViewer::~PropertyViewer() {
    delete ui;
}
