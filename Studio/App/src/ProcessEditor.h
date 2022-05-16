//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_PROCESSEDITOR_H
#define PUPPY_PROCESSEDITOR_H

#include "ProcessStudio.h"
#include "QWidget"
#include "rttr/registration.h"

class ProcessEditor : public QWidget {
Q_OBJECT
public:
    ProcessEditor(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void setProcessStudio(std::shared_ptr<ProcessStudio> ps);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    int _status;
    bool _isEnableAction;
    bool _isEnableMove;
    std::shared_ptr<Action> _currentAction;
    QPointF _mousePoint, _pressPoint;
    std::shared_ptr<ProcessStudio> _processStudio;
    std::map<std::string, std::shared_ptr<Action>> _actions;
private:
    void initTaskAction();
};


#endif //PUPPY_PROCESSEDITOR_H
