
#include "MainWindow.h"
#include "QApplication"
#include "Library.h"
#include <QLabel>
int main(int argc, char *argv[]) {
    puppy::common::library::loadDefaultLibrary();
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
//    QApplication app(argc, argv);
//    QPixmap pm(100,100);
//    pm.fill();
//
//    QPainter p(&pm);
//    p.setRenderHint(QPainter::Antialiasing, true);
//    QPen pen(Qt::blue, 2);
//    p.setPen(pen);
//    QBrush brush(Qt::green);
//    p.setBrush(brush);
//    p.drawEllipse(10, 10, 80, 80);
//    QLabel l;
//    l.setPixmap(pm);
//    l.show();
//    return app.exec();
}