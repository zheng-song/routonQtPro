#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(0,Qt::Window);
    w.showMaximized();
//    w.setMinimumSize(300,200);
//    w.show();

    return a.exec();
}
