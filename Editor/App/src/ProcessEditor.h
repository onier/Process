//
// Created by xuzhenhai on 2022/5/12.
//

#ifndef PUPPY_PROCESSEDITOR_H
#define PUPPY_PROCESSEDITOR_H
#include "Process.h"
#include "QWidget"
#include "memory"
#include "Shape.h"
#include "ProcessGraphics.h"
#include "Edge.h"
#include "Action.h"

class ProcessEditor : public QWidget {
Q_OBJECT
public:
    ProcessEditor(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    std::shared_ptr<ProcessGraphics> _graphics;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    int _status;
    bool _isEnableAction;
    bool _isEnableMove;
    QPointF _mousePoint, _pressPoint;
    std::shared_ptr<Shape> _currentSelectShape;
    std::map<std::string, std::shared_ptr<Action>> _actions;
    std::shared_ptr<Action> _currentAction;
    std::shared_ptr<Process::Process> _process;

private:
    void initTaskAction();
};


#endif //PUPPY_PROCESSEDITOR_H
