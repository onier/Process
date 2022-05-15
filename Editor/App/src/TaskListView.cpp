//
// Created by xuzhenhai on 2022/5/11.
//

#include "TaskListView.h"
#include "QMouseEvent"
#include "QDrag"
#include "QMimeData"
#include "glog/logging.h"

TaskListView::TaskListView(QWidget *parent) : QListView(parent) {

}

void TaskListView::setTaskModel(TaskListModel *taskListModel) {
    setModel(taskListModel);
    _taskListModel = taskListModel;
}

void TaskListView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == 1)
        _isPress = true;
    LOG(INFO) << "mousePressEvent" << " " << event->button();;
    QListView::mousePressEvent(event);
}

void TaskListView::mouseMoveEvent(QMouseEvent *e) {
    if (_isPress) {
        LOG(INFO) << "enable drug";
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        if (selectedIndexes().empty()) {
            return;
        }
        int row = QListView::selectedIndexes().at(0).row();
        auto item = _taskListModel->_taskItems.at(row);
//        mimeData->setText(item._text);
        mimeData->setData("application/shape_icon",item._text.toUtf8());
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap::fromImage(item._image));
        Qt::DropAction dropAction = drag->exec();
    }
    QListView::mouseMoveEvent(e);
}

void TaskListView::mouseReleaseEvent(QMouseEvent *e) {
    _isPress = false;
    QListView::mouseReleaseEvent(e);
}

int TaskListModel::rowCount(const QModelIndex &parent) const {
    return _taskItems.size();
}

QVariant TaskListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return _taskItems.at(index.row())._text;
    else
        return QVariant();
}

TaskItemIcon::TaskItemIcon(const QImage &image) : _image(image) {}

TaskItemIcon::TaskItemIcon(const QString &text) : _text(text) {}
